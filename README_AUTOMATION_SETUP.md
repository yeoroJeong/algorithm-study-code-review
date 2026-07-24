# 주차별 README 자동화 적용 가이드

## 자동화 결과

다음 변경이 `main`에 반영되면 자동화가 실행됩니다.

1. `problems/weekXX/week.yml`의 문제·멤버 정보 변경
2. 문제 폴더의 풀이 파일 추가·삭제

자동화는 각 주차의 `README.md`를 다시 만들고 다음 커밋을 남깁니다.

```text
docs: 주차 README 자동 갱신
```

커밋 작성자는 `github-actions[bot]`으로 표시됩니다.

## 1. 저장소에 복사할 파일

압축을 풀고 다음 파일을 저장소 최상위 폴더 기준으로 복사합니다.

```text
.github/workflows/update-week-readmes.yml
scripts/generate_week_readmes.py
problems/week01/week.yml
```

기존 `problems/week01/README.md`가 있어도 첫 실행 시 자동 생성 내용으로 교체됩니다. 기존 README에 보존할 내용이 있다면 `week.yml`의 `notes`로 옮겨주세요.

## 2. 제출 파일 규칙

기본 규칙은 다음과 같습니다.

```text
problems/weekXX/문제폴더/GitHub아이디.확장자
```

예시:

```text
problems/week01/SWEA_1954/yeoroJeong.py
```

위 파일이 존재하면 `yeoroJeong`의 1954번 문제 제출 상태가 `✅`로 표시됩니다.

지원 확장자는 `week.yml`의 `submission.extensions`에서 바꿀 수 있습니다.

## 3. week.yml 수정

실제 스터디원과 문제에 맞게 수정합니다.

```yaml
members:
  - id: yeoroJeong
    name: 정현수
  - id: memberA
    name: 스터디원A

problems:
  - site: SWEA
    number: "1954"
    title: 달팽이 숫자
    difficulty: D2
    category: 구현
    folder: SWEA_1954
    url: "문제 링크"
```

`id`는 제출 파일명과 일치해야 합니다.

## 4. 로컬에서 미리 실행

```bash
python -m pip install PyYAML
python scripts/generate_week_readmes.py --week week01
```

생성 파일:

```text
problems/week01/README.md
```

최신 상태인지 검사만 하려면:

```bash
python scripts/generate_week_readmes.py --week week01 --check
```

## 5. GitHub에 올리기

```bash
git switch -c chore/readme-automation
git add .
git commit -m "chore: 주차별 README 자동화 추가"
git push -u origin chore/readme-automation
```

PR 제목:

```text
[설정] 주차별 README 자동화 추가
```

리뷰 후 `main`으로 병합합니다.

## 6. Actions 쓰기 권한

자동 커밋이 403 오류로 실패하면 다음을 확인합니다.

```text
저장소 → Settings → Actions → General
→ Workflow permissions
→ Read and write permissions
→ Save
```

## 7. 브랜치 보호 사용 시

`main` 직접 push를 완전히 차단하면 Actions의 README 자동 커밋도 실패할 수 있습니다. 현재 워크플로는 간단한 자동 커밋 방식입니다.

```text
풀이 PR 병합
→ Actions 실행
→ README 자동 갱신 커밋
```

브랜치 보호 때문에 실패한다면 Ruleset에서 GitHub Actions의 우회 권한을 허용하거나, README 갱신용 PR을 자동 생성하는 방식으로 변경해야 합니다.

## 8. 다음 주차 만들기

Git Bash:

```bash
cp -r problems/week01 problems/week02
rm -rf problems/week02/SWEA_1954 problems/week02/SWEA_1959 problems/week02/SWEA_12712
```

그다음 `problems/week02/week.yml`에서 다음을 수정합니다.

```yaml
week: 2
period:
  start: 2026-08-03
  end: 2026-08-09
deadline: "2026-08-09 23:59"
```

문제 목록과 폴더명도 새 주차에 맞게 변경합니다.

## 9. 수동 실행

```text
GitHub 저장소 → Actions → Update Week READMEs → Run workflow
```

특정 주차만 실행하려면 `week01`처럼 입력합니다. 비워두면 `week.yml`이 있는 모든 주차를 갱신합니다.
