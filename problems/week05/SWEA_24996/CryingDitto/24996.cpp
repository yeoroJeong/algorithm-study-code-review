// SWEA 24996. [Pro] 카페2
// 주문 순서를 보장하기 위해 vector 사용함
// 각 음료별 대기Queue를 만들어 해당 Queue에 해당 음료를 필요로 하는 Order의 vector index를 집어넣음
// vector에서 매번 찾는 게 너무 오래걸릴 것 같아서 unordered_map<mID, vector내 Order의 인덱스> 사용
// unordered_map의 탐색 시간은 O(1)이므로 필요한 주문을 빨리 찾을 수 있지 않을까 싶어 사용함
// hurry 주문을 찾을 때 남은 주문 수가 같으면 주문 순서 상 앞선 것을 먼저 출력하기 위해 partial_sort에 비교 함수 추가 제공하여 작성함 => time out
// hurry 주문을 찾기 위해 remaining beverage count별 order의 vIdx 목록 set으로 관리
//		각 주문의 vector index로 set에 빠르게 추가/삭제 가능

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <set>
#define MAX_NUM_BEVERAGES       (10)

using namespace std;
struct RESULT
{
	int cnt;
	int IDs[5];
};

struct Order
{
	int mId; // Order #
	int remainBevCount; // for partial sort, hurry()
	// Beverage #1's supplied / completed count will be stored in bevCountList[1]
	int bevToSupplyList[MAX_NUM_BEVERAGES + 1];
	int bevCompletedList[MAX_NUM_BEVERAGES + 1];

	bool isCancelled = false;
	bool isCompleted = false;
};

// use drinkTypes as a counter to access Order.bevToSupplyList[] or Order.bevCompletedList[]
// aliveOrders : to return alive orders fast in `int order()`
int drinkTypes, aliveOrders;

// to keep the order of orders, vector is used.
vector<Order> orders;
// find the 'vector index' of an order using key 'mId'
unordered_map<int, int> idMap;
// queue to keep 'vector index' of an order which needs to supply each beverage #.
queue<int> bevQ[MAX_NUM_BEVERAGES + 1];

// an array of sets which holds each order's vIdx 
// a set of orders whose remainBevCount is equal to array index
set<int> orderSetPerBevRemain[MAX_NUM_BEVERAGES + 1];
void init(int N)
{
	// drink 종류 수 N // min 3 ~ max 10
	drinkTypes = N;
	aliveOrders = 0;
	orders.clear();
	idMap.clear();
	for (int i = 1; i <= MAX_NUM_BEVERAGES; i++)
	{
		bevQ[i] = queue<int>();
		orderSetPerBevRemain[i] = set<int>();
	}
	return;
}

int order(int mID, int M, int mBeverages[])
{
	// mID: order id
	// M: drink counts (1 ~ 10) -> use M as a counter to access mBeverages
	// mBeverages: order detail
	int vIdx = orders.size();
	idMap[mID] = vIdx;
	//idMap.insert({ mID, vIdx}); // WARNINGS: make_pair나 {}로 묶어서 넣어주기!!!!

	Order od;
	od.mId = mID;
	od.remainBevCount = M;
	orderSetPerBevRemain[M].insert(vIdx); // for hurry()
	// initialize beverage count array
	fill(od.bevToSupplyList, od.bevToSupplyList + MAX_NUM_BEVERAGES + 1, 0);
	fill(od.bevCompletedList, od.bevCompletedList + MAX_NUM_BEVERAGES + 1, 0);
	// fill out beverage count info
	for (int i = 0; i < M; i++)
	{
		int bevNum = mBeverages[i];
		od.bevToSupplyList[bevNum]++;
		bevQ[bevNum].push(vIdx);
	}
	orders.push_back(od);
	aliveOrders++;

	return aliveOrders;
}

void doSupply(int mBeverage, int vectorIdx)
{
	// supply beverage to current order
	orders[vectorIdx].bevToSupplyList[mBeverage]--;
	orders[vectorIdx].bevCompletedList[mBeverage]++;
	orders[vectorIdx].remainBevCount--;
	// update remaining beverage state of current order
	int remain = orders[vectorIdx].remainBevCount;
	// update set : remove from set
	orderSetPerBevRemain[remain + 1].erase(vectorIdx);
	// Check if supplied order is completed.
	if (remain <= 0)
	{
		orders[vectorIdx].isCompleted = true;
		aliveOrders--;
	}
	else
	{
		// remain exists -> add to set
		orderSetPerBevRemain[remain].insert(vectorIdx);
	}
}

