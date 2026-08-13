# 🐍 Algorithm Study | Python

[![Repository Check](https://github.com/yeoroJeong/algorithm-study-code-review/actions/workflows/repository-check.yml/badge.svg)](https://github.com/yeoroJeong/algorithm-study-code-review/actions/workflows/repository-check.yml)
[![PR Policy](https://github.com/yeoroJeong/algorithm-study-code-review/actions/workflows/pr-policy.yml/badge.svg)](https://github.com/yeoroJeong/algorithm-study-code-review/actions/workflows/pr-policy.yml)

김대원 · 정현수 · 임정현 · 이지은 · 최수빈이 함께 사용하는 **Python 알고리즘 풀이·코드리뷰 저장소**입니다.

정답 코드만 모으는 것이 아니라 다음 세 가지를 남기는 것을 목표로 합니다.

1. 문제를 어떻게 접근했는지
2. 왜 이 알고리즘과 자료구조를 선택했는지
3. 다음에 다시 풀 때 무엇을 조심해야 하는지

---

## 🚀 처음 참여하는 사람

상세 설명은 [`docs/quick-start.md`](docs/quick-start.md)를 확인하세요.

```bash
# 1. 최신 main 받기
git switch main
git pull origin main

# 2. 이번 주 개인 브랜치 만들기
git switch -c week04/github-id

# 3. 풀이 후 저장
git add .
git commit -m "[W04][SWEA-5658] 풀이 추가"
git push -u origin week04/github-id
```

GitHub에서 Pull Request를 만들고 템플릿을 작성하면 됩니다.

> 기본 운영 단위는 **한 사람당 한 주에 PR 하나**입니다. 각 문제는 별도 커밋으로 나누면 리뷰와 복습이 편합니다.

---

## 📅 현재 주차

| 주차 | 문제·풀이 폴더 | 진행 상태 |
|---|---|---|
| 4주차 | [`problems/week04`](problems/week04/README.md) | 진행 중 |

새 주차는 GitHub의 **Actions → Create Weekly Setup → Run workflow**에서 문제 목록과 기간을 입력해 생성합니다. 로컬에서 생성할 때는 같은 형식의 문제 목록 파일을 준비합니다.

```bash
python scripts/create_week.py 4 \
  --start 2026-08-17 \
  --end 2026-08-23 \
  --problems-file week04-problems.txt
```

GitHub Actions를 사용하면 문제별 개인 폴더가 포함된 새 브랜치와 PR까지 자동 생성됩니다.

---

## 🔄 주간 진행 흐름

```text
문제 선정
  ↓
주차 README 등록
  ↓
개인 브랜치에서 풀이
  ↓
한 주차 단위 PR 생성
  ↓
최소 1명 리뷰
  ↓
수정 반영 및 병합
  ↓
복습 문제 기록
```

### 권장 일정 예시

| 시점 | 할 일 |
|---|---|
| 월요일 | 문제 확정 및 주차 문서 생성 |
| 화~금 | 개인 풀이 및 커밋 |
| 토요일 | PR 생성 |
| 일요일 | 리뷰·수정·병합 |

---

## 📁 저장소 구조

```text
.
├── .github/
│   ├── ISSUE_TEMPLATE/          # 문제 제안·도움 요청·주간 운영 양식
│   ├── workflows/               # 자동 검사·주차 생성 자동화
│   └── PULL_REQUEST_TEMPLATE.md
├── docs/
│   ├── operation-guide.md       # 스터디장 운영 방법
│   ├── quick-start.md           # 초보자용 Git 사용법
│   └── review-guide.md          # 코드 리뷰 기준
├── problems/
│   └── weekXX/
│       ├── week.yml             # 주차 설정
│       ├── README.md            # 문제 목록·제출 현황
│       └── 문제폴더/
│           └── 개인폴더/        # 자유로운 파일명의 풀이
├── scripts/
│   ├── create_week.py           # 새 주차 구조 생성
│   ├── progress.py              # 제출 현황 출력
│   ├── validate_pr.py           # PR 제목·본문 검사
│   └── validate_repository.py   # 풀이 경로·문법 검사
├── templates/
├── study-config.json            # 멤버·플랫폼·파일 규칙
└── CONTRIBUTING.md
```

---

## 📝 제출 규칙

### 브랜치

```text
week01/hyeonsu
week02/daewon
```

### 파일 위치

```text
problems/week01/SWEA_1954/YeoroJeong/solution.py
problems/week01/BOJ_2178/DaewonKim/solution.py
```

### 파일 이름

개인 폴더 안의 파일명은 자유입니다. 제출 여부는 폴더 경로와 확장자로 판단합니다.

허용 플랫폼은 `SWEA`, `BOJ`, `Programmers`, `ETC`입니다.

### PR 제목

```text
[W01] 정현수 주간 풀이
[DOCS] 코드 리뷰 규칙 수정
[CHORE] 2주차 구조 생성
[FIX] 자동 검사 오류 수정
```

### 커밋

```text
[W01][SWEA-1954] 풀이 추가
[W01][BOJ-2178] 리뷰 반영
[DOCS] 1주차 문제 목록 수정
```

---

## 🤖 자동으로 확인되는 항목

PR을 생성하면 GitHub Actions가 다음을 검사합니다.

- Python 문법 오류
- `problems/weekXX/문제폴더/개인폴더/` 경로 규칙
- 등록되지 않은 이름의 폴더 사용 여부
- 허용된 풀이 파일 확장자
- 빈 파일 또는 지나치게 큰 파일
- PR 제목 형식
- PR 본문의 핵심 접근·리뷰 요청 작성 여부
- 변경 경로 기반 주차·작성자·문제·언어 자동 분석
- PR 자동 요약 댓글 생성 및 갱신

자동 검사는 **문제의 정답 여부를 판별하지 않습니다.** 제출 사이트에서 통과한 코드를 올려주세요.

로컬에서도 같은 검사를 실행할 수 있습니다.

```bash
python scripts/validate_repository.py
python scripts/progress.py
```

---

## 💬 리뷰 표기

| 표기 | 의미 |
|---|---|
| `[MUST]` | 병합 전에 반드시 수정해야 하는 오류 |
| `[SUGGEST]` | 더 나은 구현이나 가독성 제안 |
| `[QUESTION]` | 코드의 의도나 선택 이유 질문 |
| `[GOOD]` | 잘한 구현과 배울 점 |
| `[NOTE]` | 참고할 Python 문법·알고리즘 지식 |

자세한 기준은 [`docs/review-guide.md`](docs/review-guide.md)를 확인하세요.

---

## 📚 문서

- [처음 참여하는 사람용 가이드](docs/quick-start.md)
- [기여 및 제출 규칙](CONTRIBUTING.md)
- [코드 리뷰 가이드](docs/review-guide.md)
- [반복 실수 기록](docs/common-mistakes.md)
- [스터디 운영 가이드](docs/operation-guide.md)
- [문제 중심 폴더 구조 전환](PROBLEM_FIRST_MIGRATION.md)


## 주차 폴더 자동 생성

문제별 개인 제출 폴더 자동 생성 방법은 [`WEEK_FOLDER_AUTOMATION.md`](WEEK_FOLDER_AUTOMATION.md)를 참고하세요.
