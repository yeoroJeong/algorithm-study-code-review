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
        print("등록된 주차가 없습니다."); return 0
    print("| 주차 | " + " | ".join(m["name"] for m in members) + " | 총 제출 |")
    print("|---|" + "---|" * (len(members) + 1))
    total_all = 0
    for config_path in weeks:
        data = yaml.safe_load(config_path.read_text(encoding="utf-8")) or {}
        raw = data.get("problems") or {}
        items = raw if isinstance(raw, list) else [i for group in raw.values() for i in (group or [])]
        problem_folders = [str(i.get("folder", "")) for i in items]
        exts = {str(e).lower() for e in (data.get("submission", {}).get("extensions") or [".py"])}
        counts = []
        for member in members:
            count = 0
            for problem in problem_folders:
                directory = ROOT / "solutions" / config_path.parent.name / member["folder"] / problem
                if directory.is_dir() and any(p.is_file() and p.suffix.lower() in exts for p in directory.rglob("*")):
                    count += 1
            counts.append(count)
        total = sum(counts); total_all += total
        print(f"| {config_path.parent.name} | " + " | ".join(f"{c}개" if c else "-" for c in counts) + f" | {total}개 |")
    print(f"\n전체 제출 문제 수: {total_all}개"); return 0
if __name__ == "__main__": raise SystemExit(main())