int supply(int mBeverage)
{
	// Beverage #mBeverage needs to be served.
	// TODO: remove mBeverage from front order

	// if any order does not contain mBeverage : return -1
	// otherwise, return order ID
	while (!bevQ[mBeverage].empty())
	{
		int vIdx = bevQ[mBeverage].front();
		bevQ[mBeverage].pop();
		// BeverageQ may contain cancelled or completed orders' vector index.
		if (orders[vIdx].isCompleted || orders[vIdx].isCancelled)
		{
			continue;
		}
		if (orders[vIdx].bevToSupplyList[mBeverage] <= 0) continue;
		// Supply beverage #mBeverage to alive order.
		doSupply(mBeverage, vIdx);

		return orders[vIdx].mId;
	}
	return -1;
}

int cancel(int mID)
{
	// Order #mID is cancelled.
	// TODO: get beverages supplied to order #mID -> supply beverages to other orders

	// returns remaining beverages count
	// returns 0 if order is completed
	// returns -1 if order is already cancelled

	// Find vector index using order id.
	auto iter = idMap.find(mID);
	if (iter == idMap.end()) return -1;

	// order to cancel found
	int vIdx = iter->second;
	if (orders[vIdx].isCancelled) return -1;
	if (orders[vIdx].isCompleted) return 0;
	// Cancel order #mID.
	orders[vIdx].isCancelled = true;
	aliveOrders--;

	// Calculate remains to supply (return value)
	int remains = orders[vIdx].remainBevCount;
	orderSetPerBevRemain[remains].erase(vIdx);
	orders[vIdx].remainBevCount = 0; // Set to zero to exclude this order from hurry up search.

	// Distribute beverages from cancelled order to other orders
	for (int drinkNum = 1; drinkNum <= drinkTypes; drinkNum++)
	{
		// Check completed drinks
		int bevCount = orders[vIdx].bevCompletedList[drinkNum];
		//orders[vIdx].bevToSupplyList[drinkNum] = 0; // 필요할까 싶긴 한데 혹시나...
		//orders[vIdx].bevCompletedList[drinkNum] = 0;
		if (bevCount <= 0) continue;

		while (bevCount > 0 && !bevQ[drinkNum].empty())
		{
			int vIdxToSupply = bevQ[drinkNum].front();
			bevQ[drinkNum].pop();
			// Ignore cancelled order from the queue.
			if (vIdxToSupply == vIdx || orders[vIdxToSupply].isCancelled || orders[vIdxToSupply].isCompleted)
			{
				continue;
			}

			// Supply current beverage to other orders containing drink #.
			if (orders[vIdxToSupply].bevToSupplyList[drinkNum] <= 0) continue;
			doSupply(drinkNum, vIdxToSupply);
			bevCount--;
		}
	}

	return remains;
}

int getStatus(int mID)
{
	// TODO: returns remaining beverages count
	// returns 0 if order is completed
	// returns -1 if order is already cancelled

	auto iter = idMap.find(mID);
	if (iter != idMap.end())
	{
		int vIdx = iter->second;

		if (orders[vIdx].isCompleted) return 0;
		if (orders[vIdx].isCancelled) return -1;

		return orders[vIdx].remainBevCount;
	}
	return -1;
}

RESULT hurry()
{
	RESULT res;
	// Result.cnt => the number of orders to be processed quickly
	// Result.IDs => a list of orders to be processed quickly
	res.cnt = 0; // initialize to use!

	// TODO: need to find top5 urgent orders' ids (use sort or something)
	// Search from back.
	for (int remainBevCount = MAX_NUM_BEVERAGES; remainBevCount >= 1 && res.cnt < 5; remainBevCount--)
	{
		for (auto iter = orderSetPerBevRemain[remainBevCount].begin(); iter != orderSetPerBevRemain[remainBevCount].end(); iter++)
		{
			res.IDs[res.cnt] = orders[*iter].mId;
			res.cnt++;

			if (res.cnt == 5) break;
		}
	}

	return res;
}