/*
음식점의 하루 수익
재료 정보, 메뉴 정보, 하루 판매 내역
음식점의 하루 총 수익 계산
메뉴 하나의 수익: 판매 가격 - 사용한 재료의 총가격
하루 총수익: 각 메뉴의 수익 * 판매 수량 합

입력
ings: string, "재료이름 재료가격"
menu: string, "메뉴이름 재료목록 판매가격"
=>한개당 얼마나 드는지 계산해보자
sell: string, "메뉴이름 판매수량"
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

int solution(vector<string> ings, vector<string> menu, vector<string> sell) {
    // 여기에 풀이를 작성하세요.
    map<char, int> ing; // key: 재료이름, value: 재료가격
    map<string, int> men; // key: 메뉴이름, value: 1개당 수익
    int sum = 0;
    for (int i = 0; i < ings.size(); i++) {
        stringstream ss(ings[i]);
        char in;
        int cost;
        ss >> in >> cost;
        ing[in] = cost;
    }
    // 재료 처리 완료
    for (int i = 0; i < menu.size(); i++) {
        stringstream ss(menu[i]);
        string name, lst;
        int cost;
        ss >> name >> lst >> cost;
        for (int j = 0; j < lst.size(); j++) {
            men[name] -= ing[lst[j]];
        }
        men[name] += cost;
    }
    // 1개당 처리 완료
    for (int i = 0; i < sell.size(); i++) {
        stringstream ss(sell[i]);
        string name;
        int cnt;
        ss >> name >> cnt;
        sum += men[name] * cnt;
    }
    return sum;
}