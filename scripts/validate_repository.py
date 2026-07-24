#!/usr/bin/env python3
"""Validate problem-first submission paths and source syntax."""

from __future__ import annotations

import argparse
import json
import py_compile
import subprocess
from pathlib import Path
from typing import Any

import yaml

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_EXTENSIONS = {".py", ".java", ".cpp"}


def git_changed_files(base: str, head: str) -> list[Path]:
    command = ["git", "diff", "--name-only", "--diff-filter=ACMR", base, head]
    result = subprocess.run(command, cwd=ROOT, check=True, capture_output=True, text=True)
    return [ROOT / line.strip() for line in result.stdout.splitlines() if line.strip()]


def load_study_config() -> dict[str, Any]:
    path = ROOT / "study-config.json"
    data = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise ValueError("study-config.json 최상위 값은 객체여야 합니다.")
    return data


def normalize_extensions(study_config: dict[str, Any]) -> set[str]:
    raw = study_config.get("submission_extensions") or sorted(DEFAULT_EXTENSIONS)
    if not isinstance(raw, list):
        raise ValueError("study-config.json의 submission_extensions는 목록이어야 합니다.")
    extensions: set[str] = set()
    for value in raw:
        extension = str(value).strip().lower()
        if extension and not extension.startswith("."):
            extension = "." + extension
        if extension:
            extensions.add(extension)
    return extensions


def flatten_problems(raw: Any) -> list[dict[str, str]]:
    if isinstance(raw, list):
        groups = {"미분류": raw}
    elif isinstance(raw, dict):
        groups = raw
    else:
        raise ValueError("week.yml의 problems 형식이 잘못되었습니다.")

    result: list[dict[str, str]] = []
    for level, items in groups.items():
        if items is None:
            continue
        if not isinstance(items, list):
            raise ValueError(f"week.yml의 '{level}' 문제 목록은 배열이어야 합니다.")
        for item in items:
            if not isinstance(item, dict):
                raise ValueError("week.yml의 각 문제는 객체여야 합니다.")
            folder = str(item.get("folder", "")).strip()
            if not folder:
                raise ValueError("week.yml의 모든 문제에는 folder가 필요합니다.")
            result.append({"folder": folder})
    return result


def load_week_config(week_folder: str) -> tuple[dict[str, Any], set[str], set[str]]:
    path = ROOT / "problems" / week_folder / "week.yml"
    if not path.is_file():
        raise ValueError(f"등록되지 않은 주차입니다: {week_folder}")
    data = yaml.safe_load(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise ValueError(f"{path.relative_to(ROOT)} 최상위 값은 객체여야 합니다.")

    raw_members = data.get("members")
    if not isinstance(raw_members, list) or not raw_members:
        raise ValueError(f"{path.relative_to(ROOT)}의 members를 확인하세요.")
    members = {
        str(item.get("folder") or item.get("id") or "").strip()
        for item in raw_members
        if isinstance(item, dict)
    }
    members.discard("")
    problems = {item["folder"] for item in flatten_problems(data.get("problems"))}
    return data, members, problems


def validate_week_structure(week_folder: str) -> list[str]:
    warnings: list[str] = []
    try:
        _, members, problems = load_week_config(week_folder)
    except (ValueError, yaml.YAMLError) as error:
        return [str(error)]

    week_directory = ROOT / "problems" / week_folder
    for problem in sorted(problems):
        problem_directory = week_directory / problem
        if not problem_directory.is_dir():
            warnings.append(f"자동 생성 예정 문제 폴더: {problem_directory.relative_to(ROOT)}")
            continue
        for member in sorted(members):
            member_directory = problem_directory / member
            if not member_directory.is_dir():
                warnings.append(f"자동 생성 예정 개인 폴더: {member_directory.relative_to(ROOT)}")
    return warnings


def validate_source_file(path: Path, allowed_extensions: set[str]) -> tuple[list[str], list[str]]:
    errors: list[str] = []
    warnings: list[str] = []
    relative = path.relative_to(ROOT)
    parts = relative.parts

    if len(parts) < 5 or parts[0] != "problems" or not parts[1].startswith("week"):
        errors.append(
            f"{relative}: problems/weekXX/문제폴더/개인폴더/파일 위치에 있어야 합니다."
        )
        return errors, warnings

    week_folder, problem_folder, member_folder = parts[1:4]
    extension = path.suffix.lower()
    if extension not in allowed_extensions:
        errors.append(f"{relative}: 허용되지 않은 확장자입니다: {extension}")
        return errors, warnings

    try:
        _, members, problems = load_week_config(week_folder)
    except (ValueError, yaml.YAMLError) as error:
        errors.append(str(error))
        return errors, warnings

    if problem_folder not in problems:
        errors.append(f"{relative}: {week_folder}에 등록되지 않은 문제 폴더입니다: {problem_folder}")
    if member_folder not in members:
        errors.append(f"{relative}: {week_folder}에 등록되지 않은 개인 폴더입니다: {member_folder}")

    if path.stat().st_size == 0:
        warnings.append(f"{relative}: 빈 파일입니다.")
    if path.stat().st_size > 1_000_000:
        warnings.append(f"{relative}: 파일 크기가 1MB를 초과합니다.")

    if extension == ".py":
        try:
            py_compile.compile(str(path), doraise=True)
        except py_compile.PyCompileError as error:
            errors.append(f"{relative}: Python 문법 오류\n{error.msg}")

    return errors, warnings


def main() -> int:
    parser = argparse.ArgumentParser(description="저장소 구조와 풀이 파일을 검사합니다.")
    parser.add_argument("--base")
    parser.add_argument("--head", default="HEAD")
    args = parser.parse_args()

    try:
        study_config = load_study_config()
        allowed_extensions = normalize_extensions(study_config)
    except (ValueError, json.JSONDecodeError) as error:
        print(f"ERROR: {error}")
        return 1

    if args.base:
        candidates = git_changed_files(args.base, args.head)
    else:
        candidates = sorted((ROOT / "problems").glob("week*/**/*"))

    code_files = [
        path for path in candidates
        if path.is_file() and path.suffix.lower() in allowed_extensions
    ]

    week_folders: set[str] = set()
    for path in candidates:
        try:
            relative = path.relative_to(ROOT)
        except ValueError:
            continue
        if len(relative.parts) >= 2 and relative.parts[0] == "problems" and relative.parts[1].startswith("week"):
            week_folders.add(relative.parts[1])

    if not args.base:
        week_folders.update(path.parent.name for path in (ROOT / "problems").glob("week*/week.yml"))

    errors: list[str] = []
    warnings: list[str] = []

    for week_folder in sorted(week_folders):
        warnings.extend(validate_week_structure(week_folder))

    for path in code_files:
        file_errors, file_warnings = validate_source_file(path, allowed_extensions)
        errors.extend(file_errors)
        warnings.extend(file_warnings)

    for warning in warnings:
        print(f"WARNING: {warning}")
    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        return 1

    print(f"VALID: 주차 {len(week_folders)}개, 코드 파일 {len(code_files)}개")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
