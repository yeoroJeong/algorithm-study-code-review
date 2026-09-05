/* USER CODE 시작 */
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
 
using namespace std;
 
 
struct RESULT
{
    int cnt;
    int IDs[5];
};
 
 
struct Order
{
    int bevs[11] = { 0 };           // 남은 음료 종류,갯수
    int finishedBevs[11] = { 0 };   // 주문이 완료된 음료 종류,갯수
    int count = 0;              // 남은 음료 갯수
    int seq = 0;                    // 주문을 받은 순서
    int id = 0;                 // 주문 ID
    bool isCompleted = false;
    bool isCanceled = false;
};
 
// 자동 정렬
struct OrderRank
{
    int count;  // 남은 음료 갯수
    int seq;    // 주문 순서
    int id;     // 주문 ID
 
    bool operator<(const OrderRank& other)const
    {
        if (count != other.count)
            return count > other.count;  // 갯수가 많은.(내림)
        if (seq != other.seq)
            return seq < other.seq;      // 주문이 빠른(오름)
        return id < other.id;
    }
};
 
int num;                // 음료 종류
int orderSequence;      // 주문 순서
int activeOrderCnt;     // 현재 남아있는 주문 갯수
 
unordered_map<int, int> orderIdx; // mID -> orders의 index
vector<Order> orders;
 
// index -> 음료 번호,안에 들어가는 건 해당 음료를 주문한 주문ID.
// 주문은 항상 뒤에 추가되므로 vector 순서 자체가 주문을 받은 순서.
// 삭제는 직접하지 않고 bucketHead를 이용한 lazy deletion.
vector<int> bucket[11];
int bucketHead[11];
 
set<OrderRank> hurryOrder;    // 자동으로 급한 것 정렬.(1.남은 주문이 많음 2.주문이 빠름.)
 
// 음료 지급,주문 취소 시 음료가 배치되는 매커니즘이 동일해서 함수 형태로 만듬.
// 해당 음료가 있는 주문 중 가장 먼저 받은 주문에게 음료 1개를 배치.
// 취소되거나 완료된 주문은 bucket에서 실제 삭제하지 않고 건너뜀.
int updateStatus(int bevNum)
{
    while (bucketHead[bevNum] < (int)bucket[bevNum].size())
    {
        int mID = bucket[bevNum][bucketHead[bevNum]];
        Order& curOrder = orders[orderIdx[mID]];
 
        // 취소,완료 또는 해당 음료를 더 이상 필요로 하지 않는 주문
        if (curOrder.isCanceled || curOrder.isCompleted || curOrder.bevs[bevNum] <= 0)
        {
            bucketHead[bevNum]++;
            continue;
        }
 
        // 기존 음료 Rank에 대한 정보 삭제
        hurryOrder.erase({ curOrder.count,curOrder.seq,mID });
 
        // 음료 1개 배치
        curOrder.bevs[bevNum]--;
        curOrder.finishedBevs[bevNum]++;
        curOrder.count--;
 
        // 음료 번호에 대해서 이제 다 주문이 완료되었으면 다음 주문으로 이동
        if (curOrder.bevs[bevNum] <= 0)
            bucketHead[bevNum]++;
 
        // 현재 주문의 남은 음료가 0이라면 주문 완료
        if (curOrder.count <= 0)
        {
            curOrder.isCompleted = true;
            activeOrderCnt--;
        }
        else
        {
            // 음료 1개 삭제했다면 전체 순위에도 반영
            hurryOrder.insert({ curOrder.count,curOrder.seq,mID });
        }
 
        // 주문 ID값을 반환
        return mID;
    }
 
    // 해당 음료가 필요한 주문이 없음
    return -1;
}
 
void init(int N)
{
    num = N;
    orderSequence = 0;
    activeOrderCnt = 0;
 
    orders.clear();
    orderIdx.clear();
    hurryOrder.clear();
 
    for (int i = 0; i <= 10; ++i)
    {
        bucket[i].clear();
        bucketHead[i] = 0;
    }
}
 
