#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT            (100)
#define CMD_ORDER           (200)
#define CMD_SUPPLY          (300)
#define CMD_CANCEL          (400)
#define CMD_GET_STATUS      (500)
#define CMD_HURRY           (600)



extern void init(int N);
extern int order(int mID, int M, int mBeverages[]);
extern int supply(int mBeverage);
extern int cancel(int mID);
extern int getStatus(int mID);

#define MAX_NUM_BEVERAGES       (10)

#include<vector>
#include<cstring>
#include<algorithm>
#include<deque>
#include<unordered_map>
#include<set>
using namespace std;
struct RESULT
{
	int cnt;
	int IDs[5];
};

struct Ord
{
	int seq;
	vector<int> order_list;
	vector<int> get_list;
};
deque<int>coffees[11] ;
set<pair<int,int>>remain_order[11];
unordered_map<int, Ord>orders;
unordered_map<int, int>completed_orders;
vector<int>temp;
int n;
int temp_seq;
void init(int N)
{
	n = N;
	temp_seq = 1;
	
	for (int i = 1; i <= 10; i++) {
		coffees[i].clear();
		remain_order[i].clear();
	}
	orders.clear();
	completed_orders.clear();
	return;
}

int order(int mID, int M, int mBeverages[])
{
	temp.clear();
	for (int i = 0; i < M; i++) {
		int whatcof = mBeverages[i];
		temp.push_back(whatcof);
		coffees[whatcof].push_back(mID);
	}
	orders[mID] = {temp_seq,temp};
	remain_order[temp.size()].insert({ temp_seq,mID });

	temp_seq++;
	return orders.size();
}

int supply(int mBeverage)
{
	if (coffees[mBeverage].empty()) {
		return -1;
	}
	int cur_ord = coffees[mBeverage].front();
	coffees[mBeverage].pop_front();

	auto it = orders.find(cur_ord);
	if (it != orders.end()) {
		Ord& cur = it->second;

		int before_size = cur.order_list.size();

		auto coffe_it = find(cur.order_list.begin(), cur.order_list.end(), mBeverage);
		if (coffe_it != cur.order_list.end()) {
			cur.order_list.erase(coffe_it);
			int cur_seq = cur.seq;
			remain_order[before_size].erase({cur_seq ,cur_ord });
			remain_order[before_size - 1].insert({ cur_seq,cur_ord });

			cur.get_list.push_back(mBeverage);
		}

		if (cur.order_list.empty()) {
			orders.erase(it);
			completed_orders[cur_ord] = 0;
		}
	}


	return cur_ord;
}

int cancel(int mID)
{
	auto it = completed_orders.find(mID);
	if (it != completed_orders.end()) {
		return it->second;
	}
	Ord &point = orders[mID];
	for (int i = 1; i <= n; i++) {
		coffees[i].erase(remove(coffees[i].begin(), coffees[i].end(), mID), coffees[i].end());
	}
	for (int i = 1; i <= 10; i++) {
		
		remain_order[i].erase({ point.seq,mID });
	}

	int ans = point.order_list.size();


	for (int i = 0; i < point.get_list.size(); i++) {
		supply(point.get_list[i]);
	}
	completed_orders[mID] = -1;
	orders.erase(mID);


	return ans;
}

int getStatus(int mID)
{
	auto it = completed_orders.find(mID);
	if (it != completed_orders.end()) {
		return completed_orders[mID];
	}
	
	return orders[mID].order_list.size();
}

RESULT hurry()
{
	RESULT res;
	int idx = 0;
	int cnt = 0;
	for(int i=10;i>=1;i--){
		if (remain_order[i].empty()) continue;
		for (auto cur : remain_order[i]) {
			if (idx == 5) break;
			res.IDs[idx] = cur.second;
			cnt++;
			idx++;
		}
		if (idx == 5) break;
	}
	res.cnt = cnt;
	return res;
}

static bool run()
{
	int Q, N, M;
	int mID, mBeverage;

	int mBeverages[MAX_NUM_BEVERAGES];

	int ret = -1, ans, cnt;

	RESULT res;

	scanf("%d", &Q);

	bool okay = false;

	for (int q = 0; q < Q; ++q)
	{
		int cmd;
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d", &N);
			init(N);
			okay = true;
			break;
		case CMD_ORDER:
			scanf("%d %d", &mID, &M);
			for (int i = 0; i < M; ++i)
				scanf("%d", &mBeverages[i]);
			ret = order(mID, M, mBeverages);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_SUPPLY:
			scanf("%d", &mBeverage);
			ret = supply(mBeverage);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_CANCEL:
			scanf("%d", &mID);
			ret = cancel(mID);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_GET_STATUS:
			scanf("%d", &mID);
			ret = getStatus(mID);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_HURRY:
			res = hurry();
			scanf("%d", &cnt);
			if (res.cnt != cnt)
				okay = false;
			for (int i = 0; i < cnt; ++i)
			{
				scanf("%d", &ans);
				if (res.IDs[i] != ans)
					okay = false;
			}
			break;
		default:
			okay = false;
			break;
		}
	}

	return okay;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int TC, MARK;

	scanf("%d %d", &TC, &MARK);
	for (int tc = 1; tc <= TC; ++tc)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}