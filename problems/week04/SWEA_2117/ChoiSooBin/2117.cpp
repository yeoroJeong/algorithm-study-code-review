/*
일단 완탐은 너무 시간이 오래걸릴거 같아서 누적합으로 풀어보려고 했는데 아무래도 마름모꼴을
구현하려다 보니 코드가 너무 더러워져서 저도 디버깅 안하면 못 알아보는 코드가 되어버렸습니다...
코드를 보시기보다는 아이디어와 풀이 과정 읽어보시고 방식 위주로 리뷰해주셨으면 합니다.

SWEA 실행시간 및 메모리
언어        C++
메모리      5,836kb
실행시간    29ms
결과        Pass

문제: 2117. [모의 SW 역량테스트] 홈 방범 서비스
링크: https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5VwAr6APYDFAWu
작성자: 최수빈(sbyy77dev)
알고리즘: 완전 탐색, 누적합

[문제 요약]
N*N 크기의 도시에서 마름모 모양의 홈 방범 서비스 영역을 정하는 문제다.
서비스 영역의 크기가 K일 때 운영 비용은 K*K + (K-1)*(K-1)이고,
서비스를 받는 집들은 각각 M의 비용을 지불한다.
서비스 영역이 도시 밖으로 벗어나도 운영 비용은 동일하며,
손해를 보지 않는 경우 중 가장 많은 집에 서비스를 제공할 수 있는 집의 수를 구해야 한다.

[핵심 아이디어]
서비스 영역의 크기 K와 중심 좌표를 모두 완탐(완전 탐색)한다.
각 행의 집 정보를 누적합(prefix)으로 저장해두고, 하나의 중심점과 K가 정해지면
마름모에 포함되는 각 행의 [left, right] 구간에 존재하는 집의 수를 O(1)에 계산한다.

마름모는 중심 행에서 가장 넓고, 중심에서 한 행씩 멀어질수록 좌우 범위가 1칸씩 줄어든다.
현재 행과 중심 행 사이의 거리를 dist라고 하면 좌우로 확장되는 범위는 K-1-dist가 된다.
이를 이용해 마름모 내부의 집의 수를 구하고, 집의 수*M이 운영 비용 이상인 경우 중
가장 많은 집을 포함하는 경우를 찾는다.

[풀이 과정]
1. 도시 정보를 입력받으면서 각 행별로 집의 개수를 누적한 prefix 배열을 만든다.
2. 서비스 영역의 크기 K를 N+1부터 1까지 감소시키며 탐색한다.
3. 현재 K의 운영 비용을 2*K*K - 2*K + 1로 계산한다.
4. 도시의 모든 좌표 (i, j)를 서비스 영역의 중심으로 두고 완전 탐색한다.
5. 중심 행 i를 기준으로 서비스 영역에 포함되는 가장 위쪽 행(up)과 아래쪽 행(down)을 구한다.
6. 중심 행부터 위쪽과 아래쪽을 각각 순회하며 현재 행과 중심 사이의 거리에 따라 left, right를 계산한다.
7. 각 행의 [left, right]에 존재하는 집의 수를 prefix[row][right] - prefix[row][left-1]로 O(1)에 구해 tmp에 누적한다.
   단, left가 0인 경우에는 prefix[row][right] 자체가 해당 구간의 집의 수가 된다.
8. 같은 K에서 모든 중심점을 확인하면서 서비스 영역 안에 포함할 수 있는 최대 집의 수를 mx[k]에 저장한다.
9. mx[k]*M이 현재 K의 운영 비용 이상이라면 손해가 발생하지 않으므로 ans에 저장하고 탐색을 종료한다.
10. 최종적으로 손해를 보지 않으면서 서비스를 제공할 수 있는 최대 집의 수 ans를 출력한다.

[복잡도]
- 시간 복잡도: 서비스 영역의 크기 K를 최대 N+1개 탐색하고, 각 K마다 N*N개의 중심점을 확인한다.
  각 중심점에서는 마름모에 포함되는 최대 O(N)개의 행을 순회하며, 한 행에 포함된 집의 수는 누적합을 이용해 O(1)에 계산한다.
  따라서 전체 시간 복잡도는 O(N^4)이다. N은 최대 20이므로 최대 연산량이 크지 않아 제한 시간 3초 안에 충분히 통과한다.

- 공간 복잡도: 도시 정보를 저장하는 city 배열과 각 행의 누적합을 저장하는 prefix 배열이 각각 N*N 크기이고,
  K별 최대 집의 수를 저장하는 mx 배열이 O(N) 크기이다.
  따라서 전체 공간 복잡도는 O(N^2)이며, N이 최대 20이므로 메모리 사용량도 충분히 작다.

[막혔던 부분과 오답 원인 및 해결 방법]
- 첫 번째 삽질 (서비스 영역 K의 범위를 잘못 해석):
  처음에는 K를 중심에서 K칸까지 뻗는 마름모의 반지름처럼 생각해 up, down, left, right 범위를 계산했다.
  하지만 K=1일 때 중심 한 칸만 포함되므로 실제로 중심에서 서비스 영역의 끝까지의 거리는 K가 아니라 K-1이다.
  이 때문에 전체 마름모의 범위를 한 칸씩 크게 계산하는 문제가 발생했다.
  -> 중심으로부터 현재 행까지의 거리(dist)를 기준으로 해당 행의 좌우 확장 범위를 K-1-dist로 계산하도록 수정했다.

- 두 번째 삽질 (마름모의 각 행별 left, right 범위 계산 오류):
  마름모를 위쪽과 아래쪽으로 나누어 계산하면서 row가 중심에서 멀어질수록 left와 right가 어떻게 변하는지 부호를 잘못 설정했다.
  특히 중심에서 멀어질수록 좌우 범위가 좁아져야 하는데, 일부 식에서는 오히려 right가 더 멀어지는 문제가 있었다.
  -> 중심과 현재 row의 거리만큼 좌우 범위가 줄어든다는 규칙을 기준으로 식을 다시 정리했다.
     위쪽과 아래쪽 모두 현재 행의 좌우 확장 범위가 K-1-dist가 되도록 left와 right를 계산했다.

- 세 번째 삽질 (K의 최대 범위 설정):
  처음에는 K=N이면 N*N 도시 전체를 항상 덮을 수 있다고 생각해 K를 N부터 탐색했다.
  하지만 N이 짝수인 경우에는 K=N만으로 모든 칸을 포함할 수 있는 중심점이 존재하지 않을 수 있다.
  -> 도시 전체를 포함할 수 있는 경우까지 검사하기 위해 K=N+1부터 탐색하도록 수정했다.

- 네 번째 삽질 (누적합 구간의 경계 처리):
  각 행의 [left, right] 구간에 존재하는 집의 수를 구할 때 누적합의 left 경계 처리에서 혼동이 있었다.
  prefix[row][x]는 0번 열부터 x번 열까지의 합이므로 [left, right]의 합을 구하려면
  prefix[row][right] - prefix[row][left-1]로 계산해야 한다.
  -> left가 0인 경우에는 prefix[row][right]만 사용하고,
     left가 0보다 큰 경우에만 prefix[row][left-1]을 빼도록 처리했다.
*/

