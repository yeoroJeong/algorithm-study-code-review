#!/usr/bin/env python3
"""Print a Markdown table of solution counts per week and member."""

from __future__ import annotations

import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    config = json.loads((ROOT / "study-config.json").read_text(encoding="utf-8"))
    members = [member["name"] for member in config["members"]]
    weeks = sorted(
        [path for path in (ROOT / "solutions").glob("week*") if path.is_dir()],
        key=lambda path: int(re.search(r"\d+", path.name).group()),
    )

    if not weeks:
        print("등록된 주차가 없습니다.")
        return 0

    print("| 주차 | " + " | ".join(members) + " | 총 풀이 |")
    print("|---|" + "---|" * (len(members) + 1))

    grand_total = 0
    for week in weeks:
        counts = [len(list((week / member).glob("*.py"))) for member in members]
        total = sum(counts)
        grand_total += total
        display = [f"{count}개" if count else "-" for count in counts]
        print(f"| {week.name} | " + " | ".join(display) + f" | {total}개 |")

    print(f"\n전체 풀이 파일: {grand_total}개")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
