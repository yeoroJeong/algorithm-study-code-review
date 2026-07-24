#!/usr/bin/env python3
"""Print solved problem counts per week and member."""

from __future__ import annotations

import json
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    config = json.loads((ROOT / "study-config.json").read_text(encoding="utf-8"))
    members = config["members"]
    weeks = sorted((ROOT / "problems").glob("week*/week.yml"))
    if not weeks:
        print("등록된 주차가 없습니다.")
        return 0

    print("| 주차 | " + " | ".join(member["name"] for member in members) + " | 총 제출 |")
    print("|---|" + "---|" * (len(members) + 1))
    total_all = 0

    for config_path in weeks:
        data = yaml.safe_load(config_path.read_text(encoding="utf-8")) or {}
        raw = data.get("problems") or {}
        items = raw if isinstance(raw, list) else [item for group in raw.values() for item in (group or [])]
        problem_folders = [str(item.get("folder", "")) for item in items]
        extensions = {
            str(extension).lower()
            for extension in (data.get("submission", {}).get("extensions") or [".py"])
        }

        counts: list[int] = []
        for member in members:
            count = 0
            for problem in problem_folders:
                directory = config_path.parent / problem / member["folder"]
                if directory.is_dir() and any(
                    path.is_file() and path.suffix.lower() in extensions
                    for path in directory.rglob("*")
                ):
                    count += 1
            counts.append(count)

        total = sum(counts)
        total_all += total
        print(
            f"| {config_path.parent.name} | "
            + " | ".join(f"{count}개" if count else "-" for count in counts)
            + f" | {total}개 |"
        )

    print(f"\n전체 제출 문제 수: {total_all}개")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
