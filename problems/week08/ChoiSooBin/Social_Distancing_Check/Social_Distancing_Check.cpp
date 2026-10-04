/*
문제: 거리두기 확인하기
링크: https://school.programmers.co.kr/learn/courses/30/lessons/81302
작성자: 최수빈
알고리즘: 완전 탐색, 구현, 맨해튼 거리

[문제 요약]
5개의 5x5 대기실이 주어진다.
각 대기실에서 응시자는 'P', 빈 테이블은 'O', 파티션은 'X'로 표현된다.

모든 응시자는 맨해튼 거리가 2 이하인 다른 응시자와 거리두기를 해야 하며,
두 응시자 사이에 파티션이 있어 서로 막혀 있는 경우에는 거리두기를 지킨 것으로 판단한다.

각 대기실이 거리두기를 지키고 있으면 1,
한 명이라도 거리두기를 지키지 않고 있으면 0을 반환하는 문제다.


[핵심 아이디어]
각 대기실에서 먼저 모든 응시자 'P'의 좌표를 vector에 저장한다.

그 후 응시자 두 명씩 모든 조합을 확인하면서
두 사람 사이의 맨해튼 거리를 계산한다.

맨해튼 거리:
abs(y1 - y2) + abs(x1 - x2)

두 사람의 거리에 따라 다음과 같이 판단한다.

1. 거리 > 2
   - 문제 조건과 관계없는 거리이므로 정상이다.

2. 거리 == 1
   - 두 사람이 바로 붙어 있으므로 무조건 거리두기 위반이다.

3. 거리 == 2
   - 두 사람의 위치 관계에 따라 사이의 파티션을 확인해야 한다.

   ① 같은 행에 있는 경우
      P ? P

      두 사람 사이의 한 칸이 'X'이면 정상,
      'O'이면 위반이다.

   ② 같은 열에 있는 경우
      P
      ?
      P

      마찬가지로 두 사람 사이의 한 칸이 'X'이면 정상,
      그렇지 않으면 위반이다.

   ③ 대각선에 있는 경우
      P ?
      ? P

      두 사람이 서로에게 갈 수 있는 경로가 2개 존재한다.
      따라서 두 중간 위치가 모두 'X'인 경우에만 정상이다.

      P X
      X P

      둘 중 하나라도 'O'라면 파티션으로 완전히 막히지 않았으므로
      거리두기 위반이다.


[풀이 과정]
1. 각 대기실 k에 대해 5x5 배열을 순회한다.

2. 'P'를 발견하면 해당 좌표를
   vector<pair<int, int>> person[k]에 저장한다.

3. 저장된 모든 응시자에 대해 두 명씩 조합을 만든다.

   for (int i = 0; i < person[k].size(); i++)
       for (int j = i + 1; j < person[k].size(); j++)

   j를 i+1부터 시작하여
   같은 두 사람을 중복해서 비교하지 않도록 한다.

4. 두 사람의 맨해튼 거리를 distance() 함수에서 계산한다.

5. isFine() 함수에서 두 사람의 거리와 위치 관계를 이용하여
   거리두기를 지키고 있는지 확인한다.

6. 한 쌍이라도 거리두기를 위반했다면 flag를 false로 만들고
   더 이상 해당 대기실을 검사할 필요가 없으므로 반복문을 종료한다.

7. 해당 대기실의 최종 flag 값을 answer에 저장한다.

8. 5개의 대기실에 대한 결과를 반환한다.


[복잡도]
각 대기실은 항상 5x5 크기이므로 실제 문제에서는 거의 상수 시간에 처리된다.

일반적으로 한 대기실에 응시자가 P명 있다고 하면,
모든 응시자 쌍을 비교하므로

P * (P - 1) / 2

번의 비교가 발생한다.

따라서 응시자 수를 P라고 할 때 시간 복잡도는 O(P^2)이다.

이 문제에서는 한 대기실에 최대 25명만 존재할 수 있으므로
최대 비교 횟수도 25C2 = 300회밖에 되지 않아 충분히 빠르다.

공간 복잡도는 각 응시자의 위치를 저장하므로 O(P)이다.


[막혔던 부분과 오답 원인 및 해결 방법]

- 첫 번째 실수 (대각선 파티션 검사 조건 누락)

  거리 2인 두 사람이 대각선으로 위치한 경우,
  두 사람 사이의 두 칸이 모두 파티션이어야만 거리두기를 지킨 것이다.

  처음에는 다음과 같이 두 번째 조건에서
  해당 위치가 'X'인지 비교하지 않고 char 값 자체를 조건식에 넣었다.

  [수정 전]

  if (place[p1.first][p2.second] == 'X'
      && place[p2.first][p1.second]) {
      return 1;
  }

  C++에서는 'O', 'P', 'X' 같은 char 값도 0이 아닌 정수값을 가지므로
  조건문에서는 대부분 true로 판단된다.

  따라서 두 번째 위치 역시 명시적으로 'X'인지 비교해야 한다.

  [수정 후]

  if (place[p1.first][p2.second] == 'X'
      && place[p2.first][p1.second] == 'X') {
      return 1;
  }

  [해결]
  char 값을 조건식에 그대로 넣지 않고,
  원하는 문자와 직접 비교하도록 수정했다.


- 두 번째 실수 (대기실마다 flag를 초기화하지 않음)

  처음에는 solution() 함수 시작 부분에서

  bool flag = true;

  를 한 번만 선언했다.

  하지만 이전 대기실에서 거리두기 위반을 발견해
  flag가 false가 된 상태에서 다음 대기실 검사를 시작하면,
  새로운 대기실에서도 이전 결과가 남을 수 있다.

  특히 다음 대기실에 응시자가 0명 또는 1명뿐이라
  응시자 쌍 비교 반복문이 실행되지 않는 경우에는
  flag가 true로 변경될 기회 자체가 없다.

  [수정 전]

  bool flag = true;

  for (int k = 0; k < 5; k++) {
      ...
  }

  [수정 후]

  for (int k = 0; k < 5; k++) {
      ...
      bool flag = true;

      // 현재 대기실 검사
  }

  [해결]
  flag는 "현재 대기실이 지금까지 거리두기를 지키고 있는가"를
  나타내는 변수이므로 각 대기실 검사를 시작할 때마다 true로 초기화했다.


[풀이를 통해 배운 점]
처음에는 거리두기 문제라 BFS를 사용해야 한다고 생각할 수도 있지만,
이 문제는 맵이 5x5로 매우 작기 때문에
모든 응시자 좌표를 저장하고 사람 두 명씩 직접 비교해도 충분하다.

또한 맨해튼 거리 2인 경우를
'같은 행 / 같은 열 / 대각선' 세 가지 경우로 나누면
문제에서 요구하는 파티션 조건을 그대로 구현할 수 있다.

다만 이 방식은 경우를 직접 나누기 때문에
대각선 조건이나 파티션 위치 계산에서 실수가 발생하기 쉽다.

다른 풀이로는 각 P에서 깊이 2까지만 BFS를 진행하면서
X를 만나면 해당 방향으로 탐색하지 않고,
거리 2 이내에 다른 P가 존재하는지 확인하는 방법도 있다.

현재 풀이는 문제의 조건을 직접 코드로 옮긴
완전 탐색 + 맨해튼 거리 기반 풀이이다.
*/
#include <string>
#include <vector>
#include <iostream>
using namespace std;


