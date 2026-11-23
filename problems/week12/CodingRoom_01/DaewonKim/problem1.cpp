#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

using namespace std;

int solution(vector<string> ings, vector<string> menu, vector<string> sell) {

    int totalPrice = 0;

    // 재료이름, 원가
    map<char, int> ingredients;

    // 메뉴 이름, 들어가는 재료들
    map<string, string> menuIngre;

    // 메뉴, 메뉴 가격
    map<string, int> menuPrices;

    // 메뉴, 갯수
    map<string, int> numMenu;

    for (int i = 0; i < (int)ings.size(); ++i)
    {
        stringstream ss(ings[i]);
        char in;
        string price;
        ss >> in >> price;
        ingredients[in] = stoi(price);
    }

    for (int i = 0; i < (int)sell.size(); ++i)
    {
        stringstream ss(sell[i]);
        string menuName;
        string num;
        ss >> menuName >> num;
        numMenu[menuName] += stoi(num);
    }

    for (int i = 0; i < (int)menu.size(); ++i)
    {
        stringstream ss(menu[i]);
        string menuName;
        string menuIngredients;
        string menuPrice;

        ss >> menuName >> menuIngredients >> menuPrice;

        menuIngre[menuName] = menuIngredients;
        menuPrices[menuName] = stoi(menuPrice);

        int curMenuIngrePrice = 0;
        // 재료 원가의 총합
        for (auto& c : menuIngredients)
        {
            curMenuIngrePrice += ingredients[c];
        }

        totalPrice += (stoi(menuPrice) - curMenuIngrePrice) * numMenu[menuName];
    }


    // 하루 총 수익을 정수로 반환
    return totalPrice;
}


// 아래는 예제 실행 코드입니다. solution 함수만 수정하세요.
namespace study_runner {
void output(int value) { cout << value; }
void output(const vector<int>& values) {
    cout << "[";
    for (int i = 0; i < (int)values.size(); i++) {
        if (i) cout << ",";
        cout << values[i];
    }
    cout << "]";
}
}
int main() {
    {
        int expected = 2100;
        int actual = solution({"a 100", "b 200"}, {"TOAST ab 1000"}, {"TOAST 3"});
        cout << "\n__STUDY_RESULT__{\"test\":1,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    {
        int expected = 1400;
        int actual = solution({"a 100", "r 200", "c 300", "t 400"}, {"PIZZA arract 2000"}, {"PIZZA 2"});
        cout << "\n__STUDY_RESULT__{\"test\":2,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    {
        int expected = 7700;
        int actual = solution({"a 100", "b 200", "c 300", "d 400"}, {"BURGER abb 1500", "PASTA bcd 2000", "SALAD ac 900"}, {"PASTA 2", "SALAD 3", "BURGER 4"});
        cout << "\n__STUDY_RESULT__{\"test\":3,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    {
        int expected = -200;
        int actual = solution({"a 100", "b 200", "c 500"}, {"SOUP ab 300", "STEAK cc 800", "RICE a 500"}, {"SOUP 4", "STEAK 3", "RICE 1"});
        cout << "\n__STUDY_RESULT__{\"test\":4,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    {
        int expected = 5000;
        int actual = solution({"a 100", "b 200", "c 300"}, {"TOAST ab 1000", "JUICE c 800", "CAKE aa 1500", "TEA a 300"}, {"JUICE 2", "TOAST 3", "JUICE 1", "CAKE 0", "TOAST 2"});
        cout << "\n__STUDY_RESULT__{\"test\":5,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    return 0;
}
