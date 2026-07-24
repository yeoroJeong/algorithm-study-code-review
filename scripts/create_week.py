#!/usr/bin/env python3
"""Create week.yml, README, problem folders and per-member folders."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any

import yaml

from generate_week_readmes import ROOT, load_config, render_readme
from sync_week_folders import sync_week

CONFIG_PATH = ROOT / "study-config.json"
LEVELS = {"쉬움", "중간", "어려움"}


def normalized_week(value: int) -> str:
    if not 1 <= value <= 99:
        raise ValueError("주차는 1부터 99 사이여야 합니다.")
    return f"week{value:02d}"


def safe_folder(site: str, number: str) -> str:
    raw = f"{site}_{number}".replace(" ", "_")
    folder = re.sub(r"[^A-Za-z0-9가-힣_-]", "_", raw)
    return re.sub(r"_+", "_", folder).strip("_")


def parse_problem_lines(path: Path) -> dict[str, list[dict[str, str]]]:
    grouped: dict[str, list[dict[str, str]]] = {"쉬움": [], "중간": [], "어려움": []}
    for line_number, raw_line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        parts = [part.strip() for part in line.split("|")]
        if len(parts) != 7:
            raise ValueError(
                f"문제 목록 {line_number}번째 줄은 7개 항목이어야 합니다: "
                "난이도그룹|사이트|번호|제목|공식난이도|분류|링크"
            )

        level, site, number, title, difficulty, category, url = parts
        if level not in LEVELS:
            raise ValueError(f"{line_number}번째 줄 난이도 그룹은 쉬움/중간/어려움 중 하나여야 합니다.")
        if not site or not number or not title:
            raise ValueError(f"{line_number}번째 줄의 사이트, 번호, 제목은 필수입니다.")

        grouped[level].append({
            "site": site,
            "number": number,
            "title": title,
            "difficulty": difficulty or "-",
            "category": category or "-",
            "folder": safe_folder(site, number),
            "url": url,
        })

    if not any(grouped.values()):
        raise ValueError("문제 목록이 비어 있습니다.")
    return grouped


def load_members() -> list[dict[str, str]]:
    config = json.loads(CONFIG_PATH.read_text(encoding="utf-8"))
    members = config.get("members")
    if not isinstance(members, list) or not members:
        raise ValueError("study-config.json에 members가 없습니다.")

    result: list[dict[str, str]] = []
    for member in members:
        folder = str(member.get("folder") or "").strip()
        name = str(member.get("name") or folder).strip()
        github = str(member.get("github") or "").strip()
        if not folder:
            raise ValueError("모든 멤버에는 folder가 필요합니다.")
        item = {"folder": folder, "name": name}
        if github:
            item["github"] = github
        result.append(item)
    return result


def create_week(
    week_number: int,
    start: str,
    end: str,
    deadline: str,
    problems_file: Path,
    force: bool,
) -> list[Path]:
    week = normalized_week(week_number)
    week_directory = ROOT / "problems" / week
    config_path = week_directory / "week.yml"

    if config_path.exists() and not force:
        raise FileExistsError(f"이미 존재합니다: {config_path.relative_to(ROOT)}")
    if not deadline and end:
        deadline = f"{end} 23:59"

    config: dict[str, Any] = {
        "week": week_number,
        "period": {"start": start or "미정", "end": end or "미정"},
        "deadline": deadline or "미정",
        "members": load_members(),
        "problems": parse_problem_lines(problems_file),
        "submission": {"extensions": [".py", ".java", ".cpp"]},
        "notes": (
            "- 파일명은 자유이며, 반드시 해당 문제 폴더 아래 본인의 개인 폴더에 제출합니다.\n"
            "- 다른 스터디원의 PR을 최소 1개 리뷰해주세요.\n"
            "- 리뷰 반영이 끝난 뒤 `main` 브랜치로 병합합니다.\n"
        ),
    }

    week_directory.mkdir(parents=True, exist_ok=True)
    config_path.write_text(
        yaml.safe_dump(config, allow_unicode=True, sort_keys=False, width=1000),
        encoding="utf-8",
        newline="\n",
    )

    created = [config_path]
    created.extend(sync_week(config_path))
    readme = week_directory / "README.md"
    readme.write_text(
        render_readme(week_directory, load_config(config_path)),
        encoding="utf-8",
        newline="\n",
    )
    created.append(readme)
    return created


def main() -> int:
    parser = argparse.ArgumentParser(description="새 주차 구조를 생성합니다.")
    parser.add_argument("week", type=int)
    parser.add_argument("--start", default="")
    parser.add_argument("--end", default="")
    parser.add_argument("--deadline", default="")
    parser.add_argument("--problems-file", type=Path, required=True)
    parser.add_argument("--force", action="store_true")
    args = parser.parse_args()

    try:
        paths = create_week(
            args.week,
            args.start,
            args.end,
            args.deadline,
            args.problems_file,
            args.force,
        )
    except (FileExistsError, ValueError) as error:
        print(f"ERROR: {error}")
        return 1

    print("생성 완료:")
    for path in paths:
        print(f"- {path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
