# Clockwise City - B형 원복 실습 패키지

[문제 전체 설명](problem.md) | [구현 파일](user_solution.cc) | [채점 코드](main.cc) | [예시 입력](input.txt) | [예시 출력](output.txt)

## 실행 방법

```bash
g++ -std=c++17 -O2 main.cc user_solution.cc -o clockwise_city
./clockwise_city < input.txt
```

정상 구현이면 각 테스트 케이스에서 `100`점이 출력됩니다. `main.cc`는 수정하지 않고 `user_solution.cc`의 세 API를 구현합니다.

## 첫 번째 테스트 케이스: 짧은 디버깅용

첫 번째 테스트 케이스는 `init` 1회, `addBuilding` 3회, `move` 3회만 사용한다. 먼저 연결되지 않은 두 건물을 조회하고, 중간 건물을 추가한 뒤 일반 이동과 경유 이동을 차례로 확인한다.

```text
7
100 30
200 1 5 5 5 5 0 2
200 3 5 17 5 5 0 2
300 1 3 0 -1
200 2 5 11 5 5 0 2
300 1 3 0 14
300 1 3 1 2 16
```

각 `move()`가 반환해야 하는 값은 다음과 같다.

| 호출 | 확인할 내용 | 예상 반환값 |
|---|---|---:|
| `move(1, 3, 0, ...)` — 건물 2 추가 전 | 연결되지 않은 경로 | `-1` |
| `move(1, 3, 0, ...)` — 건물 2 추가 후 | 기본 최단 거리 | `14` |
| `move(1, 3, 1, {2})` | 건물 2를 실제 방문 | `16` |

마지막 명령은 건물 2 주변을 통과하는 것만으로는 부족하고, 출입구로 들어갔다가 나오는 비용 `2`가 더해지는지 확인한다. 디버깅할 때는 `move()`의 실제 반환값을 잠시 출력해 `-1 → 14 → 16` 순서인지 비교하면 된다. 전체 채점 프로그램의 표준 출력은 API별 값이 아니라 테스트 케이스별 점수인 `#1 100` 형식이다.

## 구현할 코드: `user_solution.cc`

```cpp
#include <bits/stdc++.h>
using namespace std;

void init(int N)
{
    // TODO
}

void addBuilding(int mID, int mRow, int mCol,
                 int mHeight, int mWidth,
                 int mRelRow, int mRelCol)
{
    // TODO
}

int move(int mStartID, int mEndID, int M, int mStopover[])
{
    // TODO
    return -1;
}
```

## 채점 및 실행 코드: `main.cc`

```cpp
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>

#define CMD_INIT 100
#define CMD_ADD  200
#define CMD_MOVE 300
#define MAX_STOPOVER 5

extern void init(int N);
extern void addBuilding(int mID, int mRow, int mCol,
                        int mHeight, int mWidth,
                        int mRelRow, int mRelCol);
extern int move(int mStartID, int mEndID, int M, int mStopover[]);

static bool run()
{
    int Q;
    std::scanf("%d", &Q);

    bool okay = false;

    for (int q = 0; q < Q; ++q)
    {
        int cmd;
        std::scanf("%d", &cmd);

        if (cmd == CMD_INIT)
        {
            int N;
            std::scanf("%d", &N);
            init(N);
            okay = true;
        }
        else if (cmd == CMD_ADD)
        {
            int mID, mRow, mCol, mHeight, mWidth, mRelRow, mRelCol;
            std::scanf("%d %d %d %d %d %d %d",
                       &mID, &mRow, &mCol,
                       &mHeight, &mWidth,
                       &mRelRow, &mRelCol);

            addBuilding(mID, mRow, mCol,
                        mHeight, mWidth,
                        mRelRow, mRelCol);
        }
        else if (cmd == CMD_MOVE)
        {
            int mStartID, mEndID, M;
            int mStopover[MAX_STOPOVER];
            int expected;

            std::scanf("%d %d %d", &mStartID, &mEndID, &M);

            for (int i = 0; i < M; ++i)
                std::scanf("%d", &mStopover[i]);

            std::scanf("%d", &expected);

            int ret = move(mStartID, mEndID, M, mStopover);
            if (ret != expected)
                okay = false;
        }
    }

    return okay;
}

int main()
{
    setbuf(stdout, NULL);

    // 로컬 테스트 시 필요하면 아래 주석을 해제한다.
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int T, MARK;
    std::scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; ++tc)
    {
        int score = run() ? MARK : 0;
        std::printf("#%d %d\n", tc, score);
    }

    return 0;
}
```

## 예시 입력: `input.txt`

```text
3 100
7
100 30
200 1 5 5 5 5 0 2
200 3 5 17 5 5 0 2
300 1 3 0 -1
200 2 5 11 5 5 0 2
300 1 3 0 14
300 1 3 1 2 16
12
100 30
200 21 5 5 5 5 0 2
200 22 5 11 5 5 2 0
200 23 11 5 5 5 2 4
200 24 11 11 5 5 4 2
300 21 24 0 26
300 21 24 1 22 40
300 21 24 1 23 40
300 21 24 2 22 23 54
300 24 21 0 26
300 22 23 0 14
300 23 22 0 14
11
100 30
200 31 4 4 4 6 0 3
200 33 11 11 5 4 2 0
300 31 33 0 -1
200 32 4 11 6 4 3 3
300 31 32 0 14
300 31 33 0 31
300 31 33 1 32 33
300 33 31 0 23
300 33 31 1 32 49
300 32 33 0 19
```

## 예시 출력: `output.txt`

```text
#1 100
#2 100
#3 100
```

`MOVE` 명령의 마지막 값은 채점용 expected answer입니다.
