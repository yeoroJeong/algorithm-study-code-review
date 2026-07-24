# 강화 버전 적용 안내

## 주요 변경 사항

1. 문제별 PR 방식에서 **한 사람당 주간 PR 한 개** 방식으로 단순화했습니다.
2. `study-config.json`에서 멤버와 파일 규칙을 관리합니다.
3. `scripts/create_week.py`로 새 주차 폴더와 문서를 자동 생성합니다.
4. `scripts/validate_repository.py`가 경로, 멤버명, 파일명, Python 문법을 검사합니다.
5. `scripts/validate_pr.py`가 PR 제목과 필수 본문 섹션을 검사합니다.
6. Issue를 입력 폼 방식으로 바꿔 누락을 줄였습니다.
7. GitHub Actions에서 주차 생성 PR을 자동으로 만들 수 있습니다.
8. 초보자 가이드와 스터디장 운영 가이드를 추가했습니다.

## 적용 방법

기존 저장소에 이 ZIP의 파일을 덮어쓴 후:

```bash
git switch -c chore/repository-upgrade
git add .
git commit -m "[CHORE] 스터디 저장소 운영 기능 강화"
git push -u origin chore/repository-upgrade
```

PR 제목:

```text
[CHORE] 스터디 저장소 운영 기능 강화
```

## 최초 설정

### 멤버 GitHub 아이디

`study-config.json`에서 빈 `github` 값을 실제 아이디로 채웁니다. 현재 자동 검사는 이름을 사용하므로 비어 있어도 동작합니다.

### GitHub Actions 주차 생성 기능

자동으로 브랜치와 PR을 만들려면:

```text
Settings → Actions → General → Workflow permissions
```

에서 쓰기 권한과 PR 생성 권한을 허용합니다. 이 권한을 사용하지 않아도 나머지 자동 검사는 정상 동작합니다.

### 브랜치 보호

`docs/operation-guide.md`의 권장 설정을 참고해 `main` 브랜치에 PR과 상태 검사를 요구하세요.
