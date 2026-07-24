# 스터디 운영 가이드

## 매주 해야 할 일

### 1. 새 주차 생성

로컬 명령:

```bash
python scripts/create_week.py 2 --start 2026-07-27 --end 2026-08-02
```

또는 GitHub:

```text
Actions → Create Weekly Setup → Run workflow
```

자동 워크플로를 사용하려면 저장소의 다음 옵션을 허용해야 할 수 있습니다.

```text
Settings → Actions → General
→ Workflow permissions
→ Read and write permissions
→ Allow GitHub Actions to create and approve pull requests
```

자동 PR을 허용하고 싶지 않다면 로컬 명령만 사용하면 됩니다.

### 2. 문제 문서 작성

`problems/weekXX/README.md`에서 다음을 채웁니다.

- 진행 기간과 마감
- 문제 링크와 난이도
- 알고리즘 분류
- 발표자 또는 리뷰 담당

### 3. 주간 Issue 생성

Issues → New issue → `주간 문제 및 일정` 양식을 선택합니다.

Issue에 결정 사항과 변경 이력을 남기고, 각 PR에서 해당 Issue 번호를 연결합니다.

### 4. 제출 확인

```bash
python scripts/progress.py
```

출력된 표를 주차 README 또는 공지에 붙여넣을 수 있습니다.

## 저장소 권장 설정

### Pull Request

```text
Settings → General → Pull Requests
```

- Allow squash merging: 켜기
- Automatically delete head branches: 켜기

### Branch ruleset

```text
Settings → Rules → Rulesets → New branch ruleset
```

`main`에 다음 규칙을 권장합니다.

- Require a pull request before merging
- Require approvals: 1
- Require status checks to pass
  - Validate solutions
  - Validate PR title and body
- Block force pushes
- Block deletions

처음 GitHub를 사용하는 멤버가 많다면 승인 리뷰 1개와 상태 검사만 먼저 적용하고 점차 강화합니다.

## 운영 기준

- 한 사람당 한 주에 PR 하나
- 문제 하나당 커밋 하나
- PR당 리뷰 최소 한 개
- `[MUST]` 해결 후 병합
- 미제출자 공개 압박보다 진행 중 문제를 먼저 확인
- 스터디 종료 후 주차 README에 회고 작성

## 반복되는 실수 관리

개인별로 다음 내용을 기록하도록 권장합니다.

- 경계 조건 실수
- 행·열 인덱스 혼동
- 시간 복잡도 오판
- 복사와 원본 변경 혼동
- BFS에서 `list.pop(0)` 사용
- 백트래킹 복구 누락

PR의 복습 기록에 남기고, 반복되면 `docs/common-mistakes.md`에 공통 문서로 정리합니다.
