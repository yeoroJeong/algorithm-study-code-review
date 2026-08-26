#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <cmath>
 
 
using namespace std;
 
struct TaxiInfo
{
    int x;
    int y;
    int totalDistance = 0;
    int clientMoveDistance = 0;
 
    // cellX, cellY, bucketPos는 더 이상 저장하지 않음
    // -> 현재 셀은 x/cellSize, y/cellSize로 그때그때 계산하면 되고
    //    unordered_set은 erase(id)/insert(id)만으로 위치 관리가 끝남
};
 
int n;
int limitDistance;
int cellSize;
int gridCnt;
 
vector<TaxiInfo> Taxis;
 
// N = 10L 조건상 gridCnt가 항상 10 근처로 고정되므로 10x10이면 충분
// vector 대신 unordered_set 사용 -> erase/insert가 평균 O(1)이라 swap-remove 로직이 불필요해짐
unordered_set<int> bucket[10][10];
 
// 자기 셀 기준 3x3 이웃 오프셋
// cellSize = L로 고정하면, 반경 L 이내의 모든 후보는 이 9개 셀 안에만 존재함이 보장됨
// (자기 셀 끝에서 L만큼 이동해도 바로 옆 이웃 셀을 넘어가지 않음)
int dCellX[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
int dCellY[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
 
 
struct Result
{
    int mX, mY;
    int mMoveDistance;
    int mRideDistance;
};
 
 
// top5 유지용 - {clientMoveDistance, id} 쌍을 넣어두고
// 항상 "지우고 다시 넣기"만으로 갱신 (pickup, reset 공통 패턴)
// 거리 내림차순, 같으면 ID 오름차순 (원본 compare와 동일 기준)
struct TaxiRank
{
    int dist;
    int id;
 
    bool operator<(const TaxiRank& other) const
    {
        if (dist != other.dist)
            return dist > other.dist;
 
        return id < other.id;
    }
};
 
set<TaxiRank> bestRide;
 
void init(int N, int M, int L, int mXs[], int mYs[])
{
    Taxis.clear();
 
    n = N;
    limitDistance = L;
 
    // 셀 크기를 L 자체로 고정 -> N=10L 조건상 gridCnt는 항상 10
    cellSize = max(1, L);
    gridCnt = (N + cellSize - 1) / cellSize;
 
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            bucket[i][j].clear();
        }
    }
 
    bestRide.clear();
 
    // ID 0 dummy
    TaxiInfo dummy;
    dummy.x = -1;
    dummy.y = -1;
    Taxis.push_back(dummy);
 
    for (int i = 0; i < M; ++i)
    {
        TaxiInfo taxi;
 
        taxi.x = mXs[i];
        taxi.y = mYs[i];
 
        int id = i + 1;
 
        Taxis.push_back(taxi);
 
        bucket[taxi.x / cellSize][taxi.y / cellSize].insert(id);
 
        bestRide.insert({ 0, id });
    }
}
 
 
// 더 이상 필요 없음: cellSize = L 고정 + 3x3 탐색만으로
// 반경 L 이내 후보 셀을 놓치지 않는다는 것이 보장되므로
// 셀 단위 최소거리 필터링/정렬 자체가 불필요해짐
//
// int getCellMinDistance(...)
 
 
int pickup(int mSX, int mSY, int mEX, int mEY)
{
    int minId = -1;
    int minDistance = limitDistance + 1;
 
    int lx = mSX / cellSize;
    int ly = mSY / cellSize;
 
    // 1. 자기 셀 기준 3x3(9개) 이웃 셀만 검사
    //    -> 후보 셀을 모으고 정렬하던 기존 로직이 통째로 사라짐
 
    for (int d = 0; d < 9; ++d)
    {
        int cx = lx + dCellX[d];
        int cy = ly + dCellY[d];
 
        if (cx < 0 || cx >= gridCnt || cy < 0 || cy >= gridCnt)
            continue;
 
        for (int id : bucket[cx][cy])
        {
            int x = Taxis[id].x;
            int y = Taxis[id].y;
 
            int toClient = abs(x - mSX) + abs(y - mSY);
 
            if (toClient > limitDistance)
                continue;
 
            if (toClient < minDistance)
            {
                minDistance = toClient;
                minId = id;
            }
            else if (toClient == minDistance && id < minId)
            {
                minId = id;
            }
        }
    }
 
    // 호출 가능한 택시 없음
    if (minId == -1)
        return -1;
 
    // 2. 선택된 택시의 거리 정보 갱신
 
    int rideDistance =
        abs(mSX - mEX) +
        abs(mSY - mEY);
 
    // bucket에서 제거 (현재 위치는 x/cellSize, y/cellSize로 바로 계산)
    bucket[Taxis[minId].x / cellSize][Taxis[minId].y / cellSize].erase(minId);
 
    // bestRide에서도 이전 상태를 지움
    bestRide.erase({ Taxis[minId].clientMoveDistance, minId });
 
    Taxis[minId].totalDistance +=
        minDistance + rideDistance;
 
    Taxis[minId].clientMoveDistance +=
        rideDistance;
 
    // 실제 좌표 갱신
    Taxis[minId].x = mEX;
    Taxis[minId].y = mEY;
 
    // 새 위치의 bucket에 다시 삽입
    bucket[mEX / cellSize][mEY / cellSize].insert(minId);
 
    // bestRide에도 갱신된 상태로 다시 삽입
    bestRide.insert({ Taxis[minId].clientMoveDistance, minId });
 
    return minId;
}
 
Result reset(int mNo)
{
    // 총 이동 거리 및 손님을 태우고 이동한 거리를 모두 저장 후 출력,
    // 그리고 초기화된 택시의 이동거리 2개는 모두 0
    Result res;
 
    res.mX = res.mY = res.mMoveDistance = res.mRideDistance = -1;
 
    res.mX = Taxis[mNo].x;
    res.mY = Taxis[mNo].y;
    res.mMoveDistance = Taxis[mNo].totalDistance;
    res.mRideDistance = Taxis[mNo].clientMoveDistance;
 
    // bestRide도 "지우고 다시 넣기"로 동일하게 갱신
    // -> pickup과 완전히 같은 패턴이라 top5 관련 예외 처리가 필요 없음
    bestRide.erase({ Taxis[mNo].clientMoveDistance, mNo });
 
    // 위치를 제외한 거리들만 초기화
    Taxis[mNo].totalDistance = 0;
    Taxis[mNo].clientMoveDistance = 0;
 
    bestRide.insert({ 0, mNo });
 
    return res;
}
 
 
// mNos -> 전역으로 이미 선언된 것
// bestRide는 항상 최신 상태로 유지되고 있으므로
// 맨 앞 5개만 꺼내면 됨 (O(5), 사실상 O(1))
void getBest(int mNos[])
{
    int count = 0;
 
    for (const auto& taxi : bestRide)
    {
        mNos[count++] = taxi.id;
 
        if (count == 5)
            break;
    }
}