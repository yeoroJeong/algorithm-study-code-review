#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

// first: 우선순위에 따라 재배치한 특징값
// second: 과일 번호
bool compareFruit(
    const pair<vector<int>, int>& a,
    const pair<vector<int>, int>& b
) {
    // vector 비교는 첫 원소부터 차례대로 사전순 비교
    if (a.first != b.first) {
        return a.first > b.first;
    }

    // 특징값이 모두 같으면 과일 번호 오름차순
    return a.second < b.second;
}

vector<int> solution(
    vector<vector<int>> fruits,
    vector<vector<int>> priority
) {
    int n = (int)fruits.size();
    int m = (int)fruits[0].size();

    vector<pair<vector<int>, int>> ordered;

    for (int i = 0; i < n; i++) {
        vector<int> values(m);

        for (int j = 0; j < m; j++) {
            // 우선순위 1인 특징은 values[0]에 저장
            int position = priority[i][j] - 1;
            values[position] = fruits[i][j];
        }

        ordered.push_back({values, i + 1});
    }

    sort(ordered.begin(), ordered.end(), compareFruit);

    vector<int> answer;

    for (int i = 0; i < n; i++) {
        answer.push_back(ordered[i].second);
    }

    return answer;
}