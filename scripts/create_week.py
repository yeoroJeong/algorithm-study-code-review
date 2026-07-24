#!/usr/bin/env python3
"""Create a problem document and member solution folders for one study week."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIG_PATH = ROOT / "study-config.json"


def load_config() -> dict:
    with CONFIG_PATH.open(encoding="utf-8") as file:
        return json.load(file)


def normalized_week(value: int) -> str:
    if value < 1 or value > 99:
        raise ValueError("주차는 1부터 99 사이여야 합니다.")
    return f"week{value:02d}"


def make_problem_readme(week_number: int, start: str, end: str, members: list[str]) -> str:
    period = f"{start or '미정'} ~ {end or '미정'}"
    rows = "\n".join(f"| {name} | ⬜ 미제출 | - | - |" for name in members)

    return f"""# {week_number}주차 문제 목록

- **진행 기간**: {period}
- **PR 마감**: 미정
- **리뷰 마감**: 미정

## 문제

| 번호 | 플랫폼 | 문제 | 난이도 | 알고리즘 | 발표자 |
|---|---|---|---|---|---|
| 1 | 미정 | [문제명](문제 링크) | 미정 | 미정 | 미정 |
| 2 | 미정 | [문제명](문제 링크) | 미정 | 미정 | 미정 |
| 3 | 자유 | [문제명](문제 링크) | 미정 | 미정 | 미정 |

## 제출 및 리뷰 현황

| 스터디원 | 제출 | PR | 리뷰 완료 |
|---|---|---|---|
{rows}

상태 표기:

- ⬜ 미제출
- 🟡 진행 중
- ✅ 완료
- 🔁 수정 필요

## 이번 주 목표

- [ ] 모든 필수 문제 풀이
- [ ] 사람마다 주간 PR 한 개 생성
- [ ] 다른 스터디원의 PR 최소 한 개 리뷰
- [ ] 복습 문제와 반복 실수 기록

## 회고

- 어려웠던 문제:
- 새롭게 배운 내용:
- 다음 주에 보완할 점:
"""


def create_week(week_number: int, start: str, end: str, force: bool) -> list[Path]:
    config = load_config()
    members = [member.get("folder") or member["name"] for member in config["members"]]
    week = normalized_week(week_number)

    problem_dir = ROOT / "problems" / week
    solution_dir = ROOT / "solutions" / week
    readme = problem_dir / "README.md"

    if readme.exists() and not force:
        raise FileExistsError(f"이미 존재합니다: {readme.relative_to(ROOT)}")

    problem_dir.mkdir(parents=True, exist_ok=True)
    solution_dir.mkdir(parents=True, exist_ok=True)
    readme.write_text(
        make_problem_readme(week_number, start, end, members),
        encoding="utf-8",
        newline="\n",
    )

    created = [readme]
    for name in members:
        member_dir = solution_dir / name
        member_dir.mkdir(parents=True, exist_ok=True)
        keep = member_dir / ".gitkeep"
        if not keep.exists():
            keep.touch()
        created.append(keep)

    return created


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="새 알고리즘 스터디 주차 구조를 생성합니다.")
    parser.add_argument("week", type=int, help="주차 번호 (예: 2)")
    parser.add_argument("--start", default="", help="시작일 YYYY-MM-DD")
    parser.add_argument("--end", default="", help="종료일 YYYY-MM-DD")
    parser.add_argument("--force", action="store_true", help="기존 주차 README 덮어쓰기")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        paths = create_week(args.week, args.start, args.end, args.force)
    except (FileExistsError, ValueError) as error:
        print(f"ERROR: {error}")
        return 1

    print("생성 완료:")
    for path in paths:
        print(f"- {path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
