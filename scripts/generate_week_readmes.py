from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import Any

import yaml

ROOT = Path(__file__).resolve().parents[1]
PROBLEMS_ROOT = ROOT / "problems"
DEFAULT_EXTENSIONS = {".py", ".java", ".cpp", ".cc", ".c", ".js", ".ts", ".kt"}
LEVEL_ORDER = ("쉬움", "중간", "어려움")
LEVEL_ICONS = {"쉬움": "🟢", "중간": "🟡", "어려움": "🔴", "미분류": "⚪"}


class ConfigError(ValueError):
    pass


def markdown_escape(value: object) -> str:
    return str(value).replace("|", r"\|").replace("\n", " ").strip()


def normalized_week(value: object, directory_name: str) -> str:
    if isinstance(value, int):
        return f"{value}주차"
    text = str(value or "").strip()
    if text:
        return text if text.endswith("주차") else f"{text}주차"
    digits = "".join(ch for ch in directory_name if ch.isdigit())
    return f"{int(digits)}주차" if digits else directory_name


def normalize_members(raw: Any) -> list[dict[str, str]]:
    if not isinstance(raw, list) or not raw:
        raise ConfigError("members에는 한 명 이상의 스터디원이 필요합니다.")
    result: list[dict[str, str]] = []
    for item in raw:
        if isinstance(item, str):
            folder = item.strip(); name = folder; github = ""
        elif isinstance(item, dict):
            folder = str(item.get("folder") or item.get("id") or "").strip()
            name = str(item.get("name") or folder).strip()
            github = str(item.get("github") or "").strip()
        else:
            raise ConfigError("members 항목은 문자열 또는 객체여야 합니다.")
        if not folder or "/" in folder or "\\" in folder:
            raise ConfigError(f"잘못된 개인 폴더명: {folder!r}")
        result.append({"folder": folder, "name": name, "github": github})
    return result


def normalize_problem_item(item: Any, index: int, level: str) -> dict[str, str]:
    if not isinstance(item, dict):
        raise ConfigError(f"{level} 그룹 {index}번째 문제가 객체가 아닙니다.")
    site = str(item.get("site", "")).strip()
    number = str(item.get("number", "")).strip()
    title = str(item.get("title", "")).strip()
    folder = str(item.get("folder") or f"{site}_{number}").strip().replace(" ", "_")
    if not site or not number or not title or not folder:
        raise ConfigError(f"{level} 그룹 {index}번째 문제의 site, number, title, folder를 확인하세요.")
    if "/" in folder or "\\" in folder:
        raise ConfigError(f"잘못된 문제 폴더명: {folder!r}")
    return {
        "study_level": level,
        "site": site,
        "number": number,
        "title": title,
        "folder": folder,
        "difficulty": str(item.get("difficulty", "-")).strip() or "-",
        "category": str(item.get("category", "-")).strip() or "-",
        "url": str(item.get("url", "")).strip(),
    }


def normalize_problems(raw: Any) -> list[dict[str, str]]:
    if isinstance(raw, list):
        if not raw:
            raise ConfigError("problems에는 한 개 이상의 문제가 필요합니다.")
        return [normalize_problem_item(item, i, "미분류") for i, item in enumerate(raw, 1)]
    if not isinstance(raw, dict) or not raw:
        raise ConfigError("problems에는 문제 목록 또는 쉬움/중간/어려움 그룹이 필요합니다.")
    unknown = set(raw) - set(LEVEL_ORDER)
    if unknown:
        raise ConfigError("지원하지 않는 난이도 그룹: " + ", ".join(sorted(map(str, unknown))))
    result: list[dict[str, str]] = []
    for level in LEVEL_ORDER:
        items = raw.get(level, []) or []
        if not isinstance(items, list):
            raise ConfigError(f"{level} 문제 그룹은 목록이어야 합니다.")
        result.extend(normalize_problem_item(item, i, level) for i, item in enumerate(items, 1))
    if not result:
        raise ConfigError("problems에는 한 개 이상의 문제가 필요합니다.")
    return result


def normalize_submission(config: dict[str, Any]) -> dict[str, Any]:
    raw = config.get("submission") or {}
    if not isinstance(raw, dict):
        raise ConfigError("submission은 객체여야 합니다.")
    root = str(raw.get("root") or "solutions").strip()
    extensions_raw = raw.get("extensions") or sorted(DEFAULT_EXTENSIONS)
    if not isinstance(extensions_raw, list):
        raise ConfigError("submission.extensions는 목록이어야 합니다.")
    extensions: set[str] = set()
    for value in extensions_raw:
        ext = str(value).strip().lower()
        if ext and not ext.startswith("."):
            ext = "." + ext
        if ext:
            extensions.add(ext)
    if not extensions:
        raise ConfigError("허용된 제출 확장자가 없습니다.")
    return {"root": root, "extensions": extensions}


def load_config(path: Path) -> dict[str, Any]:
    try:
        data = yaml.safe_load(path.read_text(encoding="utf-8"))
    except yaml.YAMLError as error:
        raise ConfigError(f"YAML 문법 오류: {error}") from error
    if not isinstance(data, dict):
        raise ConfigError("week.yml 최상위 값은 객체여야 합니다.")
    return data


