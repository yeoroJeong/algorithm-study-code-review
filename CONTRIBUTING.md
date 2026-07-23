# 기여 가이드

## 1. 작업 시작

```bash
git switch main
git pull origin main
git switch -c week01/이름/플랫폼-문제번호
```

예시:

```bash
git switch -c week01/hyeonsu/swea-1954
```

## 2. 풀이 파일 추가

본인 폴더에 파일을 추가합니다.

```text
solutions/week01/정현수/SWEA_1954_달팽이숫자.py
```

파일 상단에는 다음 내용을 주석으로 남기는 것을 권장합니다.

- 문제 링크
- 핵심 아이디어
- 시간 복잡도
- 공간 복잡도
- 다시 볼 포인트

## 3. 커밋

```bash
git add .
git commit -m "[W01][SWEA-1954] 정현수 풀이 추가"
git push origin week01/hyeonsu/swea-1954
```

## 4. Pull Request

PR 템플릿에 따라 다음 내용을 작성합니다.

- 문제 정보
- 풀이 아이디어
- 시간·공간 복잡도
- 리뷰받고 싶은 부분
- 복습 필요 여부

## 5. 리뷰

리뷰 의견은 다음 표현을 권장합니다.

- `MUST`: 반드시 수정이 필요한 부분
- `SUGGEST`: 개선을 제안하는 부분
- `QUESTION`: 의도를 확인하고 싶은 부분
- `GOOD`: 잘 작성된 부분
- `NOTE`: 추가로 알아두면 좋은 내용
