// 주문 순서를 보장하기 위해 vector를 사용함
// vector에서 매번 찾는 게 너무 오래걸릴 것 같아서 unordered_map<mID, vector내 Order의 인덱스> 사용
// unordered_map의 탐색 시간은 O(1)이므로 필요한 주문을 빨리 찾을 수 있지 않을까 싶어 사용함
// hurry 주문을 찾을 때 남은 주문 수가 같으면 주문 순서 상 앞선 것을 먼저 출력하기 위해 vector<Order> orders 복제본에 stable_sort를 사용함 (시간 복잡도 NlogN)


#include <vector>
#include <algorithm>
#include <unordered_map>
#define MAX_NUM_BEVERAGES       (10)
#define MAX_MID 1000000000

using namespace std;
struct RESULT
{
	int cnt;
	int IDs[5];
};

struct Order
{
	int mId, remainBevCounts;
	// beverage #1's count will be stored in bevCountList[1]
	int bevToSupplyList[MAX_NUM_BEVERAGES + 1];

	int bevCompletedList[MAX_NUM_BEVERAGES + 1];

	bool isCancelled = false;
	bool isCompleted = false;
};

// use drinkTypes as a counter to access Order.bevToSupplyList or Order.bevCompletedList 
int drinkTypes, aliveOrders;
// to keep the order of orders, vector is used.
vector<Order> orders;
// find the 'vector index' of an order using key 'mId'
unordered_map<int, int> idMap;

void init(int N)
{
	// drink 종류 수 N // min 3 ~ max 10
	drinkTypes = N;
	aliveOrders = 0;
	orders.clear();
	idMap.clear();
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
	od.remainBevCounts = M;
	// initialize beverage count array
	fill(od.bevToSupplyList, od.bevToSupplyList + MAX_NUM_BEVERAGES + 1, 0);
	fill(od.bevCompletedList, od.bevCompletedList + MAX_NUM_BEVERAGES + 1, 0);
	// fill out beverage count info
	for (int i = 0; i < M; i++)
	{
		int bevNum = mBeverages[i];
		od.bevToSupplyList[bevNum]++;
	}
	orders.push_back(od);
	aliveOrders++;

	return aliveOrders;
}

int supply(int mBeverage)
{
	// mBeverage번 음료 제조됨
	// todo: remove mBeverage from front order

	// if any order does not contain mBeverage : return -1
	// otherwise, return order ID
	int orderId = -1;
	int vIdx = -1;
	for (int i = 0; i < orders.size(); i++)
	{
		if (orders[i].isCancelled || orders[i].isCompleted) continue;
		if (orders[i].bevToSupplyList[mBeverage] > 0)
		{
			orderId = orders[i].mId;
			vIdx = i;
			orders[i].bevToSupplyList[mBeverage]--;
			orders[i].bevCompletedList[mBeverage]++;
			orders[i].remainBevCounts--;
			break;
		}
	}
	if (vIdx == -1) return -1;

	// check if order is completed
	//bool isComplete = orders[vIdx].remainBevCounts == 0;
	//for (int i = 1; i <= drinkTypes; i++)
	//{
	//	if (orders[vIdx].bevToSupplyList[i] > 0)
	//	{
	//		isComplete = false;
	//	}
	//}
	//if (isComplete)
	//{
	//	orders[vIdx].isCompleted = true;
	//}
	if (orders[vIdx].remainBevCounts == 0)
	{
		orders[vIdx].isCompleted = true;
		aliveOrders--;
	}

	return orderId;
}

int cancel(int mID)
{
	// order is cancelled
	// restore beverages from order #mID -> remove beverages from other orders

	// returns remaining beverages count
	// returns 0 if order is completed
	// returns -1 if order is already cancelled

	//int orderIdx = -1;
	//for (int i = 0; i < orders.size(); i++)
	//{
	//	if (mID == orders[i].mId)
	//	{
	//		curOd = orders[i];
	//		break;
	//	}
	//}

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
	int remains = orders[vIdx].remainBevCounts;
	orders[vIdx].remainBevCounts = 0; // Set to zero to exclude this order from hurry up search.

	// Distribute drinks from cancelled order to other orders
	for (int i = 1; i <= drinkTypes; i++)
	{
		orders[vIdx].bevToSupplyList[i] = 0; // 필요할까 싶긴 한데 혹시나...

		// Check completed drinks
		int bevCount = orders[vIdx].bevCompletedList[i];
		if (bevCount <= 0) continue;
		// Supply completed beverages of cancelled order to other alive orders
		for (Order& od : orders)
		{
			if (od.mId == mID || od.isCancelled || od.isCompleted) continue;
			if (od.bevToSupplyList[i] == 0) continue;
			int curCount = min(bevCount, od.bevToSupplyList[i]);

			// update order info
			od.bevToSupplyList[i] -= curCount;
			od.bevCompletedList[i] += curCount;
			// update remaining beverage counts
			od.remainBevCounts -= curCount;
			bevCount -= curCount;

			// Check if order is complted.
			if (od.remainBevCounts == 0)
			{
				od.isCompleted = true;
				aliveOrders--;
			}
			if (bevCount == 0) break;
		}

	}

	return remains;
}

int getStatus(int mID)
{
	// returns remaining beverages count
	// returns 0 if order is completed
	// returns -1 if order is already cancelled

	auto iter = idMap.find(mID);
	if (iter != idMap.end())
	{
		int vIdx = iter->second;

		if (orders[vIdx].isCompleted) return 0;
		if (orders[vIdx].isCancelled) return -1;

		return orders[vIdx].remainBevCounts;
	}
	return -1;
}

RESULT hurry()
{
	RESULT res;
	// Result.cnt => the number of orders to be processed quickly
	// Result.IDs => a list of orders to be processed quickly

	// todo: find orders with the largest remaining beverage counts
	//       orders에서 남은 음료 개수가 가장 많은 주문 찾기
	// if remaining bevCount is equal, print out first-come order
	//vector<Order> hurryUp = orders;
	//stable_sort(hurryUp.begin(), hurryUp.end(), compare);
	//int count = 0;
	//for (int i = 0; i < 5; i++)
	//{
	//	if (i == hurryUp.size())
	//	{
	//		break;
	//	}
	//	res.IDs[i] = hurryUp[i].mId;
	//	count++;
	//}

	// pointer copy >>> faster!
	vector<const Order*> orderPtrs;
	orderPtrs.reserve(orders.size());
	// WARNINGS: 참조자 &로 가져와야 원본 주소 가져올 수 있음
	for (const Order& od : orders)
	{
		if (od.isCancelled || od.isCompleted) continue;
		// get pointers of alive orders
		orderPtrs.push_back(&od);
	}
	int count = min(5, (int)orderPtrs.size());
	partial_sort(orderPtrs.begin(), orderPtrs.begin() + count, orderPtrs.end(),
		[](const Order* a, const Order* b) {
			if (a->remainBevCounts != b->remainBevCounts)
				return a->remainBevCounts > b->remainBevCounts;

			return a < b;
		}
	);
	for (int i = 0; i < count; i++)
	{
		res.IDs[i] = orderPtrs[i]->mId;
	}
	res.cnt = count;

	return res;
}