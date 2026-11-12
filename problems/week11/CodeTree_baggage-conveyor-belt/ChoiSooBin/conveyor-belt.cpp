#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

struct Item {
    int id;
    int time;
    pair<int, int> pos;
    int weight;
};

bool comp(const Item& a, const Item& b) {
    return a.time < b.time;
}

int conv[10][10];
int n, b;
vector<Item> bag;

int dy[5] = {0, 1, 0, -1, 0};
int dx[5] = {0, 0, -1, 0, 1};

// 아래 배열은 수하물 번호를 기준으로 사용
int board[10][10]; // 각 칸의 수하물 번호, 0이면 빈칸
int state[31]; // 0: 투입 전, 1: 벨트 위, 2: 적재 완료
int stopCnt[31]; // 연속으로 이동하지 못한 횟수
bool broken[31]; // 파손 여부
int answer[31]; // 최종 적재 턴
int loadedCnt; // 적재된 수하물 수

void move(int num) {
    int winner[10][10] = {}; // 해당 칸으로 이동할 우선권을 가진 번호
    int bestWeight[10][10] = {}; // 우선권을 가진 수하물의 무게
    pair<int, int> nextPos[31];
    bool canMove[31] = {};

    // 1. 목적지를 구하고, 같은 칸을 노리는 수하물 중 승자 선택
    for (int i = 1; i <= b; i++) {
        int id = bag[i].id;
        if (state[id] != 1) continue;

        int y = bag[i].pos.first;
        int x = bag[i].pos.second;
        int dir = conv[y][x];

        int ny = y + dy[dir];
        int nx = x + dx[dir];

        nextPos[id] = {ny, nx};

        if (winner[ny][nx] == 0 ||
            bag[i].weight > bestWeight[ny][nx] ||
            (bag[i].weight == bestWeight[ny][nx] &&
                id < winner[ny][nx])) {

                    winner[ny][nx] = id;
                    bestWeight[ny][nx] = bag[i].weight;
                }
    }

    // 2. 같은 칸으로 이동하려는 경쟁에서 이긴 수하물만 이동 후보
    for (int i = 1; i <= b; i++) {
        int id = bag[i].id;
        if (state[id] != 1) continue;

        int ny = nextPos[id].first;
        int nx = nextPos[id].second;

        canMove[id] = (winner[ny][nx] == id);
    }

    // 3. 앞 수하물이 멈추면 뒤 수하물도 멈추도록 반복 확인
    for (int repeat = 0; repeat < b; repeat++) {
        bool changed = false;

        for (int i = 1; i <= b; i++) {
            int id = bag[i].id;
            if (state[id] != 1 || !canMove[id]) continue;

            int ny = nextPos[id].first;
            int nx = nextPos[id].second;
            int front = board[ny][nx];

            if (front != 0 && !canMove[front]) {
                canMove[id] = false;
                changed = true;
            }
        }

        if (!changed) break;
    }

    // 이동 결과를 기록하기 위해 점유 배열 초기화
    for (int y = 1; y <= n; y++) {
        for (int x = 1; x <= n; x++) {
            board[y][x] = 0;
        }
    }

    // 4. 이동 결과 반영 및 적재·파손 처리
    for (int i = 1; i <= b; i++) {
        int id = bag[i].id;
        if (state[id] != 1) continue;

        if (canMove[id]) {
            bag[i].pos = nextPos[id];
            stopCnt[id] = 0;
        }
        else {
            stopCnt[id]++;
        }

        int y = bag[i].pos.first;
        int x = bag[i].pos.second;

        if (conv[y][x] == 9) {
            answer[id] = broken[id] ? -num : num;
            state[id] = 2;
            loadedCnt++;
        }
        else {
            if (stopCnt[id] >= 3) {
                broken[id] = true;
            }

            board[y][x] = id;
        }
    }
}

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> conv[i][j];
        }
    }

    cin >> b;

    // 1번부터 b번까지 접근할 공간 확보
    bag.resize(b + 1);

    for (int i = 1; i <= b; i++) {
        int t, r, c, w; // 투입 예정 턴, row, column, weight
        bag[i].id = i;
        cin >> bag[i].time >> bag[i].pos.first
            >> bag[i].pos.second >> bag[i].weight;
    }

    // 적재 순서에 따라서 bag 정렬
    // 실제 정렬 기준은 투입 예정 턴
    sort(bag.begin(), bag.end(), comp);

    for (int turn = 1; loadedCnt < b; turn++) {
        // 1단계 이동 + 2단계 적재 및 파손
        move(turn);

        // 3단계: 투입 예정 턴이 지난 수하물 투입
        for (int i = 1; i <= b; i++) {
            int id = bag[i].id;

            if (state[id] != 0 || bag[i].time > turn) continue;

            int y = bag[i].pos.first;
            int x = bag[i].pos.second;

            if (board[y][x] != 0) continue;

            board[y][x] = id;
            state[id] = 1;
        }
    }

    for (int i = 1; i <= b; i++) {
        cout << answer[i] << ' ';
    }
    cout << '\n';

    return 0;
}
