#include <string>
#include <vector>
#include <queue>

using namespace std;

struct State
{
    int redX;
    int redY;
    int blueX;
    int blueY;
    int turn;

    vector<vector<bool>> redVisit;
    vector<vector<bool>> blueVisit;
};

int solution(vector<vector<int>> maze) 
{
    int n = (int)maze.size();
    int m = (int)maze[0].size();

    int redStartX = 0;
    int redStartY = 0;
    int blueStartX = 0;
    int blueStartY = 0;

    int redEndX = 0;
    int redEndY = 0;
    int blueEndX = 0;
    int blueEndY = 0;

    for(int row = 0; row < n; ++row)
    {
        for(int col = 0; col < m; ++col)
        {
            if(maze[row][col] == 1)
            {
                redStartX = row;
                redStartY = col;
            }
            else if(maze[row][col] == 2)
            {
                blueStartX = row;
                blueStartY = col;
            }
            else if(maze[row][col] == 3)
            {
                redEndX = row;
                redEndY = col;
            }
            else if(maze[row][col] == 4)
            {
                blueEndX = row;
                blueEndY = col;
            }
        }
    }

    vector<vector<bool>> redVisit(n, vector<bool>(m, false));
    vector<vector<bool>> blueVisit(n, vector<bool>(m, false));

    redVisit[redStartX][redStartY] = true;
    blueVisit[blueStartX][blueStartY] = true;

    queue<State> q;

    q.push({
        redStartX,
        redStartY,
        blueStartX,
        blueStartY,
        0,
        redVisit,
        blueVisit
    });

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while(!q.empty())
    {
        State cur = q.front();
        q.pop();

        bool redArrived = 
            cur.redX == redEndX && cur.redY == redEndY;

        bool blueArrived = 
            cur.blueX == blueEndX && cur.blueY == blueEndY;

        if(redArrived && blueArrived)
        {
            return cur.turn;
        }

        vector<pair<int, int>> redNextList;
        vector<pair<int, int>> blueNextList;

        // 빨간 수레가 이미 도착했다면 현재 위치에 고정
        if(redArrived)
        {
            redNextList.push_back({cur.redX, cur.redY});
        }
        else
        {
            for(int dir = 0; dir < 4; ++dir)
            {
                int nextRedX = cur.redX + dx[dir];
                int nextRedY = cur.redY + dy[dir];

                if(nextRedX < 0 || nextRedX >= n ||
                   nextRedY < 0 || nextRedY >= m)
                {
                    continue;
                }

                if(maze[nextRedX][nextRedY] == 5)
                {
                    continue;
                }

                if(cur.redVisit[nextRedX][nextRedY])
                {
                    continue;
                }

                redNextList.push_back({nextRedX, nextRedY});
            }
        }

        // 파란 수레가 이미 도착했다면 현재 위치에 고정
        if(blueArrived)
        {
            blueNextList.push_back({cur.blueX, cur.blueY});
        }
        else
        {
            for(int dir = 0; dir < 4; ++dir)
            {
                int nextBlueX = cur.blueX + dx[dir];
                int nextBlueY = cur.blueY + dy[dir];

                if(nextBlueX < 0 || nextBlueX >= n ||
                   nextBlueY < 0 || nextBlueY >= m)
                {
                    continue;
                }

                if(maze[nextBlueX][nextBlueY] == 5)
                {
                    continue;
                }

                if(cur.blueVisit[nextBlueX][nextBlueY])
                {
                    continue;
                }

                blueNextList.push_back({nextBlueX, nextBlueY});
            }
        }

        for(auto redNext : redNextList)
        {
            for(auto blueNext : blueNextList)
            {
                int nextRedX = redNext.first;
                int nextRedY = redNext.second;

                int nextBlueX = blueNext.first;
                int nextBlueY = blueNext.second;

                // 두 수레가 같은 칸으로 이동하면 안 됨
                if(nextRedX == nextBlueX && nextRedY == nextBlueY)
                {
                    continue;
                }

                // 두 수레가 서로 위치를 바꾸면 안 됨
                if(nextRedX == cur.blueX && nextRedY == cur.blueY &&
                   nextBlueX == cur.redX && nextBlueY == cur.redY)
                {
                    continue;
                }

                State nextState = cur;

                nextState.redX = nextRedX;
                nextState.redY = nextRedY;
                nextState.blueX = nextBlueX;
                nextState.blueY = nextBlueY;
                nextState.turn = cur.turn + 1;

                if(!redArrived)
                {
                    nextState.redVisit[nextRedX][nextRedY] = true;
                }

                if(!blueArrived)
                {
                    nextState.blueVisit[nextBlueX][nextBlueY] = true;
                }

                q.push(nextState);
            }
        }
    }

    return 0;
}