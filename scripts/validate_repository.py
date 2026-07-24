#!/usr/bin/env python3
"""Validate solution paths, file names, sizes, encoding and Python syntax."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIG_PATH = ROOT / "study-config.json"
MAX_FILE_SIZE = 500_000
PATH_PATTERN = re.compile(r"^solutions/(week\d{2})/([^/]+)/([^/]+\.py)$")


def load_config() -> dict:
    with CONFIG_PATH.open(encoding="utf-8") as file:
        return json.load(file)


def git_changed_files(base: str, head: str) -> list[Path]:
    result = subprocess.run(
        ["git", "diff", "--name-only", "--diff-filter=ACMR", base, head],
        cwd=ROOT,
        check=True,
        capture_output=True,
        text=True,
    )
    return [ROOT / line for line in result.stdout.splitlines() if line.strip()]


def all_solution_files() -> list[Path]:
    return sorted((ROOT / "solutions").rglob("*.py"))


def validate_file(path: Path, config: dict) -> tuple[list[str], list[str]]:
    errors: list[str] = []
    warnings: list[str] = []
    relative = path.relative_to(ROOT).as_posix()

    match = PATH_PATTERN.fullmatch(relative)
    if not match:
        errors.append(
            f"{relative}: 풀이 파일은 solutions/weekXX/이름/파일.py 위치에 있어야 합니다."
        )
        return errors, warnings

    _, member, filename = match.groups()
    members = {item["name"] for item in config["members"]}
    if member not in members:
        errors.append(f"{relative}: study-config.json에 등록되지 않은 멤버 폴더입니다.")

    filename_pattern = re.compile(config["solution_filename_pattern"])
    if not filename_pattern.fullmatch(filename):
        errors.append(
            f"{relative}: 파일명은 플랫폼_문제번호_문제명.py 형식을 사용해주세요."
        )

    if not path.exists():
        return errors, warnings

    if path.stat().st_size == 0:
        errors.append(f"{relative}: 빈 파일입니다.")
    elif path.stat().st_size > MAX_FILE_SIZE:
        errors.append(f"{relative}: 파일 크기가 500KB를 초과합니다.")

    try:
        content = path.read_text(encoding="utf-8")
    except UnicodeDecodeError:
        errors.append(f"{relative}: UTF-8로 읽을 수 없습니다.")
        return errors, warnings

    try:
        compile(content, str(path), "exec")
    except SyntaxError as error:
        location = f"{error.lineno}:{error.offset}" if error.lineno else "위치 미상"
        errors.append(f"{relative}: Python 문법 오류 ({location})\n{error.msg}")

    header = "\n".join(content.splitlines()[:20])
    metadata_words = ["문제:", "링크:", "시간 복잡도:"]
    missing = [word for word in metadata_words if word not in header]
    if missing:
        warnings.append(
            f"{relative}: 상단 설명에서 {', '.join(missing)} 항목을 찾지 못했습니다."
        )

    if "input(\"" in content or "input('" in content:
        warnings.append(
            f"{relative}: input() 안내 문자열은 온라인 저지 출력 오류를 만들 수 있습니다."
        )

    return errors, warnings


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="알고리즘 풀이 저장소를 검사합니다.")
    parser.add_argument("--base", help="PR base commit SHA")
    parser.add_argument("--head", help="PR head commit SHA")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if bool(args.base) != bool(args.head):
        print("ERROR: --base와 --head는 함께 사용해야 합니다.")
        return 2

    config = load_config()
    if args.base and args.head:
        candidates = git_changed_files(args.base, args.head)
        files = [path for path in candidates if path.suffix == ".py" and "solutions" in path.parts]
    else:
        files = all_solution_files()

    if not files:
        print("검사할 풀이 Python 파일이 없습니다.")
        return 0

    all_errors: list[str] = []
    all_warnings: list[str] = []

    for path in files:
        errors, warnings = validate_file(path, config)
        all_errors.extend(errors)
        all_warnings.extend(warnings)
        if not errors:
            print(f"PASS: {path.relative_to(ROOT)}")

    if all_warnings:
        print("\nWARNINGS")
        for warning in all_warnings:
            print(f"- {warning}")

    if all_errors:
        print("\nERRORS")
        for error in all_errors:
            print(f"- {error}")
        print(f"\n검사 실패: 오류 {len(all_errors)}개, 경고 {len(all_warnings)}개")
        return 1

    print(f"\n검사 통과: 파일 {len(files)}개, 경고 {len(all_warnings)}개")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
