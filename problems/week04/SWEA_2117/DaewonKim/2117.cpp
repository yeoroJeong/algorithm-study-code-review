// 집 좌표 + 거리별 누적 O(N⁴)
// 거리: 맨해튼 거리로 사용
#include <bits/stdc++.h>

using namespace std;

int main()
{
    int T;
    cin >> T;

    for(int test_case = 1; test_case <= T; ++test_case)
    {
        int n, m;
        cin >> n >> m;

        // 집의 위치만 저장
        vector<pair<int, int>> houses;

        for(int row = 0; row < n; ++row)
        {
            for(int col = 0; col < n; ++col)
            {
                int value;
                cin >> value;

                if(value == 1)
                {
                    houses.push_back({row, col});
                }
            }
        }

        int maxTownCnt = 0;

        // 모든 중심점 검사
        for(int centerRow = 0; centerRow < n; ++centerRow)
        {
            for(int centerCol = 0; centerCol < n; ++centerCol)
            {
                /*
                    distCnt[d] = 현재 중심에서 정확히 거리 d에 있는 집의 개수
                    맵에서 가능한 최대 맨해튼 거리는 -> 2*(n-1)
                */
                vector<int> distCnt(2 * n, 0);

                // 모든 집과 현재 중심 사이의 거리 계산
                for(auto &house : houses)
                {
                    int houseRow = house.first;
                    int houseCol = house.second;

                    int dist = abs(houseRow - centerRow) + abs(houseCol - centerCol);
                    distCnt[dist]++;
                }

                int townCnt = 0;

                // K를 점점 증가시킴
                for(int k = 1; k <= n + 1; ++k)
                {
                    /*
                        K의 서비스 범위:
                        거리 <= K-1
                        이전 K까지의 집은 이미 townCnt에 들어있으므로
                        새롭게 거리 K-1에 있는 집만 추가
                    */
                    townCnt += distCnt[k - 1];

                    int fee = k * k + (k - 1) * (k - 1);

                    // 손해를 보지 않는 경우
                    if(townCnt * m >= fee)
                    {
                        maxTownCnt = max(maxTownCnt, townCnt);
                    }
                }
            }
        }

        cout << "#" << test_case << " " << maxTownCnt << "\n";
    }

    return 0;
}