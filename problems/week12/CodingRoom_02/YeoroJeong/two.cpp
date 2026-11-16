#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <deque>
#include<set>
using namespace std;
struct Fruit {
    int idx;
    vector<int>list;
};
struct Comp {
    bool operator()(const Fruit& a, const Fruit& b)const {
        // 우선순위 순으로 나열한 점수를 비교해 처음으로 점수가 큰 과일을 앞에 둔다.
        for (int i = 0; i < a.list.size(); i++) {
            if (a.list[i] != b.list[i]) return a.list[i] > b.list[i];
        }
        // 점수가 모두 같으면 원래 번호가 작은 과일을 앞에 둔다.
        return a.idx < b.idx;
    }
};
vector<int> solution(vector<vector<int>> fruits, vector<vector<int>> priority) {
    // 여기에 풀이를 작성하세요.
    vector<int>result;
    int siz = fruits[0].size();
    set<Fruit,Comp> solv;
    for (int i = 0; i < fruits.size(); i++) {
        deque<int> dq;
        int cur_imp = 1;
        int cur_idx = 0;
        // 우선순위 1부터 차례대로 해당 열을 찾아 비교할 점수 목록을 만든다.
        while (cur_imp <= siz) {
            if (priority[i][cur_idx] == cur_imp) {
                dq.push_back(fruits[i][cur_idx]);
                cur_imp++;
                cur_idx = 0;
                continue;
            }
            cur_idx++;
        }
        vector<int>temp;
        while (!dq.empty()) {
            temp.push_back(dq.front());
            dq.pop_front();
        }
       
        solv.insert({ i + 1,temp });
        
    }
    // set에 정렬된 순서대로 과일의 원래 번호를 반환 배열에 담는다.
    for (auto temp : solv) {
        result.push_back(temp.idx);
    }

    return result;
}
