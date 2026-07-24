# 주차·문제 폴더 완전 자동 생성

## 적용 후 동작

GitHub에서 다음 메뉴를 실행합니다.

```text
Actions → Create Weekly Setup → Run workflow
```

입력 예시:

```text
week: 2
start_date: 2026-08-03
end_date: 2026-08-09
deadline: 2026-08-09 23:59
```

`problems`에는 한 줄에 한 문제씩 아래 형식으로 붙여 넣습니다.

```text
쉬움|SWEA|1974|스도쿠 검증|D2|구현 / 2차원 배열|https://문제링크
쉬움|SWEA|2805|농작물 수확하기|D3|구현 / 2차원 배열|https://문제링크
중간|SWEA|1210|Ladder1|D4|구현 / 탐색|https://문제링크
어려움|SWEA|2819|격자판의 숫자 이어 붙이기|D4|DFS / 완전 탐색|https://문제링크
```

각 줄은 정확히 다음 7개 항목입니다.

```text
스터디난이도|사이트|문제번호|문제명|공식난이도|알고리즘분류|문제링크
```

액션이 자동 생성하는 결과:

```text
problems/week02/week.yml
problems/week02/README.md
problems/week02/SWEA_1974/.gitkeep

solutions/week02/YeoroJeong/SWEA_1974/.gitkeep
solutions/week02/DaewonKim/SWEA_1974/.gitkeep
solutions/week02/CryingDitto/SWEA_1974/.gitkeep
solutions/week02/JeonghyeonIm/SWEA_1974/.gitkeep
solutions/week02/ChoiSooBin/SWEA_1974/.gitkeep
```

모든 문제와 모든 스터디원의 조합으로 폴더가 생성됩니다.

## 문제를 나중에 추가한 경우

`problems/week02/week.yml`에 문제를 추가하고 `main`에 병합하면 `Update Week READMEs`가 자동으로:

1. 모든 개인 폴더 아래 새 문제 폴더 생성
2. README 문제 목록 갱신
3. 제출 현황 갱신
4. 변경사항 자동 커밋

을 수행합니다.

## 제출 방법

파일명은 자유입니다.

```text
solutions/week02/YeoroJeong/SWEA_1974/solution.py
solutions/week02/CryingDitto/SWEA_1974/my_answer.py
```

PR이 `main`에 병합되면 해당 문제 제출 상태가 자동으로 `✅`가 됩니다.

## 적용 명령

이 ZIP을 기존 저장소에 덮어쓴 뒤:

```bash
git switch -c chore/auto-problem-folders
git add .
git commit -m "[CHORE] 개인별 문제 폴더 자동 생성 추가"
git push -u origin chore/auto-problem-folders
```

PR 제목:

```text
[CHORE] 개인별 문제 폴더 자동 생성 추가
```
