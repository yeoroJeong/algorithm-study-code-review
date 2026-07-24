from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import Any

import yaml


REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
PROBLEMS_ROOT = REPOSITORY_ROOT / "problems"
DEFAULT_EXTENSIONS = {".py", ".java", ".cpp", ".cc", ".c", ".js", ".ts", ".kt"}


class ConfigError(ValueError):
    """Raised when a week.yml file is invalid."""


def markdown_escape(value: object) -> str:
    return str(value).replace("|", r"\|").replace("\n", " ").strip()


def normalize_week_label(value: object, directory_name: str) -> str:
    if isinstance(value, int):
        return f"{value}주차"

    text = str(value or "").strip()
    if not text:
        digits = "".join(ch for ch in directory_name if ch.isdigit())
        return f"{int(digits)}주차" if digits else directory_name

    return text if text.endswith("주차") else f"{text}주차"


def normalize_members(raw_members: Any) -> list[dict[str, Any]]:
    if not isinstance(raw_members, list) or not raw_members:
        raise ConfigError("'members'에는 스터디원을 한 명 이상 작성해야 합니다.")

    members: list[dict[str, Any]] = []

    for item in raw_members:
        if isinstance(item, str):
            member_id = item.strip()
            name = member_id
            aliases: list[str] = []
        elif isinstance(item, dict):
            member_id = str(item.get("id", "")).strip()
            name = str(item.get("name") or member_id).strip()
            raw_aliases = item.get("aliases", [])
            if raw_aliases is None:
                raw_aliases = []
            if not isinstance(raw_aliases, list):
                raise ConfigError(f"멤버 '{member_id or name}'의 aliases는 목록이어야 합니다.")
            aliases = [str(alias).strip() for alias in raw_aliases if str(alias).strip()]
        else:
            raise ConfigError("members의 각 항목은 GitHub 아이디 문자열 또는 객체여야 합니다.")

        if not member_id:
            raise ConfigError("모든 멤버에는 id가 필요합니다.")

        members.append(
            {
                "id": member_id,
                "name": name,
                "accepted_stems": {member_id, *aliases},
            }
        )

    return members


def normalize_problems(raw_problems: Any) -> list[dict[str, str]]:
    if not isinstance(raw_problems, list) or not raw_problems:
        raise ConfigError("'problems'에는 문제를 한 개 이상 작성해야 합니다.")

    problems: list[dict[str, str]] = []

    for index, item in enumerate(raw_problems, start=1):
        if not isinstance(item, dict):
            raise ConfigError(f"{index}번째 문제 정보는 객체 형식이어야 합니다.")

        site = str(item.get("site", "")).strip()
        number = str(item.get("number", "")).strip()
        title = str(item.get("title", "")).strip()
        folder = str(item.get("folder", "")).strip()

        if not site or not number or not title:
            raise ConfigError(f"{index}번째 문제에는 site, number, title이 모두 필요합니다.")

        if not folder:
            folder = f"{site}_{number}".replace(" ", "_")

        problems.append(
            {
                "site": site,
                "number": number,
                "title": title,
                "folder": folder,
                "difficulty": str(item.get("difficulty", "-")).strip() or "-",
                "category": str(item.get("category", "-")).strip() or "-",
                "url": str(item.get("url", "")).strip(),
            }
        )

    return problems


def normalize_extensions(config: dict[str, Any]) -> set[str]:
    submission = config.get("submission") or {}
    if not isinstance(submission, dict):
        raise ConfigError("'submission'은 객체 형식이어야 합니다.")

    raw_extensions = submission.get("extensions")
    if raw_extensions is None:
        return set(DEFAULT_EXTENSIONS)

    if not isinstance(raw_extensions, list) or not raw_extensions:
        raise ConfigError("submission.extensions는 비어 있지 않은 목록이어야 합니다.")

    extensions: set[str] = set()
    for value in raw_extensions:
        extension = str(value).strip().lower()
        if not extension:
            continue
        if not extension.startswith("."):
            extension = f".{extension}"
        extensions.add(extension)

    if not extensions:
        raise ConfigError("유효한 제출 파일 확장자가 없습니다.")

    return extensions


def has_submission(problem_directory: Path, accepted_stems: set[str], extensions: set[str]) -> bool:
    if not problem_directory.is_dir():
        return False

    for file_path in problem_directory.rglob("*"):
        if not file_path.is_file():
            continue
        if file_path.suffix.lower() not in extensions:
            continue
        if file_path.stem in accepted_stems:
            return True

    return False


def format_problem_link(problem: dict[str, str]) -> str:
    label = f"{problem['number']} {problem['title']}"
    if problem["url"]:
        return f"[{markdown_escape(label)}]({problem['url']})"
    return markdown_escape(label)


