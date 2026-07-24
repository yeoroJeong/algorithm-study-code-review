# README 제출 현황 자동화

## 제출 구조

```text
solutions/week01/YeoroJeong/SWEA_1954/solution.py
solutions/week01/CryingDitto/SWEA_1954/main.py
```

파일명은 자유입니다. 자동화는 `개인폴더/문제폴더` 안에 Python 파일이 하나라도 있는지 확인합니다.

## 자동 갱신

풀이 PR이 `main`에 병합되면 `Update Week READMEs`가 실행되어 `problems/week01/README.md`의 제출 현황을 갱신합니다.

## 필요한 GitHub 설정

```text
Settings → Actions → General → Workflow permissions → Read and write permissions
```

## 로컬 확인

```bash
python -m pip install PyYAML
python scripts/generate_week_readmes.py --week week01
python scripts/validate_repository.py
python scripts/progress.py
```