int distance(pair<int, int> p1, pair<int, int> p2){
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

int isFine(pair<int, int> p1, pair<int, int> p2, const vector<string>& place){
    int dist = distance(p1, p2);
    // cout << "p1: " << p1.first << ", " << p1.second << "p2: "<< p2.first << ", " << p2.second << '\n';
    // cout << dist << '\n';
    if(dist > 2) { 
        return 1;
    }else if(dist == 2){
        if(p1.first == p2.first){   // y 좌표가 같은 경우(같은 row에 존재)
            int y = p1.first;
            int x = (p1.second + p2.second) / 2;
            if(place[y][x] == 'X'){ // 사이에 파티션이 있다면
                return 1;
            }else{
                return 0;
            }
        }else if(p1.second == p2.second){   // x 좌표가 같은 경우(같은 column에 존재)
            int y = (p1.first + p2.first) / 2;
            int x = p1.second;
            if(place[y][x] == 'X'){ // 사이에 파티션이 있다면
                return 1;
            }else{
                return 0;
            }
        }else{  // 대각선에 존재
            if(place[p1.first][p2.second] == 'X' && place[p2.first][p1.second] == 'X'){
                return 1;
            }else{
                return 0;
            }
        }
    }else if(dist == 1){
        return 0;
    }
    return 1;
}



vector<int> solution(vector<vector<string>> places) {
    vector<int> answer;
    vector<pair<int, int>> person[5];  // 응시자 push_back

    for(int k = 0; k < 5; k++){
        // test_Case 단위
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                if(places[k][i][j] == 'P'){
                    person[k].push_back(make_pair(i, j));
                    // cout << "(" << person[k].back().first << ", " << person[k].back().second << "): "; 
                    // cout << "it's Person" << "\n";
                }
            }
        }
        bool flag = true;
        for(int i = 0; i < person[k].size(); i++){
            for(int j = i + 1; j < person[k].size(); j++){
                flag = isFine(person[k][i], person[k][j], places[k]);
                if(!flag){
                    // cout << k << '\n';
                    // cout << "it\'s end" << '\n';
                    break;

                }
            }
            if(!flag){
                break;
            }
        }
        answer.push_back(flag);
        // cout << "---" << '\n';
    }

    return answer;
}