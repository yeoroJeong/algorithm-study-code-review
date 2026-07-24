#!/usr/bin/env python3
"""Create missing problem and per-member submission folders from week.yml files."""

from __future__ import annotations

import argparse
from pathlib import Path

from generate_week_readmes import (
    ConfigError,
    ROOT,
    load_config,
    normalize_members,
    normalize_problems,
    normalize_submission,
)


def sync_week(config_path: Path) -> list[Path]:
    config = load_config(config_path)
    members = normalize_members(config.get("members"))
    problems = normalize_problems(config.get("problems"))
    submission = normalize_submission(config)
    week_name = config_path.parent.name
    created: list[Path] = []

    for problem in problems:
        problem_meta = config_path.parent / problem["folder"]
        problem_meta.mkdir(parents=True, exist_ok=True)
        keep = problem_meta / ".gitkeep"
        if not keep.exists():
            keep.touch(); created.append(keep)

    for member in members:
        member_dir = ROOT / submission["root"] / week_name / member["folder"]
        member_dir.mkdir(parents=True, exist_ok=True)
        root_keep = member_dir / ".gitkeep"
        if root_keep.exists():
            root_keep.unlink()
        for problem in problems:
            problem_dir = member_dir / problem["folder"]
            problem_dir.mkdir(parents=True, exist_ok=True)
            keep = problem_dir / ".gitkeep"
            if not keep.exists():
                keep.touch(); created.append(keep)
    return created


def main() -> int:
    parser = argparse.ArgumentParser()
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