def has_submission(week_name: str, member_folder: str, problem_folder: str, submission: dict[str, Any]) -> bool:
    directory = ROOT / submission["root"] / week_name / member_folder / problem_folder
    if not directory.is_dir():
        return False
    return any(
        path.is_file() and path.suffix.lower() in submission["extensions"]
        for path in directory.rglob("*")
    )


def problem_link(problem: dict[str, str]) -> str:
    label = markdown_escape(f"{problem['number']} {problem['title']}")
    return f"[{label}]({problem['url']})" if problem["url"] else label


def append_problem_table(lines: list[str], problems: list[dict[str, str]]) -> None:
    lines.extend([
        "| 사이트 | 문제 | 공식 난이도 | 분류 | 문제 폴더 |",
        "|---|---|:---:|---|---|",
    ])
    for problem in problems:
        lines.append("| " + " | ".join([
            markdown_escape(problem["site"]),
            problem_link(problem),
            markdown_escape(problem["difficulty"]),
            markdown_escape(problem["category"]),
            f"`{markdown_escape(problem['folder'])}`",
        ]) + " |")


def render_readme(week_directory: Path, config: dict[str, Any]) -> str:
    week_label = normalized_week(config.get("week"), week_directory.name)
    period = config.get("period") or {}
    if not isinstance(period, dict):
        raise ConfigError("period는 객체여야 합니다.")
    start = str(period.get("start", "")).strip()
    end = str(period.get("end", "")).strip()
    deadline = str(config.get("deadline", "")).strip()
    notes = str(config.get("notes", "")).strip()
    members = normalize_members(config.get("members"))
    problems = normalize_problems(config.get("problems"))
    submission = normalize_submission(config)

    lines = [
        "<!-- AUTO-GENERATED FILE: week.yml과 solutions 폴더를 기준으로 생성됩니다. 직접 수정하지 마세요. -->",
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

    lines.extend(["## 문제 목록", ""])
    grouped = any(p["study_level"] != "미분류" for p in problems)
    if grouped:
        for level in LEVEL_ORDER:
            items = [p for p in problems if p["study_level"] == level]
            if items:
                lines.extend([f"### {LEVEL_ICONS[level]} {level}", ""])
                append_problem_table(lines, items)
                lines.append("")
    else:
        append_problem_table(lines, problems)
        lines.append("")

    lines.extend(["## 제출 현황", ""])
    lines.append("| 스터디원 | " + " | ".join(markdown_escape(p["number"]) for p in problems) + " | 진행률 |")
    lines.append("|---|" + "".join(":---:|" for _ in problems) + ":---:|")
    for member in members:
        statuses = []
        completed = 0
        for problem in problems:
            submitted = has_submission(week_directory.name, member["folder"], problem["folder"], submission)
            statuses.append("✅" if submitted else "❌")
            completed += int(submitted)
        label = f"{markdown_escape(member['name'])} (`{markdown_escape(member['folder'])}`)"
        lines.append(f"| {label} | " + " | ".join(statuses) + f" | **{completed}/{len(problems)}** |")

    lines.extend([
        "",
        "## 제출 규칙",
        "",
        f"- 브랜치: `{week_directory.name}/GitHub아이디`",
        f"- PR 제목: `[W{int(''.join(ch for ch in week_directory.name if ch.isdigit())):02d}] 이름 주간 풀이`",
        f"- 제출 경로: `{submission['root']}/{week_directory.name}/개인폴더/문제폴더/자유로운파일명.확장자`",
        "- 개인 문제 폴더 안에 허용된 코드 파일이 하나라도 있으면 제출로 처리됩니다.",
        "- 다른 스터디원의 PR을 최소 1개 리뷰합니다.",
        "",
    ])
    if notes:
        lines.extend(["## 추가 안내", "", notes, ""])
    lines.extend(["---", "", "`week.yml` 또는 풀이가 `main`에 반영되면 이 문서는 자동 갱신됩니다.", ""])
    return "\n".join(lines)


def find_configs(selected: str | None) -> list[Path]:
    if selected:
        path = PROBLEMS_ROOT / selected / "week.yml"
        if not path.is_file():
            raise ConfigError(f"설정 파일을 찾을 수 없습니다: {path}")
        return [path]
    return sorted(PROBLEMS_ROOT.glob("week*/week.yml"))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--week")
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    try:
        configs = find_configs(args.week)
    except ConfigError as error:
        print(f"ERROR: {error}", file=sys.stderr); return 1
    stale = []
    for config_path in configs:
        readme = config_path.parent / "README.md"
        try:
            rendered = render_readme(config_path.parent, load_config(config_path))
        except ConfigError as error:
            print(f"ERROR [{config_path}]: {error}", file=sys.stderr); return 1
        current = readme.read_text(encoding="utf-8") if readme.exists() else None
        if current == rendered:
            print(f"UNCHANGED: {readme.relative_to(ROOT)}"); continue
        stale.append(readme)
        if args.check:
            print(f"STALE: {readme.relative_to(ROOT)}")
        else:
            readme.write_text(rendered, encoding="utf-8", newline="\n")
            print(f"UPDATED: {readme.relative_to(ROOT)}")
    if args.check and stale:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
