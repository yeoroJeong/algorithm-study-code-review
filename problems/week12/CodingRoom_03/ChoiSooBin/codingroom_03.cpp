#include <vector>

using namespace std;

// target부터 1까지의 추출 과정을 거꾸로 복원한다.
//
// 현재 stack1, stack2는
// "정방향에서 target을 추출한 직후의 상태"를 나타낸다.
//
// costs : 각 정수를 추출할 때 발생한 이동 비용
// answer: 지금까지 찾은 초기 상태 중 사전순으로 가장 앞서는 배열
void restoreStacks(
    int target,
    const vector<int>& costs,
    const vector<int>& stack1,
    const vector<int>& stack2,
    vector<int>& answer
) {
    // 1까지 모두 다시 넣었다면 초기 상태 복원이 끝난 것이다.
    if (target == 0) {

        // 문제 조건상 초기 stack2는 비어 있어야 한다.
        // 원소가 남아 있으면 올바른 초기 상태가 아니다.
        if (!stack2.empty()) {
            return;
        }

        // 아직 정답 후보가 없거나,
        // 새로 찾은 stack1이 기존 정답보다 사전순으로 앞서면 갱신한다.
        //
        // vector<int>끼리의 < 연산은 정수 원소 기준 사전순 비교다.
        if (answer.empty() || stack1 < answer) {
            answer = stack1;
        }

        return;
    }

    // target을 추출할 때 발생한 이동 비용
    //
    // costs[0]은 정수 1의 비용이므로,
    // 정수 target의 비용은 costs[target - 1]이다.
    int moveCount = costs[target - 1];

    // =========================================================
    // 경우 1: target을 stack1에서 추출했다고 가정
    //
    // 정방향:
    // stack1 → stack2로 moveCount개 이동한 뒤 target 추출
    //
    // 역방향:
    // stack1에 target을 넣고,
    // stack2 → stack1로 moveCount개를 되돌린다.
    // =========================================================

    // stack2에 되돌릴 원소가 충분히 있어야 한다.
    if ((int)stack2.size() >= moveCount) {

        // 현재 상태를 복사해서 이 분기에서만 사용한다.
        vector<int> nextStack1 = stack1;
        vector<int> nextStack2 = stack2;

        // 마지막으로 수행했던 "target 추출"을 되돌린다.
        nextStack1.push_back(target);

        // 그전에 수행했던 원소 이동을 거꾸로 되돌린다.
        for (int i = 0; i < moveCount; i++) {

            // stack2의 맨 위 원소를 확인한다.
            int value = nextStack2.back();

            // stack2에서 제거한다.
            nextStack2.pop_back();

            // stack1의 맨 위에 넣는다.
            nextStack1.push_back(value);
        }

        // target 복원이 끝났으므로,
        // 그 전에 추출했던 target - 1을 복원한다.
        restoreStacks(
            target - 1,
            costs,
            nextStack1,
            nextStack2,
            answer
        );
    }

    // =========================================================
    // 경우 2: target을 stack2에서 추출했다고 가정
    //
    // 정방향:
    // stack2 → stack1로 moveCount개 이동한 뒤 target 추출
    //
    // 역방향:
    // stack2에 target을 넣고,
    // stack1 → stack2로 moveCount개를 되돌린다.
    // =========================================================

    // stack1에 되돌릴 원소가 충분히 있어야 한다.
    if ((int)stack1.size() >= moveCount) {

        // 경우 1의 결과가 아닌, 이 함수가 받은 원래 상태를 복사한다.
        vector<int> nextStack1 = stack1;
        vector<int> nextStack2 = stack2;

        // 추출했던 target을 stack2에 다시 넣는다.
        nextStack2.push_back(target);

        // 이동했던 원소들을 stack1 → stack2로 되돌린다.
        for (int i = 0; i < moveCount; i++) {
            int value = nextStack1.back();
            nextStack1.pop_back();
            nextStack2.push_back(value);
        }

        // 다음으로 target - 1을 복원한다.
        restoreStacks(
            target - 1,
            costs,
            nextStack1,
            nextStack2,
            answer
        );
    }
}

vector<int> solution(vector<int> costs) {
    vector<int> answer;

    // 모든 정수를 추출한 뒤에는 두 스택이 모두 비어 있다.
    vector<int> stack1;
    vector<int> stack2;

    // 마지막으로 추출한 n부터 거꾸로 복원한다.
    restoreStacks(
        (int)costs.size(),
        costs,
        stack1,
        stack2,
        answer
    );

    return answer;
}