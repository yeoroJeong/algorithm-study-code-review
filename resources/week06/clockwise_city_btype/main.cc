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
