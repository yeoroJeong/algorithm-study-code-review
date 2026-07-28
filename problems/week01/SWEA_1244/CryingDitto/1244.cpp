// 1244. [S/W 문제해결 응용] 2일차 - 최대 상금
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV15Khn6AN0CFAYD
// 2자리 골라서 바꾸는 모든 경우의 수 탐색해야 할 것 같다는 느낌은 들었는데 풀지는 못했음
// 바꾸고 나서 방문 배열을 어떻게 작성할지 몰랐던 것 같음
// AI 도움 받아서 풀이 과정 이해하고 혼자서 다시 적어보았음
// 근데 다시 풀라고 하면 풀 수 있을지...? ㅎㅎ...

#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // swap
#include <cstring>   // memset

#define MAX 1000000
using namespace std;

int swapCnt;
string str;
// 교환 횟수 10회, 숫자 6자리므로 범위 최대 999999
bool visited[11][1000000];
int maxResult = 0;
// algorithm의 swap 함수 사용

void dfs(int curSwapCnt)
{
    if (curSwapCnt == swapCnt)
    {
        maxResult = max(maxResult, stoi(str));
        return;
    }

    int sNum = stoi(str);

    // 같은 횟수 교환했을 때 이미 확인한 결과라면 리턴
    if (visited[curSwapCnt][sNum]) return;

    visited[curSwapCnt][sNum] = true;

    int size = str.length();
    // 가능한 6자리 중 두 자리 뽑아서 바꾸는 과정
    // i=0, j=1일 때와 j=1, i=0일 때는 같은 경우이므로 범위를 아래처럼 적음
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            swap(str[i], str[j]);
            dfs(curSwapCnt + 1);
            swap(str[i], str[j]);
        }
    }
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;

    for (int tc = 0; tc < T; tc++)
    {
        // 숫자문자열, 교환 횟수
        cin >> str >> swapCnt;
        maxResult = 0;
        memset(visited, false, sizeof(visited));

        // memset 안 쓰면
        /*for (int i = 0; i < 11; i++)
        {
            fill(visited[i], visited[i] + MAX, false);
        }*/
        dfs(0);
        cout << "#" << tc + 1 << " " << maxResult << "\n";
    }
    return 0;
}