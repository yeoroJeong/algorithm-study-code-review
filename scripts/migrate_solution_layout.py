#!/usr/bin/env python3
"""Move legacy solutions/week/member/problem files into problems/week/problem/member."""

from __future__ import annotations

import argparse
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LEGACY_ROOT = ROOT / "solutions"


def migrate(dry_run: bool, keep_old: bool) -> int:
    if not LEGACY_ROOT.exists():
        print("기존 solutions 폴더가 없습니다.")
        return 0

    moved = 0
    conflicts: list[str] = []
    for source in sorted(LEGACY_ROOT.rglob("*")):
        if not source.is_file() or source.name == ".gitkeep":
            continue
        relative = source.relative_to(LEGACY_ROOT)
        if len(relative.parts) < 4 or not relative.parts[0].startswith("week"):
            continue
        week, member, problem, *rest = relative.parts
        target = ROOT / "problems" / week / problem / member / Path(*rest)
        if target.exists():
            conflicts.append(f"{source.relative_to(ROOT)} -> {target.relative_to(ROOT)}")
            continue
        print(f"MOVE: {source.relative_to(ROOT)} -> {target.relative_to(ROOT)}")
        if not dry_run:
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.move(str(source), str(target))
        moved += 1

    if conflicts:
        print("충돌 파일:")
        for conflict in conflicts:
            print(f"- {conflict}")
        return 1

    if not dry_run and not keep_old:
        shutil.rmtree(LEGACY_ROOT)
        print("REMOVED: solutions/")

    print(f"완료: 파일 {moved}개")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--keep-old", action="store_true")
    args = parser.parse_args()
    return migrate(args.dry_run, args.keep_old)


if __name__ == "__main__":
    raise SystemExit(main())
