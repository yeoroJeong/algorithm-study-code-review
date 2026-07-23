# 🐍 Algorithm Study | Python

김대원 · 정현수 · 임정현 · 이지은 · 최수빈이 함께 사용하는 알고리즘 스터디 코드리뷰 저장소입니다.

## 스터디 목표

- Python으로 문제 해결 능력을 꾸준히 높입니다.
- 정답 코드뿐 아니라 접근 방식과 시간 복잡도를 설명합니다.
- Pull Request를 통해 서로의 풀이를 비교하고 리뷰합니다.
- 다시 풀어야 할 문제와 반복되는 실수를 기록합니다.

## 저장소 구조

```text
.
├── .github/
│   ├── ISSUE_TEMPLATE/
│   ├── workflows/
│   └── PULL_REQUEST_TEMPLATE.md
├── docs/
│   └── review-guide.md
├── problems/
│   └── week01/
│       └── README.md
├── solutions/
│   └── week01/
│       ├── 김대원/
│       ├── 정현수/
│       ├── 임정현/
│       ├── 이지은/
│       └── 최수빈/
├── templates/
│   ├── problem-note-template.md
│   └── solution_template.py
├── .gitignore
├── CONTRIBUTING.md
└── README.md
```

## 기본 진행 순서

1. 주차별 문제를 `problems/weekXX/README.md`에 등록합니다.
2. 각자 개인 브랜치를 생성합니다.
3. `solutions/weekXX/이름/`에 풀이 코드를 추가합니다.
4. 풀이가 끝나면 Pull Request를 생성합니다.
5. 팀원 리뷰를 반영한 뒤 `main` 브랜치에 병합합니다.
6. 복습이 필요한 문제는 PR 체크리스트와 노션에 표시합니다.

## 브랜치 이름

```text
week01/정현수/swea-1954
week02/김대원/boj-2178
```

영문 이름을 사용하고 싶다면 다음처럼 작성해도 됩니다.

```text
week01/hyeonsu/swea-1954
```

## 파일 이름

```text
SWEA_1954_달팽이숫자.py
BOJ_2178_미로탐색.py
Programmers_문제명.py
```

## 커밋 메시지

```text
[W01][SWEA-1954] 정현수 풀이 추가
[W01][SWEA-1954] 리뷰 반영 및 시간복잡도 설명 추가
[DOCS] 1주차 문제 목록 수정
```

## Pull Request 규칙

- PR 하나에는 가능하면 문제 하나만 포함합니다.
- 핵심 접근 방식과 시간 복잡도를 작성합니다.
- 막힌 부분이나 리뷰받고 싶은 부분을 구체적으로 적습니다.
- 최소 한 명 이상의 리뷰를 받은 뒤 병합합니다.
- 단순 정답 여부보다 가독성, 예외 처리, 복잡도와 대안 풀이를 확인합니다.

자세한 기준은 [`docs/review-guide.md`](docs/review-guide.md)를 확인하세요.
