#include <string>
#include <vector>
#include <sstream>
#include <map>
using namespace std;

int solution(
    vector<string> ings,
    vector<string> menu,
    vector<string> sell
) {
    map<char, int> ingredientPrice;
    map<string, int> menuProfit;

    // 재료별 가격 저장
    for (int i = 0; i < (int)ings.size(); i++) {
        stringstream ss(ings[i]);
        char name;
        int price;

        ss >> name >> price;
        ingredientPrice[name] = price;
    }

    // 메뉴별 1개당 수익 계산
    for (int i = 0; i < (int)menu.size(); i++) {
        stringstream ss(menu[i]);
        string name, ingredients;
        int price;

        ss >> name >> ingredients >> price;

        int cost = 0;

        for (int j = 0; j < (int)ingredients.size(); j++) {
            cost += ingredientPrice[ingredients[j]];
        }

        menuProfit[name] = price - cost;
    }

    // 각 판매 기록의 수익 합산
    int answer = 0;

    for (int i = 0; i < (int)sell.size(); i++) {
        stringstream ss(sell[i]);
        string name;
        int quantity;

        ss >> name >> quantity;
        answer += menuProfit[name] * quantity;
    }

    return answer;
}