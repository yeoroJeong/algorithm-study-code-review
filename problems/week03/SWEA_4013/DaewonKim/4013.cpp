/*
    - mags[i][2]  : i번 자석의 오른쪽(3시) 접촉면
    - mags[i][6]  : i번 자석의 왼쪽(9시) 접촉면
    - 인접한 두 자석의 접촉면 극이 서로 다르면(0,1 또는 1,0) 이웃 자석도 반대 방향으로 회전
      (원본 코드 상단 주석: "1의 자석이 시계방향 - 2의 자석이 반시계방향" 규칙을 그대로 따름)
    - 접촉면 극이 같으면 이웃 자석은 회전하지 않음
*/

#include <iostream>
#include <vector>
using namespace std;

const int RIGHT_CONTACT = 2; // 3시 방향
const int LEFT_CONTACT  = 6; // 9시 방향

// 시계방향 회전: index i의 값이 i+1 로 이동 
void clockWise(vector<int>& gear)
{
    vector<int> tmp(8);
    for (int i = 0; i < 8; ++i)
        tmp[(i + 1) % 8] = gear[i];
    gear = tmp;
}

// 반시계방향 회전: index i의 값이 i-1 로 이동
void counterClockWise(vector<int>& gear)
{
    vector<int> tmp(8);
    for (int i = 0; i < 8; ++i)
        tmp[(i - 1 + 8) % 8] = gear[i];
    gear = tmp;
}

/*
    idx번 자석이 dir 방향으로 돈다는 것이 "확정"되었을 때 호출.
    - 이 자석의 회전을 spinDir[idx]에 기록
    - 아직 확인하지 않은 왼쪽/오른쪽 이웃을 검사해서,
      필요하면 이웃에 대해 다시 자기 자신(decideSpin)을 호출 -> 재귀로 전파
*/
void decideSpin(const vector<vector<int>>& mags,
                 vector<int>& spinDir,
                 vector<int>& checked,
                 int idx, int dir)
{
    checked[idx]  = 1;
    spinDir[idx]  = dir;

    // 왼쪽 이웃(idx-1) 확인
    if (idx - 1 >= 0 && !checked[idx - 1])
    {
        checked[idx - 1] = 1; // 이번 판단으로 결과가 확정되므로 먼저 체크 처리
        if (mags[idx][LEFT_CONTACT] != mags[idx - 1][RIGHT_CONTACT])
            decideSpin(mags, spinDir, checked, idx - 1, -dir);
    }

    // 오른쪽 이웃(idx+1) 확인
    if (idx + 1 <= 3 && !checked[idx + 1])
    {
        checked[idx + 1] = 1;
        if (mags[idx][RIGHT_CONTACT] != mags[idx + 1][LEFT_CONTACT])
            decideSpin(mags, spinDir, checked, idx + 1, -dir);
    }
}

int main()
{
    int T;
    cin >> T;

    for (int tc = 1; tc <= T; ++tc)
    {
        int numSpin;
        cin >> numSpin; // 원본과 동일하게: 회전 횟수를 먼저 입력받음

        vector<vector<int>> mags(4, vector<int>(8));
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 8; ++j)
                cin >> mags[i][j];

        for (int q = 0; q < numSpin; ++q)
        {
            int magNo, dir;
            cin >> magNo >> dir;
            magNo -= 1; // 0-based index로 변환

            // 회전 지시 한 번마다 새로 초기화 (원본 코드의 버그였던 부분)
            vector<int> spinDir(4, 0);
            vector<int> checked(4, 0);

            decideSpin(mags, spinDir, checked, magNo, dir);

            for (int i = 0; i < 4; ++i)
            {
                if (spinDir[i] == 1)
                    clockWise(mags[i]);
                else if (spinDir[i] == -1)
                    counterClockWise(mags[i]);
            }
        }

        // 점수 계산: index 0(화살표)이 s극(1)이면 해당 weight 획득 
        long long score = 0;
        int weight[4] = {1, 2, 4, 8};
        for (int i = 0; i < 4; ++i)
            if (mags[i][0] == 1)
                score += weight[i];

        cout << "#" << tc << " " << score << '\n';
    }

    return 0;
}