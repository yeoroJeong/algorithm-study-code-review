/*
    기존 코드 
    -> N!의 시간 복잡도에 의해 Error 그 이외의 로직은 올바른 로직
    
    memo를 통해 현재 배열 그 자체를 key값으로 설정해서, 매번 배열을 계산하는 것이 아니라
    
    그 배열의 최댓값을 바로 도출 
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>       

using namespace std;

// ============================================================
// [방법 A 추가] 메모이제이션 캐시
// - key   : 현재 남아있는 블록들의 배열(vector<int>) 그 자체
// - value : 그 배열 상태에서 앞으로 얻을 수 있는 최댓값
// ============================================================
map<vector<int>, long long> memo;

long long recursiveBlock(vector<int>& blocks)
{
    if (blocks.size() == 1)
    {
        return blocks[0];
    }

    // ------------------------------------------------------------
    // [방법 A 추가] 캐시 조회
    // 지금 들어온 blocks와 "값과 순서가 완전히 동일한" 배열을
    // 예전에 이미 계산해본 적이 있는지 map에서 찾는다.
    // - 있다면: 아래의 반복문(모든 블록을 하나씩 제거해보는 시도)을
    //           단 한 번도 실행하지 않고 즉시 그 값을 반환한다.
    //           => 동일한 부분 문제를 중복 계산하지 않게 되어
    //              시간복잡도가 O(N!)에서 O(2^N) 수준으로 줄어드는 핵심 부분.

    // - 없다면: 아래로 내려가 원본 로직대로 정상적으로 계산을 진행한다.
    // ------------------------------------------------------------
    auto it = memo.find(blocks);
    if (it != memo.end())
    {
        return it->second;
    }

    long long maxResult = 0;
    for (int index = 0; index < (int)blocks.size(); ++index)
    {
        long long currentScore = 0;

        // 1) 맨 왼쪽 블록을 깨는 경우 -> 왼쪽엔 이웃이 없고 오른쪽 이웃만 존재
        if (index == 0)
        {
            currentScore = blocks[index + 1];
        }
        // 2) 맨 오른쪽 블록을 깨는 경우 -> 오른쪽엔 이웃이 없고 왼쪽 이웃만 존재
        else if (index == (int)blocks.size() - 1)
        {
            currentScore = blocks[index - 1];
        }
        // 3) 양쪽 다 이웃이 있는 경우 -> 두 이웃 값의 곱
        else
        {
            currentScore =
                static_cast<long long>(blocks[index - 1]) * blocks[index + 1];
        }


        int removedBlock = blocks[index];
        blocks.erase(blocks.begin() + index);

        long long result = currentScore + recursiveBlock(blocks);

        maxResult = max(maxResult, result);

        blocks.insert(blocks.begin() + index, removedBlock);
    }

    // ------------------------------------------------------------
    // [방법 A 추가] 캐시 저장
    // 반복문을 모두 마쳐 이 상태(blocks)의 "확정된 최댓값"이 나온 시점.
    // 다음에 재귀 호출 어딘가에서 똑같은 blocks 상태가 다시 나타나면
    // 위쪽의 캐시 조회 코드가 이 값을 즉시 재사용할 수 있도록 저장해둔다.
    // ------------------------------------------------------------
    memo[blocks] = maxResult;

    return maxResult;
}

int main()
{
    int T;
    cin >> T;

    for (int test_case = 1; test_case <= T; ++test_case)
    {
        int n;
        cin >> n;

        vector<int> blocks(n);

        for (int i = 0; i < n; ++i)
        {
            cin >> blocks[i];
        }

        memo.clear();

        long long result = recursiveBlock(blocks);

        cout << "#" << test_case << " " << result << '\n';
    }

    return 0;
}