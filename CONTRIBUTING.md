# 기여 가이드

## 1. 작업 시작

한 사람은 한 주 동안 하나의 브랜치를 사용합니다.

```bash
git switch main
git pull origin main
git switch -c week01/github-id
```

예시:

```bash
git switch -c week01/yeoroJeong
```

이미 브랜치를 만들었다면 새로 만들지 말고 이동합니다.

```bash
git switch week01/yeoroJeong
```

## 2. 풀이 파일 추가

반드시 본인의 주차별 폴더에 추가합니다.

```text
solutions/week01/YeoroJeong/SWEA_1954/solution.py
```

개인 폴더 아래에 문제 폴더를 만들고, 그 안에는 자유로운 파일명을 사용합니다.

```text
solutions/weekXX/개인폴더/문제폴더/자유파일명.py
```

## 3. 코드 설명

풀이 파일 상단에는 최소한 다음 내용을 남깁니다.

```python
"""
문제: SWEA 1954 달팽이 숫자
링크: https://...
작성자: 정현수
알고리즘: 구현, 시뮬레이션
핵심 아이디어: 방향 배열을 순환하며 다음 칸이 막히면 회전한다.
시간 복잡도: O(N^2)
공간 복잡도: O(N^2)
다시 볼 포인트: 방향 전환 조건과 배열 범위 검사
"""
```

설명이 길다면 [`templates/problem-note-template.md`](templates/problem-note-template.md)를 복사해 별도 Markdown 파일로 작성해도 됩니다.

## 4. 로컬 검사

Push 전에 실행하는 것을 권장합니다.

```bash
python scripts/validate_repository.py
```

현재 제출 현황은 다음 명령으로 확인할 수 있습니다.

```bash
python scripts/progress.py
```

## 5. 커밋

문제 하나당 커밋 하나를 권장합니다.

```bash
git add solutions/week01/YeoroJeong/SWEA_1954/solution.py
git commit -m "[W01][SWEA-1954] 풀이 추가"
```

리뷰를 반영할 때:

```bash
git commit -m "[W01][SWEA-1954] 리뷰 반영"
```

## 6. Push 및 PR

```bash
git push -u origin week01/yeoroJeong
```

PR 제목 예시:

```text
[W01] 정현수 주간 풀이
```

PR에는 해당 주에 푼 문제를 모두 적고, 리뷰를 원하는 부분을 한 가지 이상 구체적으로 작성합니다.

## 7. 리뷰

- 각 PR은 최소 한 명의 리뷰를 받습니다.
- 리뷰어는 좋은 점을 최소 한 가지 남깁니다.
- `[MUST]`가 남아 있으면 수정 후 병합합니다.
- 취향 차이만 있는 경우 강제 수정을 요구하지 않습니다.
- 코드 전체를 대신 작성하기보다 문제를 찾을 수 있는 질문과 근거를 남깁니다.

## 8. 병합

리뷰 반영 후 **Squash and merge**를 권장합니다.

병합이 끝나면 개인 브랜치를 삭제하고 다음 주에는 최신 `main`에서 새 브랜치를 생성합니다.

```bash
git switch main
git pull origin main
git branch -d week01/yeoroJeong
```

## 9. 충돌이 발생했을 때

```bash
git switch main
git pull origin main
git switch week01/yeoroJeong
git merge main
```

충돌 파일을 수정한 후:

```bash
git add .
git commit -m "[W01] main 병합 충돌 해결"
git push
```
