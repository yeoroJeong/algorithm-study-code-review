/*
뱀은 사과를 좋아해


N*N 크기 격자 안
사과들의 위치, 뱀의 움직임 주어졌을 때
게임이 끝나는데 몇 초가 걸리는지 구하기

뱀의 시작위치 = 좌측 상단
움직임:
머리를 특정 방향으로 한칸옮김
가장 끝에 있던 꼬리가 사라짐

사과(먹는 즉시 사라짐):
움직인 장소에 사과가 존재한다면
꼬리 안사라짐

1칸당 1초

종료조건:
1. K개의 명령을 모두 수행했거나
2. 움직이는 도중 격자를 벗어났거나
3. 움직이는 도중 몸이 꼬여 서로 겹쳐졌을 경우 종료

격자 크기 N
사과 개수 M
뱀의 방향 변환 횟수 K
(d, p) => d방향으로 p칸 움직일거임

grid를 그린다음에 뱀이 있는 칸은 1로 칠하자
*/

#include <iostream>
#include <vector>
#include <deque>
#include <random>
using namespace std;

int N, M, K;
int x[10000], y[10000]; // 사과
char d[1000];
int p[1000];

int main() {
    char idx[256];
    idx['U'] = 0;
    idx['D'] = 1;
    idx['R'] = 2;
    idx['L'] = 3;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    cin >> N >> M >> K;
    vector<vector<int>> grid(N, vector<int>(N, 0));

    for (int i = 0; i < M; i++) {
        cin >> x[i] >> y[i];
        x[i]--;
        y[i]--;
        grid[x[i]][y[i]] = 2; // 사과 표시
    }

    for (int i = 0; i < K; i++) cin >> d[i] >> p[i];

    // Please write your code here.
    deque<pair<int, int>> position; // 뱀 이동 좌표 기억
    position.push_back({0, 0}); // 처음 좌표
    grid[0][0] = 1;

    int rst_s = 0; // 결과 출력(초)
    int head_r = 0, head_c = 0; // 뱀 머리 위치
    int tail_r = 0, tail_c = 0; // 뱀 꼬리 위치

    bool check = false; // 종료 조건
    for (int i = 0; i < K && !check; i++) { // 명령 K만큼
        while (p[i] > 0) {
            rst_s++;
            head_r += dr[idx[d[i]]];
            head_c += dc[idx[d[i]]];
            if (head_r < 0 || head_r >= N || head_c < 0 || head_c >= N) { // 종료 조건 2
                check = true;
                break;
            }

            position.push_back({head_r, head_c}); // 뱀 이동 좌표 추가
            if (grid[head_r][head_c] == 2) { // 이동 칸에 사과
                if (grid[head_r][head_c] == 1) { // 종료 조건 3 <= 이동하려고 하는 곳에 이미 뱀
                    check = true;
                    break;
                }
                grid[head_r][head_c] = 1; // 뱀 표시
            } else { // 그냥 이동
                grid[tail_r][tail_c] = 0; // 꼬리칸 지우기
                position.pop_front();
                tail_r = position.front().first;
                tail_c = position.front().second;
                if (grid[head_r][head_c] == 1) { // 종료 조건 3 <= 이동하려고 하는 곳에 이미 뱀
                    check = true;
                    break;
                }
                grid[head_r][head_c] = 1; // 뱀 표시
            }
            p[i]--;
        }
    }

    cout << rst_s << "\n";
    return 0;
}
