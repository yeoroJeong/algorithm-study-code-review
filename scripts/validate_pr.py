#!/usr/bin/env python3
"""Validate PR title/body and summarize changed problem-first submissions."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Any

import yaml

REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
WEEK_TITLE_PATTERN = re.compile(r"^\[W(?P<week>\d{2})\]\s+.+$")
GENERAL_TITLE_PATTERN = re.compile(r"^\[(DOCS|CHORE|FIX)\]\s+.+$")
REQUIRED_WEEKLY_SECTIONS = ("## 💡 핵심 접근", "## 🔍 리뷰 요청")
PLACEHOLDER_LINES = {"-", "없음", "해당 없음", "n/a", "na"}
LANGUAGE_BY_EXTENSION = {
    ".py": "Python",
    ".java": "Java",
    ".cpp": "C++",
    ".cc": "C++",
    ".c": "C",
    ".js": "JavaScript",
    ".ts": "TypeScript",
    ".kt": "Kotlin",
}


class PolicyError(ValueError):
    pass


@dataclass(frozen=True)
class ChangedSolution:
    status: str
    path: Path
    week_folder: str
    problem_folder: str
    member_folder: str
    extension: str


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--event", required=True)
    parser.add_argument("--base-sha", required=True)
    parser.add_argument("--head-sha", required=True)
    parser.add_argument("--summary-file", required=True)
    return parser.parse_args()


def changed_files(base_sha: str, head_sha: str) -> list[tuple[str, Path]]:
    result = subprocess.run(
        ["git", "diff", "--name-status", "--find-renames", base_sha, head_sha],
        cwd=REPOSITORY_ROOT,
        check=True,
        capture_output=True,
        text=True,
    )
    changes: list[tuple[str, Path]] = []
    for line in result.stdout.splitlines():
        if not line.strip():
            continue
        parts = line.split("\t")
        status = parts[0]
        path_text = parts[-1]
        changes.append((status, Path(path_text)))
    return changes


def extract_section(body: str, heading: str) -> str:
    start = body.find(heading)
    if start < 0:
        return ""
    content_start = start + len(heading)
    next_heading = re.search(r"^##\s+", body[content_start:], re.MULTILINE)
    end = content_start + next_heading.start() if next_heading else len(body)
    return body[content_start:end].strip()


def meaningful_section_content(section: str) -> bool:
    lines = []
    for raw_line in section.splitlines():
        line = raw_line.strip()
        if not line or line.startswith("<!--") or line.startswith("-->"):
            continue
        line = re.sub(r"^[-*+]\s*", "", line).strip()
        if line:
            lines.append(line)
    return bool(lines) and any(line.casefold() not in PLACEHOLDER_LINES for line in lines)


def load_json(path: Path) -> dict[str, Any]:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as error:
        raise PolicyError(f"JSON 문법 오류: {path}: {error}") from error
    if not isinstance(data, dict):
        raise PolicyError(f"{path} 최상위 값은 객체여야 합니다.")
    return data


def load_yaml(path: Path) -> dict[str, Any]:
    try:
        data = yaml.safe_load(path.read_text(encoding="utf-8"))
    except yaml.YAMLError as error:
        raise PolicyError(f"YAML 문법 오류: {path}: {error}") from error
    if not isinstance(data, dict):
        raise PolicyError(f"{path} 최상위 값은 객체여야 합니다.")
    return data


def normalize_extensions(study_config: dict[str, Any]) -> set[str]:
    raw = study_config.get("submission_extensions") or [".py", ".java", ".cpp"]
    if not isinstance(raw, list):
        raise PolicyError("study-config.json의 submission_extensions는 목록이어야 합니다.")
    extensions: set[str] = set()
    for value in raw:
        extension = str(value).strip().lower()
        if extension and not extension.startswith("."):
            extension = "." + extension
        if extension:
            extensions.add(extension)
    return extensions


def parse_solution_changes(
    changes: list[tuple[str, Path]],
    allowed_extensions: set[str],
) -> tuple[list[ChangedSolution], list[str]]:
    solutions: list[ChangedSolution] = []
    malformed: list[str] = []

    for status, path in changes:
        parts = path.parts
        if not parts or parts[0] != "problems":
            continue
        if status.startswith("D"):
            continue

        extension = path.suffix.lower()
        if extension not in allowed_extensions:
            continue

        if len(parts) < 5 or not parts[1].startswith("week"):
            malformed.append(path.as_posix())
            continue

        week_folder, problem_folder, member_folder = parts[1:4]
        solutions.append(ChangedSolution(
            status=status,
            path=path,
            week_folder=week_folder,
            problem_folder=problem_folder,
            member_folder=member_folder,
            extension=extension,
        ))

    return solutions, malformed


def flatten_problems(raw_problems: Any) -> list[dict[str, str]]:
    if isinstance(raw_problems, list):
        groups = {"미분류": raw_problems}
    elif isinstance(raw_problems, dict):
        groups = raw_problems
    else:
        raise PolicyError("week.yml의 problems 형식이 잘못되었습니다.")

    flattened: list[dict[str, str]] = []
    for level, items in groups.items():
        if items is None:
            continue
        if not isinstance(items, list):
            raise PolicyError(f"week.yml의 '{level}' 문제 목록은 배열이어야 합니다.")
        for item in items:
            if not isinstance(item, dict):
                raise PolicyError("week.yml의 각 문제는 객체여야 합니다.")
            folder = str(item.get("folder", "")).strip()
            if not folder:
                continue
            flattened.append({
                "level": str(level),
                "site": str(item.get("site", "-")).strip() or "-",
                "number": str(item.get("number", "-")).strip() or "-",
                "title": str(item.get("title", "-")).strip() or "-",
                "difficulty": str(item.get("difficulty", "-")).strip() or "-",
                "category": str(item.get("category", "-")).strip() or "-",
                "folder": folder,
                "url": str(item.get("url", "")).strip(),
            })
    return flattened


def normalize_members(week_config: dict[str, Any]) -> dict[str, dict[str, str]]:
    raw_members = week_config.get("members")
    if not isinstance(raw_members, list):
        raise PolicyError("week.yml의 members는 목록이어야 합니다.")

    members: dict[str, dict[str, str]] = {}
    for item in raw_members:
        if not isinstance(item, dict):
            continue
        folder = str(item.get("folder") or item.get("id") or "").strip()
        if not folder:
            continue
        members[folder] = {
            "folder": folder,
            "name": str(item.get("name") or folder).strip(),
            "github": str(item.get("github") or item.get("id") or "").strip(),
        }
    return members


def markdown_escape(value: object) -> str:
    return str(value).replace("|", r"\|").replace("\n", " ").strip()


def build_summary(
    week_code: str,
    member: dict[str, str],
    problems: list[dict[str, str]],
    solutions: list[ChangedSolution],
) -> str:
    problem_by_folder = {problem["folder"]: problem for problem in problems}
    submitted_folders = sorted({solution.problem_folder for solution in solutions})

    lines = [
        "## 🤖 PR 자동 분석",
        "",
        f"- **주차:** `{week_code}`",
        f"- **작성자:** {markdown_escape(member['name'])} (`{markdown_escape(member['folder'])}`)",
        f"- **제출 문제 수:** {len(submitted_folders)}개",
        "",
        "### 변경된 풀이",
        "",
        "| 난이도 그룹 | 사이트 | 문제 | 공식 난이도 | 언어 | 변경 파일 |",
        "|---|---|---|:---:|---|---|",
    ]

    for folder in submitted_folders:
        problem = problem_by_folder[folder]
        folder_solutions = [item for item in solutions if item.problem_folder == folder]
        languages = sorted({
            LANGUAGE_BY_EXTENSION.get(item.extension, item.extension.lstrip(".").upper())
            for item in folder_solutions
        })
        file_list = "<br>".join(
            f"`{markdown_escape(item.path.as_posix())}`" for item in folder_solutions
        )
        label = f"{problem['number']} {problem['title']}"
        problem_cell = (
            f"[{markdown_escape(label)}]({problem['url']})"
            if problem["url"] else markdown_escape(label)
        )
        lines.append("| " + " | ".join([
            markdown_escape(problem["level"]),
            markdown_escape(problem["site"]),
            problem_cell,
            markdown_escape(problem["difficulty"]),
            markdown_escape(", ".join(languages)),
            file_list,
        ]) + " |")

    lines.extend([
        "",
        "> 이 목록은 PR에서 변경된 `problems/weekXX/문제폴더/개인폴더/파일` 경로를 기준으로 자동 생성되었습니다.",
    ])
    return "\n".join(lines) + "\n"


def validate_weekly_pr(
    title: str,
    body: str,
    event: dict[str, Any],
    changes: list[tuple[str, Path]],
    summary_path: Path,
) -> list[str]:
    errors: list[str] = []
    title_match = WEEK_TITLE_PATTERN.fullmatch(title)
    if not title_match:
        return ["주간 풀이 PR 제목 형식을 해석할 수 없습니다."]

    week_number = title_match.group("week")
    week_code = f"W{week_number}"
    week_folder = f"week{week_number}"

    for heading in REQUIRED_WEEKLY_SECTIONS:
        section = extract_section(body, heading)
        if not section:
            errors.append(f"PR 본문 필수 섹션 누락: {heading}")
        elif not meaningful_section_content(section):
            errors.append(f"PR 본문을 구체적으로 작성해주세요: {heading}")

    study_config_path = REPOSITORY_ROOT / "study-config.json"
    week_config_path = REPOSITORY_ROOT / "problems" / week_folder / "week.yml"
    if not study_config_path.is_file():
        errors.append("study-config.json을 찾을 수 없습니다.")
        return errors
    if not week_config_path.is_file():
        errors.append(f"등록되지 않은 주차입니다: {week_code}")
        return errors

    try:
        study_config = load_json(study_config_path)
        week_config = load_yaml(week_config_path)
        allowed_extensions = normalize_extensions(study_config)
        solutions, malformed = parse_solution_changes(changes, allowed_extensions)
        members = normalize_members(week_config)
        problems = flatten_problems(week_config.get("problems"))
    except PolicyError as error:
        errors.append(str(error))
        return errors

    if malformed:
        errors.append(
            "풀이 파일은 'problems/weekXX/문제폴더/개인폴더/파일명' 구조여야 합니다: "
            + ", ".join(malformed)
        )

    if not solutions:
        errors.append(
            f"{week_code} 풀이 PR에는 허용된 코드 파일이 최소 한 개 필요합니다. "
            f"예: problems/{week_folder}/문제폴더/개인폴더/solution.py"
        )
        return errors

    changed_weeks = {solution.week_folder for solution in solutions}
    if changed_weeks != {week_folder}:
        errors.append(
            f"PR 제목은 {week_code}인데 변경된 풀이 주차가 다릅니다: "
            + ", ".join(sorted(changed_weeks))
        )

    changed_members = {solution.member_folder for solution in solutions}
    if len(changed_members) != 1:
        errors.append(
            "한 개의 주간 풀이 PR에서는 한 명의 개인 폴더만 변경해주세요: "
            + ", ".join(sorted(changed_members))
        )
        return errors

    member_folder = next(iter(changed_members))
    member = members.get(member_folder)
    if member is None:
        errors.append(
            f"week.yml에 등록되지 않은 개인 폴더입니다: {member_folder}. "
            f"등록된 폴더: {', '.join(sorted(members))}"
        )
        return errors

    problem_by_folder = {problem["folder"]: problem for problem in problems}
    unknown_problem_folders = sorted(
        {solution.problem_folder for solution in solutions} - set(problem_by_folder)
    )
    if unknown_problem_folders:
        errors.append(
            f"{week_code}에 등록되지 않은 문제 폴더입니다: "
            + ", ".join(unknown_problem_folders)
        )
        return errors

    author_login = str(event.get("pull_request", {}).get("user", {}).get("login", "")).strip()
    expected_github = member.get("github", "").strip()
    if expected_github and author_login and expected_github.casefold() != author_login.casefold():
        errors.append(
            f"PR 작성자(@{author_login})와 개인 폴더 소유자(@{expected_github})가 다릅니다."
        )

    if not errors:
        summary_path.write_text(
            build_summary(week_code, member, problems, solutions),
            encoding="utf-8",
        )
    return errors


def main() -> int:
    args = parse_args()
    event = json.loads(Path(args.event).read_text(encoding="utf-8"))
    pull_request = event.get("pull_request", {})
    title = (pull_request.get("title") or "").strip()
    body = pull_request.get("body") or ""
    summary_path = Path(args.summary_file)
    if not summary_path.is_absolute():
        summary_path = REPOSITORY_ROOT / summary_path
    if summary_path.exists():
        summary_path.unlink()

    try:
        changes = changed_files(args.base_sha, args.head_sha)
    except subprocess.CalledProcessError as error:
        print(f"ERROR: 변경 파일 조회 실패: {error}")
        return 1

    if WEEK_TITLE_PATTERN.fullmatch(title):
        errors = validate_weekly_pr(title, body, event, changes, summary_path)
    elif GENERAL_TITLE_PATTERN.fullmatch(title):
        errors = []
    else:
        errors = [
            "PR 제목은 '[W01] 이름 주간 풀이', '[DOCS] 내용', '[CHORE] 내용', '[FIX] 내용' 형식이어야 합니다."
        ]

    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        return 1

    print("VALID: PR 정책을 통과했습니다.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
