#include <iostream>
#include <vector>
#include <deque>

using namespace std;

// 1번 ~ 4번 자석 사용
vector<deque<int>> magnets(5);

int K;
int totalScore;


/*
 * 자석을 한 칸 회전
 *
 * direction
 *  1 : 시계 방향
 * -1 : 반시계 방향
 */
void rotateMagnet(int magnetIndex, int direction)
{
    // 시계 방향
    if (direction == 1)
    {
        int lastPole = magnets[magnetIndex].back();

        magnets[magnetIndex].pop_back();
        magnets[magnetIndex].push_front(lastPole);
    }

    // 반시계 방향
    else
    {
        int firstPole = magnets[magnetIndex].front();

        magnets[magnetIndex].pop_front();
        magnets[magnetIndex].push_back(firstPole);
    }
}


/*
 * 현재 자석을 회전시키고,
 * 맞닿은 극이 서로 다르면 인접 자석까지 반대 방향으로 회전
 *
 * magnetIndex   : 현재 회전할 자석 번호
 * direction     : 현재 자석의 회전 방향
 * previousIndex : 직전에 회전시킨 자석 번호
 *
 * previousIndex를 확인하는 이유:
 * 이미 회전 전파가 넘어온 방향으로 다시 재귀 호출하는 것을 방지
 */
void rotateConnectedMagnets(
    int magnetIndex,
    int direction,
    int previousIndex)
{
    bool shouldRotateLeft = false;
    bool shouldRotateRight = false;

    /*
     * 오른쪽 자석 확인
     *
     * 현재 자석의 2번 위치와
     * 오른쪽 자석의 6번 위치가 맞닿아 있음
     */
    if (magnetIndex < 4 && magnetIndex + 1 != previousIndex)
    {
        if (magnets[magnetIndex][2] != magnets[magnetIndex + 1][6])
        {
            shouldRotateRight = true;
        }
    }

    /*
     * 왼쪽 자석 확인
     *
     * 현재 자석의 6번 위치와
     * 왼쪽 자석의 2번 위치가 맞닿아 있음
     */
    if (magnetIndex > 1 && magnetIndex - 1 != previousIndex)
    {
        if (magnets[magnetIndex][6] != magnets[magnetIndex - 1][2])
        {
            shouldRotateLeft = true;
        }
    }

    /*
     * 인접 자석의 회전 여부는
     * 현재 자석이 회전하기 전 상태를 기준으로 판단해야 하므로
     * 위에서 먼저 확인한 뒤 회전
     */
    rotateMagnet(magnetIndex, direction);

    // 오른쪽 자석은 현재 자석과 반대 방향으로 회전
    if (shouldRotateRight)
    {
        rotateConnectedMagnets(
            magnetIndex + 1,
            -direction,
            magnetIndex
        );
    }

    // 왼쪽 자석도 현재 자석과 반대 방향으로 회전
    if (shouldRotateLeft)
    {
        rotateConnectedMagnets(
            magnetIndex - 1,
            -direction,
            magnetIndex
        );
    }
}


int main()
{
    int T;
    cin >> T;

    for (int testCase = 1; testCase <= T; testCase++)
    {
        cin >> K;

        totalScore = 0;

        // 각 자석의 8개 극 정보 입력
        for (int magnetIndex = 1; magnetIndex <= 4; magnetIndex++)
        {
            for (int poleIndex = 0; poleIndex < 8; poleIndex++)
            {
                int magneticPole;
                cin >> magneticPole;

                magnets[magnetIndex].push_back(magneticPole);
            }
        }

        // K번의 자석 회전 명령 수행
        for (int command = 0; command < K; command++)
        {
            int magnetIndex;
            int direction;

            cin >> magnetIndex >> direction;

            rotateConnectedMagnets(
                magnetIndex,
                direction,
                0
            );
        }

        /*
         * 각 자석의 0번 위치가 S극(1)이면 점수 획득
         *
         * 1번 자석 : 1점
         * 2번 자석 : 2점
         * 3번 자석 : 4점
         * 4번 자석 : 8점
         */
        for (int magnetIndex = 1; magnetIndex <= 4; magnetIndex++)
        {
            totalScore +=
                magnets[magnetIndex].front() * (1 << (magnetIndex - 1));
        }

        cout << "#" << testCase << " "
             << totalScore << "\n";

        // 다음 테스트 케이스를 위해 초기화
        for (int magnetIndex = 1; magnetIndex <= 4; magnetIndex++)
        {
            magnets[magnetIndex].clear();
        }
    }

    return 0;
}
