#!/usr/bin/env python3
"""Create missing problem/member folders from every week.yml."""

from __future__ import annotations

import argparse
from pathlib import Path

from generate_week_readmes import (
    ConfigError,
    ROOT,
    load_config,
    normalize_members,
    normalize_problems,
)


def sync_week(config_path: Path) -> list[Path]:
    config = load_config(config_path)
    members = normalize_members(config.get("members"))
    problems = normalize_problems(config.get("problems"))
    week_directory = config_path.parent
    created: list[Path] = []

    for problem in problems:
        problem_directory = week_directory / problem["folder"]
        problem_directory.mkdir(parents=True, exist_ok=True)

        legacy_keep = problem_directory / ".gitkeep"
        if legacy_keep.exists():
            legacy_keep.unlink()

        for member in members:
            member_directory = problem_directory / member["folder"]
            member_directory.mkdir(parents=True, exist_ok=True)
            keep = member_directory / ".gitkeep"
            has_real_file = any(
                path.is_file() and path.name != ".gitkeep"
                for path in member_directory.rglob("*")
            )
            if has_real_file:
                if keep.exists():
                    keep.unlink()
            elif not keep.exists():
                keep.touch()
                created.append(keep)

    return created


def main() -> int:
    parser = argparse.ArgumentParser(description="문제별 개인 제출 폴더를 동기화합니다.")
    parser.add_argument("--week", help="예: week02. 생략하면 전체 주차")
    args = parser.parse_args()

    if args.week:
        configs = [ROOT / "problems" / args.week / "week.yml"]
    else:
        configs = sorted((ROOT / "problems").glob("week*/week.yml"))

    try:
        for path in configs:
            if not path.is_file():
                raise ConfigError(f"설정 파일을 찾을 수 없습니다: {path}")
            created = sync_week(path)
            print(f"SYNCED: {path.parent.name} (새 파일 {len(created)}개)")
    except ConfigError as error:
        print(f"ERROR: {error}")
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
