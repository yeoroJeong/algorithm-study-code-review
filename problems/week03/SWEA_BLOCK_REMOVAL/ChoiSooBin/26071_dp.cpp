#include<algorithm>
#include<iostream>
#include<climits>


using namespace std;

int n;
int mx;
int block[10];
int dp[1024];

int dfs(int state) {
    if (state == ((1 << n) - 1)) return 0;
    if (dp[state] != -1) {
        return dp[state];    // 이미 계산한 적 있는 값? -> memoization 반환
    }
    int tmp = 0; 
    for (int i = 0; i < n; i++) {
        if (state & (1 << i))continue;    // 만약 i번째 bit가 1? 이미깨진 블록
        // 점수 계산하기
        int left = -1;
        int right = -1;
        for (int j = i - 1; j >= 0; j--) {
            if (!(state & (1 << j))) {
                left = block[j];
                break;
            }
        }
        for (int j = i + 1; j < n; j++) {
            if (!(state & (1 << j))) {
                right = block[j];
                break;
            }
        }

        int score = 0;
        if (left > -1 && right > -1) score = left * right;
        else if (left > -1 && right == -1) score = left;
        else if (left == -1 && right > -1) score = right;
        else score = block[i];

        int next = state | (1 << i);    // 비트마스킹 업데이트
        tmp = max(tmp, score + dfs(next));
    }
    return dp[state] = tmp;
}


int main(int argc, char** argv)
{
    int test_case;
    int T;

    freopen("26071_input.txt", "r", stdin);
    cin >> T;

    for (test_case = 1; test_case <= T; ++test_case)
    {
        mx = INT_MIN;
        cin >> n;

        fill(dp, dp + 1024, -1);
        fill(block, block + 10, false); // 0~9
        for (int i = 0; i < n; i++) {
            cin >> block[i];
        }
        int ans = dfs(0);

        cout << '#' << test_case << ' ' << ans << '\n';
    }
    return 0;
}