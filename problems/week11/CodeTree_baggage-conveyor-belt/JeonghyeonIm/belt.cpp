#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;

int dc[5] = {0, 0, -1, 0, 1}; // 순서대로 0(벽), 1(하), 2(왼), 3(위), 4(오)
int dr[5] = {0, 1, 0, -1, 0};
int N;
int B;
vector<vector<int>> board; // 처리장
vector<vector<int>> bag; // 수하물정보
vector<bool> bag_in_ck; // 수하물 투입 체크
unordered_map<int, pair<int, int>> bag_loc; // 수하물 위치, key: 수하물 번호, value: 좌표(r, c) => 적재되면 삭제
unordered_map<int, int> bag_not_cnt; // 수하물 연속 이동 못한 횟수, key: 수하물 번호, value: 연속 이동 못한 횟수
unordered_map<int, bool> bag_crashed; // 수하물 파손 정보, key: 수하물 번호, value: 파손 여부

vector<int> result; // 출력용 결과
int bag_complete_cnt; // 수하물 몇개 완료 되었는지

struct Cmp {
    bool operator() (const pair<int, int>& a, const pair<int, int>& b) const {
        if (a.first != b.first) return a.first > b.first;
        return a.second < b.second;
    }
};

void stage1() {
    if (!bag_loc.empty()) { // 수하물이 하나이상 올려져 있을 경우
        map<pair<int,int>, pair<int, int>, Cmp> next; // key: {무게, 번호}, val: r, c좌표
        set<pair<int, int>> rc;
        for (auto& [key, val] : bag_loc) { // 일단 다음 투입 위치 선정
            int r = val.first;
            int c = val.second;
            int d = board[r][c];
            if (d > 4) d = 0; // 9번 칸 방어
            next[{bag[key][3], key}] = {r + dr[d], c + dc[d]};
        }

        set<pair<int, int>> notmove; // 이동 못하는 수하물이 서 있는 칸
        set<int> stuck; // 이동 못하는 수하물 번호

        // 같은 칸으로 몰린 경우 처리
        for (auto& [key, val] : next) {
            if (rc.count({val.first, val.second}) == 0) { // <= 얘만 이동
                rc.insert({val.first, val.second});
            } else { // <= 얘는 이동 안됨
                val = bag_loc[key.second];
                notmove.insert(val);
                stuck.insert(key.second);
            }
        }

        // 앞 칸이 막힌 경우 처리: 새로 막히는 수하물이 없을 때까지 반복
        bool changed = true;
        while (changed) {
            changed = false;
            for (auto& [key, val] : next) {
                if (stuck.count(key.second)) continue; // 이미 이동 못함
                if (notmove.count({val.first, val.second}) == 1) { // 막히는 경우
                    val = bag_loc[key.second];
                    notmove.insert(val);
                    stuck.insert(key.second);
                    changed = true;
                }
            }
        }

        // 이동 + 이동 못한 횟수 갱신
        for (auto& [key, val] : next) {
            int id = key.second;
            if (stuck.count(id)) { // 이동 x
                bag_not_cnt[id]++;
            } else { // 이동
                bag_not_cnt[id] = 0;
                bag_loc[id] = val;
            }
        }
    }
}
// 2단계
void stage2(int turn) {
    if (!bag_loc.empty()) { // 수하물이 하나이상 올려져 있을 경우
        // 적재
        vector<int> done;
        for (auto& [key, val] : bag_loc) {
            if (board[val.first][val.second] == 9) done.push_back(key);
        }
        for (int k : done) {
            result[k] = turn;
            bag_loc.erase(k);
            bag_complete_cnt++;
        }
        // 파손
        for (auto& [key, val] : bag_not_cnt) {
            if (val >= 3) {
                bag_crashed[key] = true;
            }
        }
    }
}
// 3단계
void stage3(int turn) {
    set<pair<int, int>> queue_bag; // 투입 예정 bag, (투입예정턴, 번호)
    for (int i = 1; i <= B; i++) { // 투입 후보 선정
        if (bag_in_ck[i] == true || bag[i][0] > turn) { // 이미 올라갔거나, 턴 아직 안됐으면 패스
            continue;
        }
        int r = bag[i][1];
        int c = bag[i][2];
        if (!bag_loc.empty()) {
            bool ck = true; // 해당 투입 칸에 이미 수하물이 있는지 확인
            for (auto& [key, val] : bag_loc) {
                if (val.first == r && val.second == c) {
                    ck = false;
                    break;
                }
            }
            if (ck) {
                queue_bag.insert({bag[i][0], i});
            }
        } else {
            queue_bag.insert({bag[i][0], i});
        }
    }
    set<pair<int, int>> rc; // 해당 턴에서 투입된 좌표끼리 비교
    for (auto& [t, id] : queue_bag) {
        int r = bag[id][1];
        int c = bag[id][2];
        if (rc.count({r, c}) == 0) {
            bag_loc[id] = {r, c};
            rc.insert({r, c});
            bag_in_ck[id] = true;
        }
    }
}

int main() {
    // Please write your code here.
    cin >> N;
    board.assign(N + 1, vector<int>(N + 1, 0));
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> board[i][j];
        }
    }
    cin >> B;
    bag.assign(B + 1, vector<int>(4, 0));
    bag_in_ck.assign(B + 1, false);
    for (int i = 1; i <= B; i++) {
        for (int j = 0; j < 4; j++) {
            cin >> bag[i][j];
        }
        bag_not_cnt[i] = 0;
        bag_crashed[i] = false;
    }

    result.assign(B + 1, 0);
    bag_complete_cnt = 0;
    // ======== 입력 완료 ========

    int turn = 1;
    while (1) {
        stage1();
        stage2(turn);
        stage3(turn);
        if (bag_complete_cnt == B) {
            break;
        }
        turn++;
    }

    for (int i = 1; i <= B; i++) {
        if (bag_crashed[i]) {
            cout << result[i] * -1 << " ";
        } else {
            cout << result[i] << " ";
        }
    }
    return 0;
}