def render_readme(week_directory: Path, config: dict[str, Any]) -> str:
    week_label = normalize_week_label(config.get("week"), week_directory.name)
    period = config.get("period") or {}
    if period and not isinstance(period, dict):
        raise ConfigError("'period'는 start와 end를 포함한 객체여야 합니다.")

    start = str(period.get("start", "")).strip() if period else ""
    end = str(period.get("end", "")).strip() if period else ""
    deadline = str(config.get("deadline", "")).strip()
    notes = str(config.get("notes", "")).strip()

    members = normalize_members(config.get("members"))
    problems = normalize_problems(config.get("problems"))
    extensions = normalize_extensions(config)

    lines = [
        "<!-- AUTO-GENERATED FILE: week.yml과 제출 파일을 기준으로 생성됩니다. 직접 수정하지 마세요. -->",
        "",
        f"# {markdown_escape(week_label)} 알고리즘 문제",
        "",
    ]

    if start or end:
        lines.append(f"- **진행 기간:** {markdown_escape(start)} ~ {markdown_escape(end)}")
    if deadline:
        lines.append(f"- **제출 마감:** {markdown_escape(deadline)}")
    if start or end or deadline:
        lines.append("")

    lines.extend([
        "## 문제 목록",
        "",
        "| 사이트 | 문제 | 난이도 | 분류 | 제출 폴더 |",
        "|---|---|:---:|---|---|",
    ])

    for problem in problems:
        lines.append(
            "| " + " | ".join([
                markdown_escape(problem["site"]),
                format_problem_link(problem),
                markdown_escape(problem["difficulty"]),
                markdown_escape(problem["category"]),
                f"`{markdown_escape(problem['folder'])}`",
            ]) + " |"
        )

    lines.extend(["", "## 제출 현황", ""])
    problem_headers = [markdown_escape(problem["number"]) for problem in problems]
    lines.append("| 스터디원 | " + " | ".join(problem_headers) + " | 진행률 |")
    lines.append("|---|" + "".join(":---:|" for _ in problems) + ":---:|")

    for member in members:
        statuses: list[str] = []
        completed = 0

        for problem in problems:
            submitted = has_submission(
                week_directory / problem["folder"],
                member["accepted_stems"],
                extensions,
            )
            statuses.append("✅" if submitted else "❌")
            completed += int(submitted)

        display_name = markdown_escape(member["name"])
        member_id = markdown_escape(member["id"])
        member_label = (
            f"{display_name} (`{member_id}`)" if display_name != member_id else f"`{member_id}`"
        )
        lines.append(
            f"| {member_label} | " + " | ".join(statuses) + f" | **{completed}/{len(problems)}** |"
        )

    lines.extend([
        "",
        "## 제출 규칙",
        "",
        f"- 브랜치: `{week_directory.name}/GitHub아이디`",
        f"- PR 제목: `[{week_label}] 이름 - 문제 번호`",
        "- 제출 파일명은 기본적으로 `GitHub아이디.확장자` 형식을 사용합니다.",
        "- 다른 스터디원의 PR을 최소 1개 리뷰합니다.",
        "",
    ])

    if notes:
        lines.extend(["## 추가 안내", "", notes, ""])

    lines.extend([
        "---",
        "",
        "`week.yml` 또는 풀이 파일이 `main`에 반영되면 이 문서는 자동 갱신됩니다.",
        "",
    ])

    return "\n".join(lines)


def load_config(config_path: Path) -> dict[str, Any]:
    try:
        data = yaml.safe_load(config_path.read_text(encoding="utf-8"))
    except yaml.YAMLError as error:
        raise ConfigError(f"YAML 문법 오류: {error}") from error

    if not isinstance(data, dict):
        raise ConfigError("week.yml의 최상위 값은 객체 형식이어야 합니다.")
    return data


def find_config_paths(selected_week: str | None) -> list[Path]:
    if selected_week:
        config_path = PROBLEMS_ROOT / selected_week / "week.yml"
        if not config_path.is_file():
            raise ConfigError(f"설정 파일을 찾을 수 없습니다: {config_path}")
        return [config_path]
    return sorted(PROBLEMS_ROOT.glob("week*/week.yml"))


def main() -> int:
    parser = argparse.ArgumentParser(description="주차별 README를 자동 생성합니다.")
    parser.add_argument("--week", help="특정 주차 폴더만 생성합니다. 예: week01")
    parser.add_argument("--check", action="store_true", help="README 최신 상태를 검사만 합니다.")
    args = parser.parse_args()

    try:
        config_paths = find_config_paths(args.week)
    except ConfigError as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1

    if not config_paths:
        print("생성할 week.yml 파일이 없습니다.")
        return 0

    stale_files: list[Path] = []

    for config_path in config_paths:
        week_directory = config_path.parent
        readme_path = week_directory / "README.md"

        try:
            rendered = render_readme(week_directory, load_config(config_path))
        except ConfigError as error:
            print(f"ERROR [{config_path}]: {error}", file=sys.stderr)
            return 1

        current = readme_path.read_text(encoding="utf-8") if readme_path.is_file() else None
        if current == rendered:
            print(f"UNCHANGED: {readme_path.relative_to(REPOSITORY_ROOT)}")
            continue

        stale_files.append(readme_path)
        if args.check:
            print(f"STALE: {readme_path.relative_to(REPOSITORY_ROOT)}")
        else:
            readme_path.write_text(rendered, encoding="utf-8")
            print(f"UPDATED: {readme_path.relative_to(REPOSITORY_ROOT)}")

    if args.check and stale_files:
        print("README가 최신 상태가 아닙니다.", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
