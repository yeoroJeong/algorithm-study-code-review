/*
분과 초를 따로 계산하면 10초를 이동할 때마다
초가 60을 넘거나 음수가 되는 경우를 처리해야 한다.
시간을 모두 초 단위로 바꿔서 계산하고,
마지막에만 다시 문자열로 변환하는 방식으로 풀었다.

오프닝은 명령으로 이동한 뒤뿐 아니라
처음 재생 위치에서도 건너뛰어야 하므로,
명령 실행 전 한 번과 각 명령 실행 후에 확인했다.

문제: [PCCP 기출문제] 1번 / 동영상 재생기
링크: https://school.programmers.co.kr/learn/courses/30/lessons/340213
작성자: 최수빈(sbyy77dev)
알고리즘: 구현, 문자열

[문제 요약]
동영상의 길이, 현재 재생 위치, 오프닝 구간과 명령 목록이 주어진다.

"prev"는 10초 전으로, "next"는 10초 후로 이동한다.
영상 범위를 벗어나면 처음 또는 마지막 위치로 이동한다.

현재 위치가 오프닝 구간에 포함되면 오프닝 끝으로 이동한다.
모든 명령을 처리한 뒤 최종 위치를 "mm:ss" 형식으로 반환한다.

[핵심 아이디어]
"mm:ss" 형태의 시간을 분 * 60 + 초로 변환한다.
모두 정수로 바꾸면 이동과 구간 비교를 간단하게 처리할 수 있다.

처음 위치가 오프닝 안에 있는지 먼저 확인한다.
이후 각 명령마다 10초 이동, 영상 범위 보정,
오프닝 확인 순서로 처리한다.

마지막에는 현재 시간을 60으로 나눈 몫과 나머지를 이용해
분과 초를 구하고, 한 자리 숫자 앞에는 0을 붙인다.

[풀이 과정]
1. video_len, pos, op_start, op_end를 초 단위로 변환한다.
   - substr(0, 2)로 분 부분을 가져온다.
   - substr(3, 2)로 초 부분을 가져온다.
   - stoi로 정수로 바꾼 뒤 분 * 60 + 초를 계산한다.
2. 처음 위치가 오프닝 구간에 포함되면 오프닝 끝으로 이동한다.
3. commands를 순서대로 확인한다.
4. "prev"라면 10을 빼고, 음수가 되면 0으로 맞춘다.
5. "next"라면 10을 더하고, 영상 길이를 넘으면 마지막으로 맞춘다.
6. 이동한 위치가 오프닝 구간에 포함되면 오프닝 끝으로 이동한다.
7. 모든 명령을 처리한 뒤 now / 60으로 분을,
   now % 60으로 초를 구한다.
8. 분과 초를 각각 두 자리로 맞춰 "mm:ss" 형태로 반환한다.

[복잡도]
- 시간 복잡도: O(C)
  C는 명령의 개수이며, 각 명령을 한 번씩 처리한다.
  시간 문자열은 길이가 5로 고정되어 변환에 O(1)이 걸린다.

- 추가 공간 복잡도: O(1)
  계산에 필요한 정수 변수와 결과 문자열만 사용한다.
  단, 주어진 함수의 값 전달로 발생하는 매개변수 복사는 제외한다.

[구현 시 주의할 부분]
- 처음 위치의 오프닝 확인
  명령 실행 후에만 확인하면 시작 위치가 오프닝 안인 경우를 놓친다.
  예를 들어 04:05가 오프닝 안이고 끝이 04:07이라면,
  먼저 04:07로 이동한 뒤 "next"를 실행해야 04:17이 된다.

- 오프닝 경계 포함
  오프닝 시작과 끝도 구간에 포함되므로
  now >= start && now <= end로 비교한다.

- 영상 범위 보정 순서
  10초 이동 후 위치를 0부터 영상 길이 사이로 맞추고,
  그 위치를 기준으로 오프닝에 포함되는지 확인한다.

- 결과 문자열의 앞자리 0
  to_string(5)는 "5"를 반환하므로
  10보다 작은 분과 초에는 직접 "0"을 붙여야 한다.

[사용한 함수]
- substr(시작 위치, 길이): 문자열의 일부분을 가져온다.
- stoi(문자열): 숫자 문자열을 int로 변환한다.
- to_string(숫자): 숫자를 문자열로 변환한다.
- 위 함수들은 <string>으로 사용할 수 있다.
*/

#include <string>
#include <vector>

using namespace std;

string solution(string video_len, string pos, string op_start,
                string op_end, vector<string> commands) {
    string answer = "";

    // 분과 초를 따로 계산하지 않도록 모두 초로 바꾸기
    int len = stoi(video_len.substr(0, 2)) * 60
              + stoi(video_len.substr(3, 2));
    int now = stoi(pos.substr(0, 2)) * 60
              + stoi(pos.substr(3, 2));
    int start = stoi(op_start.substr(0, 2)) * 60
                + stoi(op_start.substr(3, 2));
    int end = stoi(op_end.substr(0, 2)) * 60
              + stoi(op_end.substr(3, 2));

    // 처음 위치도 오프닝 안에 있으면 건너뛰기
    if (now >= start && now <= end) {
        now = end;
    }

    for (int i = 0; i < (int)commands.size(); i++) {
        if (commands[i] == "prev") {
            now -= 10;

            // 0초보다 작아지면 처음으로 이동
            if (now < 0) {
                now = 0;
            }
        } else {
            now += 10;

            // 영상 길이를 넘으면 마지막으로 이동
            if (now > len) {
                now = len;
            }
        }

        // 명령으로 이동한 위치가 오프닝 안인지 확인
        if (now >= start && now <= end) {
            now = end;
        }
    }

    // 초를 다시 분:초 형태로 바꾸기
    int minute = now / 60;
    int second = now % 60;

    // 한 자리 숫자는 앞에 0 붙이기
    if (minute < 10) {
        answer += "0";
    }
    answer += to_string(minute);
    answer += ":";

    if (second < 10) {
        answer += "0";
    }
    answer += to_string(second);

    return answer;
}