#include<iostream>
#include<algorithm>
#include<climits>
#include<cmath>
 
using namespace std;
typedef long long ll;
 
/*
초기 아이디어: 
우선
운영 비용 (k - 1) * (k - 1) + k ^ 2 = 2 * k ^ 2 - 2 * k + 1
구역을 전부 포함하는 k = n
m * x - (2 * k ^ 2 - 2 * k + 1) > 0인 각 k에 대한 x의 최솟값을 구하자 만약 집 개수가 그보다 작으면 검사 x -> 가지치기
*/
 
int city[21][21];
int prefix[21][21];
int mx[22];
 
int main(int argc, char** argv)
{
    int test_case;
    int T;
     
    cin >> T;
     
    for (test_case = 1; test_case <= T; ++test_case)
    {
        fill(city[0], city[0] + 21 * 21, 0);
        fill(prefix[0], prefix[0] + 21 * 21, 0);
        fill(mx, mx + 22, 0);
 
        int ans = 0;    // 서비스를 제공받는 집들의 수
        int n, m, cost;
        int house_cnt = 0;
        // 입력
        cin >> n >> m;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> city[i][j];
                if(city[i][j]) house_cnt += 1;
                if(j) prefix[i][j] = prefix[i][j - 1] + city[i][j];
                else prefix[i][j] = city[i][j];
            }
            //cout << prefix[i][n - 1] << '\n';
        }
 
        for (int k = n + 1; k > 0; k--) {    // k 크기 정하기
            cost = k * k * 2 - 2 * k + 1;
            int mxijk = 0;
            for (int i = 0; i < n; i++) {            // 중심점 완탐(가지치기 o)
                int up = max(i - k + 1, 0); // i - k가 0보다 작으면 0, 0보다 크면 값 대입
                int down = min(i + k - 1, n-1); // i + k가 0보다 작으면 
                for (int j = 0; j < n; j++) {    
                    int tmp = 0;
                    for (int row = i; row >= up; row--) {
                        int left = max(j - k - row + i + 1, 0);
                        int right = min(j + k - 1 - i + row, n - 1);
                        tmp += prefix[row][right];
                        if (left) {     // left = 0이면 
                            tmp -= prefix[row][left - 1];          
                        }
                         
                    }
                    for (int row = i + 1; row <= down; row++) {
                        int left = max(j - k + row - i + 1, 0);
                        int right = min(j + k - row + i - 1, n - 1);    // dist = row - i
                        tmp += prefix[row][right];
                        if (left) {     // left = 0이면 
                            tmp -= prefix[row][left - 1];
                        }
                    }
                    mx[k] = max(tmp, mx[k]);
                    // 중심점 끝남
                }
            }
            if (mx[k] * m >= cost) {
                ans = mx[k];
                break;
            }
        }
         
        cout << '#' << test_case << ' ' << ans << '\n';
    }
    return 0;
}