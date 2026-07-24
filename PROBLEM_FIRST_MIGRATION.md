# 문제 중심 폴더 구조 전환

## 최종 구조

```text
problems/
└─ week01/
   ├─ week.yml
   ├─ README.md
   ├─ SWEA_1954/
   │  ├─ YeoroJeong/solution.py
   │  ├─ DaewonKim/.gitkeep
   │  ├─ CryingDitto/.gitkeep
   │  ├─ JeonghyeonIm/.gitkeep
   │  └─ ChoiSooBin/.gitkeep
   └─ SWEA_2001/
      └─ ...
```

## 기존 풀이 이동

기존 `solutions/weekXX/개인폴더/문제폴더/파일`이 남아 있다면 먼저 확인합니다.

```bash
python scripts/migrate_solution_layout.py --dry-run
```

문제가 없으면 실제로 이동합니다.

```bash
python scripts/migrate_solution_layout.py
```

이 명령은 다음처럼 옮긴 뒤 기존 `solutions` 폴더를 제거합니다.

```text
solutions/week01/YeoroJeong/SWEA_1954/solution.py
→ problems/week01/SWEA_1954/YeoroJeong/solution.py
```

## 자동 폴더 동기화

```bash
python scripts/sync_week_folders.py
python scripts/generate_week_readmes.py
python scripts/validate_repository.py
```

## Git 반영

```bash
git add -A
git commit -m "chore: 문제 중심 폴더 구조로 전환"
git push
```

PR 제목:

```text
[CHORE] 문제 중심 폴더 구조로 전환
```
