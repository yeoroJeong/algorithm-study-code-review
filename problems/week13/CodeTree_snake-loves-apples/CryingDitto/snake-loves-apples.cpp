#include <iostream>
#include <deque>
using namespace std;

int N, M, K; // 격자 크기, 사과 개수, 뱀의 방향 변환 횟수 (명령)

char d[1000]; // U, D, R, L
int p[1000]; // 움직임 크기

// 현재 뱀 몸이 차지한 공간 표시
int grid[10000][10000];
// 사과의 위치
int apple[10000][10000];

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };
int getDir(char d)
{
    if (d == 'U') return 0;
    if (d == 'D') return 1;
    if (d == 'L') return 2;
    return 3;
}

bool isOut(int x, int y)
{
    if (x >= N || y >= N || x < 0 || y < 0) return true;
    return false;
}

// 뱀 이동 시에 머리를 d[i] 방향으로 옮기고 꼬리 사라짐
// 이번 이동으로 사라지는 꼬리 자리면 머리가 가도 몸이 겹치지 않은 것으로 처리
// 꼬리 삭제 먼저 하기


int main() {
    cin >> N >> M >> K;

    for (int i = 0; i < M; i++)
    {
        int ax, ay;
        cin >> ax >> ay;
        apple[ax-1][ay-1] = 1;
    }

    for (int i = 0; i < K; i++) cin >> d[i] >> p[i];

    // Please write your code here.
    int time = 0;

    deque<pair<int, int>> body;
    body.push_back({ 0, 0 });
    int x = 0; int y = 0;
    for (int i = 0; i < K; i++)
    {
        int dirIdx = getDir(d[i]);
        int power = p[i];
        bool doEnd = false;
        // p만큼 움직이면서 사과 먹었는지 확인
        for (int power = 0; power < p[i]; power++)
        {
            x = body.front().first;
            y = body.front().second;
            int nx = x + dx[dirIdx];
            int ny = y + dy[dirIdx];
            if (isOut(nx, ny))
            {
                doEnd = true;
                break;
            }

            // 갈 수 있으면 꼬리 먼저 제거
            pair<int, int> tail = body.back();
            grid[tail.first][tail.second] = 0;
            body.pop_back();

            // 머리 추가
            body.push_front({ nx, ny });
            grid[nx][ny]++;
            // 머리 자리에 몸통이나 기타 없으면 통과
            if (grid[nx][ny] > 1)
            {
                doEnd = true;
                break;
            }
            // 사과이면 꼬리 추가, 사과 삭제, 꼬리 추가
            if (apple[nx][ny])
            {
                apple[nx][ny] = 0;
                body.push_back(tail);
                grid[tail.first][tail.second]++;
            }


            time++;
        }
        if (doEnd)
        {
            time++;
            break;
        }
    }

    cout << time;

    return 0;
}
