#!/usr/bin/env python3
"""Validate solution paths, member/problem folders, encoding and Python syntax."""

from __future__ import annotations

import argparse
import json
import subprocess
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]
MAX_FILE_SIZE = 500_000


def load_study_config() -> dict:
    return json.loads((ROOT / "study-config.json").read_text(encoding="utf-8"))


def changed_files(base: str, head: str) -> list[Path]:
    result = subprocess.run(["git", "diff", "--name-only", "--diff-filter=ACMR", base, head], cwd=ROOT, check=True, capture_output=True, text=True)
    return [ROOT / line for line in result.stdout.splitlines() if line.strip()]


def week_problem_folders(week: str) -> set[str]:
    path = ROOT / "problems" / week / "week.yml"
    if not path.is_file():
        return set()
    data = yaml.safe_load(path.read_text(encoding="utf-8")) or {}
    raw = data.get("problems") or {}
    items = raw if isinstance(raw, list) else [item for group in raw.values() for item in (group or [])]
    return {str(item.get("folder", "")).strip() for item in items if isinstance(item, dict)}


def validate(path: Path, config: dict) -> tuple[list[str], list[str]]:
    errors: list[str] = []; warnings: list[str] = []
    relative = path.relative_to(ROOT).as_posix()
    parts = relative.split("/")
    if len(parts) < 5 or parts[0] != "solutions" or not parts[1].startswith("week"):
        return [f"{relative}: solutions/weekXX/개인폴더/문제폴더/파일.py 위치에 있어야 합니다."], warnings
    week, member, problem = parts[1], parts[2], parts[3]
    members = {str(item.get("folder", "")) for item in config.get("members", [])}
    if member not in members:
        errors.append(f"{relative}: study-config.json에 등록되지 않은 개인 폴더입니다.")
    problems = week_problem_folders(week)
    if not problems:
        errors.append(f"{relative}: problems/{week}/week.yml을 찾을 수 없거나 문제 목록이 비어 있습니다.")
    elif problem not in problems:
        errors.append(f"{relative}: week.yml에 등록되지 않은 문제 폴더입니다.")
    if not path.exists():
        return errors, warnings
    if path.stat().st_size == 0:
        errors.append(f"{relative}: 빈 파일입니다.")
    elif path.stat().st_size > MAX_FILE_SIZE:
        errors.append(f"{relative}: 파일 크기가 500KB를 초과합니다.")
    try:
        content = path.read_text(encoding="utf-8")
    except UnicodeDecodeError:
        errors.append(f"{relative}: UTF-8로 읽을 수 없습니다."); return errors, warnings
    try:
        compile(content, str(path), "exec")
    except SyntaxError as error:
        location = f"{error.lineno}:{error.offset}" if error.lineno else "위치 미상"
        errors.append(f"{relative}: Python 문법 오류 ({location}) {error.msg}")
    if "input(\"" in content or "input('" in content:
        warnings.append(f"{relative}: input() 안내 문자열은 온라인 저지 출력 오류를 만들 수 있습니다.")
    return errors, warnings


def main() -> int:
    parser = argparse.ArgumentParser(); parser.add_argument("--base"); parser.add_argument("--head"); args = parser.parse_args()
    if bool(args.base) != bool(args.head):
        print("ERROR: --base와 --head는 함께 사용해야 합니다."); return 2
    config = load_study_config()
    candidates = changed_files(args.base, args.head) if args.base else sorted((ROOT / "solutions").rglob("*.py"))
    files = [p for p in candidates if p.suffix.lower() == ".py" and "solutions" in p.parts]
    if not files:
        print("검사할 풀이 Python 파일이 없습니다."); return 0
    errors = []; warnings = []
    for path in files:
        e, w = validate(path, config); errors += e; warnings += w
        if not e: print(f"PASS: {path.relative_to(ROOT)}")
    if warnings:
        print("\nWARNINGS"); [print(f"- {w}") for w in warnings]
    if errors:
        print("\nERRORS"); [print(f"- {e}") for e in errors]; return 1
    print(f"\n검사 통과: 파일 {len(files)}개, 경고 {len(warnings)}개"); return 0


if __name__ == "__main__":
    raise SystemExit(main())
