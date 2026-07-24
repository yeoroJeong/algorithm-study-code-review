# 처음 참여하는 사람용 빠른 시작

## 1. 저장소 내려받기

처음 한 번만 실행합니다.

```bash
git clone https://github.com/yeoroJeong/algorithm-study-code-review.git
cd algorithm-study-code-review
```

이미 내려받았다면 저장소 폴더에서 시작합니다.

## 2. 최신 내용 받기

```bash
git switch main
git pull origin main
```

항상 새 작업을 시작하기 전에 실행합니다.

## 3. 개인 브랜치 생성

```bash
git switch -c week01/github-id
```

예시:

```bash
git switch -c week01/yeoroJeong
```

`fatal: a branch named ... already exists`가 나오면 이미 만든 브랜치이므로 다음처럼 이동합니다.

```bash
git switch week01/yeoroJeong
```

## 4. 본인 폴더에 풀이 추가

```text
solutions/week01/정현수/SWEA_1954_달팽이숫자.py
```

다른 사람 폴더에는 파일을 추가하지 않습니다.

## 5. 변경 상태 확인

```bash
git status
```

로컬 자동 검사:

```bash
python scripts/validate_repository.py
```

## 6. 저장하고 GitHub로 올리기

```bash
git add .
git commit -m "[W01][SWEA-1954] 풀이 추가"
git push -u origin week01/yeoroJeong
```

같은 브랜치에서 두 번째부터는 `git push`만 실행해도 됩니다.

## 7. Pull Request 생성

GitHub 저장소에서 표시되는 **Compare & pull request** 버튼을 누릅니다.

제목:

```text
[W01] 정현수 주간 풀이
```

본문은 자동으로 나타나는 템플릿을 채웁니다.

## 8. 리뷰 반영

리뷰 내용을 수정한 뒤:

```bash
git add .
git commit -m "[W01][SWEA-1954] 리뷰 반영"
git push
```

기존 PR에 자동으로 추가됩니다. PR을 새로 만들 필요가 없습니다.

## 자주 생기는 오류

### main에 직접 작업했을 때

아직 커밋하지 않았다면:

```bash
git switch -c week01/github-id
```

현재 변경 내용이 새 브랜치로 그대로 이동합니다.

### Push가 거부될 때

```bash
git pull --rebase origin 현재브랜치명
git push
```

### 작업 내용이 사라질까 걱정될 때

```bash
git status
git stash
git switch main
git pull origin main
git switch 원래브랜치
git stash pop
```