int order(int mID, int M, int mBeverages[])
{
    // 주문 관리에 추가
    Order tmpOR;
    tmpOR.count = M;
    tmpOR.seq = orderSequence++;
    tmpOR.id = mID;
 
    // 동일 음료를 여러개 주문해도 bucket에는 주문ID를 1번만 추가
    bool added[11] = { false };
 
    for (int i = 0; i < M; ++i)
    {
        int bevNum = mBeverages[i];
        tmpOR.bevs[bevNum]++;
 
        if (!added[bevNum])
        {
            bucket[bevNum].push_back(mID);
            added[bevNum] = true;
        }
    }
 
    int idx = (int)orders.size();
    orderIdx[mID] = idx;
    orders.push_back(tmpOR);
 
    // 가장 급한 주문에 대한 추가
    hurryOrder.insert({ M,tmpOR.seq,mID });
 
    activeOrderCnt++;
 
    return activeOrderCnt;  // 남은 주문의 갯수를 반환
}
 
int supply(int mBeverage)
{
    // 해당 음료가 있는 가장 먼저 받은 주문에 배치
    // 없으면 -1을 반환하고 음료는 버림
    return updateStatus(mBeverage);
}
 
/*
주문 mID를 취소한다.취소되기 전 주문에 남은 음료의 개수를 반환한다.
주문 mID가 취소가 되면 주문 mID에 배치된 음료들은 재배치된다.재배치되는 과정은 배치되는 과정과 동일하다.
즉,그 음료가 있는 가장 먼저 받은 주문에게 재배치되고 해당 주문에서 그 음료 1개가 삭제된다.
 
조건
- 주문 mID가 이미 모든 음료가 제조되어 손님에게 전달된 경우에는 취소에 실패하고 0을 반환한다.
- 주문 mID가 이미 취소된 경우에는 취소에 실패하고 -1을 반환한다.
*/
int cancel(int mID)
{
    Order& curOrder = orders[orderIdx[mID]];
 
    // 이미 음료가 제조되어 손님에게 전달되었다면
    if (curOrder.isCompleted)
        return 0;
 
    // 주문 mID가 이미 취소된 경우
    if (curOrder.isCanceled)
        return -1;
 
    // 취소되기 전 남아있는 음료의 갯수
    int res = curOrder.count;
 
    // hurryOrder에서 취소될 주문 삭제
    hurryOrder.erase({ curOrder.count,curOrder.seq,mID });
 
    // 현재 주문의 취소 여부를 반영
    curOrder.isCanceled = true;
    activeOrderCnt--;
 
    // 취소될 주문에서 이미 제조 완료된 음료들을 순회
    for (int bevNum = 1; bevNum <= num; ++bevNum)
    {
        int finishedCnt = curOrder.finishedBevs[bevNum];
 
        // 이미 배치된 음료의 갯수만큼 다른 주문에 재배치
        for (int i = 0; i < finishedCnt; ++i)
        {
            // 재배치에 실패하면 -1을 반환하지만 해당 음료는 버리므로 무시
            updateStatus(bevNum);
        }
    }
 
    return res; // 취소되기 전 주문에 남은 음료의 개수를 반환
}
 
int getStatus(int mID)
{
    Order& curOrder = orders[orderIdx[mID]];
 
    // 완료되었으면
    if (curOrder.isCompleted)
        return 0;
 
    // 취소되었으면
    if (curOrder.isCanceled)
        return -1;
 
    // 이 주문의 남은 음료의 갯수를 반환
    return curOrder.count;
}
 
RESULT hurry()
{
    RESULT res;
    int orderCnt = (int)hurryOrder.size();
 
    // 남은 주문이 5개보다 적다면 그 갯수만큼,아니면 5개로 제한
    if (orderCnt > 5)
        orderCnt = 5;
 
    res.cnt = orderCnt;
    int cnt = 0;
 
    for (auto order : hurryOrder)
    {
        if (cnt >= orderCnt)
            break;
 
        res.IDs[cnt] = order.id;
        cnt++;
    }
 
    return res;
}
/* USER CODE 끝 */