#!/usr/bin/env python3
"""Validate pull request title and required body sections from a GitHub event file."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path

TITLE_PATTERN = re.compile(r"^\[(W\d{2}|DOCS|CHORE|FIX)\]\s+\S.+$")
WEEKLY_SECTIONS = [
    "## 📌 주간 제출 정보",
    "## 🧩 풀이 목록",
    "## 💡 핵심 접근",
    "## ⏱️ 복잡도",
    "## 🔍 리뷰 요청",
    "## ✅ 체크리스트",
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="PR 제목과 본문을 검사합니다.")
    parser.add_argument("--event", required=True, help="GitHub event JSON 경로")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    event = json.loads(Path(args.event).read_text(encoding="utf-8"))
    pull_request = event.get("pull_request", {})
    title = (pull_request.get("title") or "").strip()
    body = pull_request.get("body") or ""
    errors: list[str] = []

    if not TITLE_PATTERN.fullmatch(title):
        errors.append(
            "PR 제목은 '[W01] 이름 주간 풀이', '[DOCS] ...', '[CHORE] ...', '[FIX] ...' 형식이어야 합니다."
        )

    if title.startswith("[W"):
        missing = [section for section in WEEKLY_SECTIONS if section not in body]
        if missing:
            errors.append("PR 본문 필수 섹션 누락: " + ", ".join(missing))

        if "링크" in body and "https://" not in body and "http://" not in body:
            errors.append("주간 풀이 PR에는 최소 한 개의 문제 링크를 작성해주세요.")

        if "집중적으로 확인받고 싶은 코드:" in body:
            after = body.split("집중적으로 확인받고 싶은 코드:", 1)[1].splitlines()
            first_value = next((line.strip(" -") for line in after if line.strip()), "")
            if not first_value or first_value.startswith("고민했던 대안"):
                errors.append("리뷰 요청에 확인받고 싶은 부분을 구체적으로 작성해주세요.")

    if errors:
        print("PR 정책 검사 실패")
        for error in errors:
            print(f"- {error}")
        return 1

    print(f"PR 정책 검사 통과: {title}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
