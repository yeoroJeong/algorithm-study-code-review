/*
좋은 과일 정렬하기
n개의 과일(1~n번까지 번호), 모든 과일은 동일하게 m개의 특징 가짐
각 특징의 값은 0~100 정수
각 과일에는 특징을 비교할 우선순위 주어짐

품질 비교 규칙
1. 각 과일의 특징값을 해당 과일의 우선순위가 높은 순서대로 나열
2. 나열한 값들을 첫 번째부터 차례대로 비교
3. 처음으로 값이 달라지는 위치에서 값이 큰 과일이 더 좋은 과일
4. 모든 위치의 값이 같다면 과일 번호가 작은 과일이 먼저 옴
=> 특징값을 우선순위에 맞게 재 배열 후 연산하자

과일을 좋은 순서대로 정렬한 뒤, 과일 번호를 담은 배열 반환

입력
fruits: 각 과일의 특징값을 담은 2차원 정수 배열
priority:각 과일의 특징별 우선순위를 담은 2차원 정수 배열
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

vector<int> solution(vector<vector<int>> fruits, vector<vector<int>> priority) {
    // 여기에 풀이를 작성하세요.
    vector<int> result;
    vector<vector<int>> srtf(fruits.size(), vector<int>(fruits[0].size(), 0));
    // 과일 특징값 순서에 맞게 정렬 된 것
    for (int i = 0; i < fruits.size(); i++) {
        vector<int> res(fruits[i].size(), 0); // 특징별 우선순위에 따른 비교할 값 순서 세팅
        for (int j = 0; j < fruits[i].size(); j++) {
            res[priority[i][j] - 1] = fruits[i][j]; // (특징별 우선순위의 값 - 1) 값 => 순서
        }
        srtf[i] = res;
    }
    vector<vector<int>> temp = srtf;
    sort(srtf.begin(), srtf.end(), greater<>()); // 내림차순 정렬
    for (int i = 0; i < srtf.size(); i++) {
        for (int j = 0; j < srtf.size(); j++) {
            if (srtf[i] == temp[j]) { // 동일하다면
                result.push_back(j + 1); // 과일번호 푸시 후
                temp[j][0] = 0; // 다시 비교 못하게 0으로 세팅
                break;
            }
        }
    }
    return result;
}