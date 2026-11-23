#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

// 과일 번호, 우선순위대로 정렬된 과일특징들
bool compare(pair<int, vector<int>>& a, pair<int, vector<int>>& b)
{
    int n = (int)a.second.size();

    for (int i = 0; i < n; ++i)
    {
        if (a.second[i] > b.second[i])
            return true;
        else if (a.second[i] < b.second[i])
            return false;
    }

    // 배열의 구성이 동일?
    // 앞에 있는 것이 더 작은 과일 번호이기 때문에 true
    return a.first < b.first;
}

// 우선순위대로 특징을 재배열 시켜서
// 그 재배열한 위치 순으로 본다. 더 큰것이 있으면 다음 과일로 넘어감. 
vector<int> solution(vector<vector<int>> fruits, vector<vector<int>> priority) {

    // 특징의 갯수는 모두 동일 -> 1번째 과일 특징의 갯수
    int nIngres = (int)fruits[0].size();

    // 과일의 갯수
    int nFruit = (int)fruits.size();

    // 우선 순위대로 정렬된 <과일번호, 특징>
    vector<pair<int, vector<int>>> sortedFruitIngres
    (nFruit, pair<int, vector<int>>(nFruit, vector<int>(nIngres, 0)));

    // (int)fruits가 과일의 수
    for (int i = 0; i < (int)fruits.size(); ++i)
    {
        vector<int> fruitIngre = fruits[i];

        // 우선 순위대로 과일 특징을 정렬시키기.
        for (int j = 0; j < nIngres; ++j)
        {
            // 새롭게 들어갈 index
            int index = priority[i][j]-1;

            // fruits[i][j]의 값이 index의 자리로 가야한다. 
            sortedFruitIngres[i].first = i;
            sortedFruitIngres[i].second[index] = fruits[i][j];
        }
    }

    sort(sortedFruitIngres.begin(), sortedFruitIngres.end(), compare);

    // 반환할 결과값을 담은 것
    vector<int> result;
    for (auto& i : sortedFruitIngres)
    {
        int num = i.first;
        result.push_back(num + 1);
    }

    return result;
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
        vector<int> expected = {1};
        vector<int> actual = solution({{42}}, {{1}});
        cout << "\n__STUDY_RESULT__{\"test\":1,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    {
        vector<int> expected = {1, 2};
        vector<int> actual = solution({{1, 3, 2, 5, 4}, {6, 1, 10, 10, 10}}, {{2, 1, 3, 4, 5}, {3, 1, 2, 4, 5}});
        cout << "\n__STUDY_RESULT__{\"test\":2,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    {
        vector<int> expected = {3, 2, 1, 4};
        vector<int> actual = solution({{7, 4, 9}, {7, 5, 1}, {7, 5, 8}, {6, 100, 100}}, {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}, {1, 2, 3}});
        cout << "\n__STUDY_RESULT__{\"test\":3,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    {
        vector<int> expected = {4, 1, 2, 3};
        vector<int> actual = solution({{8, 3, 5}, {5, 8, 3}, {3, 5, 8}, {8, 3, 6}}, {{1, 2, 3}, {3, 1, 2}, {2, 3, 1}, {1, 2, 3}});
        cout << "\n__STUDY_RESULT__{\"test\":4,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    {
        vector<int> expected = {3, 4, 5, 2, 1};
        vector<int> actual = solution({{0, 100, 50, 0}, {100, 0, 0, 100}, {100, 50, 0, 100}, {100, 100, 0, 50}, {0, 100, 50, 100}}, {{4, 2, 3, 1}, {1, 4, 3, 2}, {2, 3, 4, 1}, {1, 2, 4, 3}, {3, 1, 4, 2}});
        cout << "\n__STUDY_RESULT__{\"test\":5,\"passed\":" << (actual == expected ? "true" : "false") << ",\"expected\":";
        study_runner::output(expected);
        cout << ",\"actual\":";
        study_runner::output(actual);
        cout << "}\n";
    }
    return 0;
}
