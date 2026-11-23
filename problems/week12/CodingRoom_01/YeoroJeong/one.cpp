#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;
map<char, int> cost;
map<string, int>margin;
int solution(vector<string> ings, vector<string> menu, vector<string> sell) {
    // 여기에 풀이를 작성하세요.
    int result = 0;
    // 재료별 단가를 저장해 메뉴 원가를 계산할 때 조회한다.
    for (auto c : ings) {
        stringstream ss(c);
        char idx;
        string cost_str;
        ss >> idx >> cost_str;
        cost[idx] = stoi(cost_str);
    }

    // 필요한 재료의 단가를 더한 뒤 판매가에서 빼 메뉴당 이익을 계산한다.
    for (auto m : menu) {
        stringstream ss(m);
        string idx;
        string needs;
        string cost_str;
        ss >> idx >> needs >> cost_str;
        int temp_cost = 0;
        for (int i = 0; i < needs.length(); i++) {
            temp_cost += cost[needs[i]];
        }
        margin[idx] = stoi(cost_str) - temp_cost;
    }
    // 메뉴당 이익에 판매 수량을 곱해 하루 총이익에 더한다.
    for (auto s : sell) {
        stringstream ss(s);
        string idx;
        string cost_str;
        ss >> idx >> cost_str;
        result = result + (margin[idx] * (stoi(cost_str)));
    }
    return result;
}
