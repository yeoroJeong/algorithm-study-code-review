#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAX_N 20
#define MAX_WIDTH 500


extern void init(int N, int mWidth, int mHeight, int mIDs[], int mLengths[][MAX_WIDTH], int mUpShapes[][MAX_WIDTH]);
extern int checkStructures(int mLengths[3], int mUpShapes[3], int mDownShapes[3]);
extern int addStructures(int mLengths[3], int mUpShapes[3], int mDownShapes[3]);


#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_CHECK 3
#define CMD_POUR 4

static int mIDs[MAX_N];
static int mLengths_tanks[MAX_N][MAX_WIDTH];
static int mUpShapes_tanks[MAX_N][MAX_WIDTH];

#define MAX_WIDTH 500
#define BASE 20
#define MAX_KEY (19*20*20+19*20+20)
#define MAX_HEIGHT 1500
#include<utility>
#include<functional>
#include<cstring>
#include<queue>
#include<vector>
#include<iostream>
#include<cmath>
#include<climits>
#include<set>
using namespace std;
struct Result {
	int ID;
	int height;
	int used;
};

int n, width, height;
int IDs[MAX_N];
int lengths[MAX_N][MAX_WIDTH];
int upshape[MAX_N][MAX_WIDTH];

int affected[5];

int best_id, best_wid,best_id_idx;
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>pq;
set<pair<int, int>> savepoint[MAX_KEY];

int dp[MAX_N][MAX_HEIGHT+1];
int getKey(int idx, int start) {
	return upshape[idx][start]
		+ upshape[idx][start + 1] * BASE
		+ upshape[idx][start + 2] * BASE * BASE;
}

void init(int N, int mWidth, int mHeight, int mIDs[], int mLengths[][MAX_WIDTH], int mUpShapes[][MAX_WIDTH])
{
	n = N;
	width = mWidth;
	height = mHeight;

	for (int tk = 0; tk < MAX_KEY; tk++) {
		savepoint[tk].clear();
	}
	for (int i = 0; i < n; i++) {
		for (int j = 1; j <= height; j++) {
			dp[i][j] = 0;
		}
	}
	for (int i = 0; i < N; i++) {
		IDs[i] = mIDs[i];
		for (int wid = 0; wid < mWidth; wid++) {
			lengths[i][wid] = mLengths[i][wid];
			upshape[i][wid] = mUpShapes[i][wid];

			for (int cur_len = lengths[i][wid]+1; cur_len <= height; cur_len++) {
				dp[i][cur_len] += 1;
			}

			if (wid < mWidth - 2) {
				int temp_key = 0;
				for (int point = 0; point < 3; point++) {
					int temp_base = pow(BASE, point);
					temp_base *= mUpShapes[i][wid + point];
					temp_key += temp_base;
				}
				savepoint[temp_key].insert({ i,wid });
			}
		}
	}
	
}

int checkStructures(int mLengths[], int mUpShapes[], int mDownShapes[])
{

	best_id = INT_MAX;
	best_wid = INT_MAX;
	best_id_idx = INT_MAX;

	int cnt_possible = 0;

	int cur_key = 0;
	for (int i = 0; i < 3; i++) {
		int cur_base = pow(BASE, i);
		cur_base *= mDownShapes[i];
		cur_key += cur_base;
	}

	for (const auto&pos :savepoint[cur_key]) {

		int id_index = pos.first;
		int wid = pos.second;

		bool is_ok = true;
		int last_st = 0;
		int last_ed = 0;

		for (int idx = 0; idx < 3; idx++) {

			int before_height = lengths[id_index][wid + idx];
			int temp_height = before_height + mLengths[idx];

			if (temp_height > height) {
				is_ok = false;
				break;
			}

			//경계 판정 중요..
			if (idx != 0) {
				if (temp_height <= last_st || last_ed <= before_height) {
					is_ok = false;
					break;
				}
			}

			last_ed = temp_height;
			last_st = before_height;

		}
		if (is_ok) {
			cnt_possible += 1;
			if (IDs[id_index] < best_id) {
				best_id = IDs[id_index];
				best_id_idx = id_index;
				best_wid = wid;
			}
			else if (IDs[id_index] == best_id && wid < best_wid) {
				best_wid = wid;
			}
		}
	}
	return cnt_possible;
}

int addStructures(int mLengths[], int mUpShapes[], int mDownShapes[])
{
	int cnt = checkStructures(mLengths, mUpShapes, mDownShapes);
	if (cnt == 0) return 0;
	int insert_id = best_id_idx;
	int insert_wid = best_wid;
	int ans = IDs[insert_id] * 1000 + insert_wid;

	int affected_cnt = 0;
	for (int changed = insert_wid - 2; changed <= insert_wid + 2; changed++) {
		if (changed < 0 || changed >= width-2) continue;
		savepoint[getKey(insert_id, changed)].erase({ insert_id,changed });
		affected[affected_cnt] = changed;
		affected_cnt++;
	}

	for (int idx = 0; idx < 3; idx++) {
		int temp_wid = insert_wid + idx;
		int before_len = lengths[insert_id][temp_wid];
		upshape[insert_id][temp_wid] = mUpShapes[idx];
		lengths[insert_id][temp_wid] = lengths[insert_id][temp_wid] + mLengths[idx];
		
		for (int len = before_len + 1; len <= lengths[insert_id][temp_wid]; len++) {
			dp[insert_id][len] -= 1;
		}
	}
	for (int i = 0; i < affected_cnt; i++) {
		int cur_w = affected[i];
		savepoint[getKey(insert_id, cur_w)].insert({ insert_id,cur_w });
	}


	ans += 1;
	return ans;
}

Result pourIn(int mWater)
{
	Result ret;
	ret.ID = ret.height = ret.used = 0;
	for (int i = 0; i < n; i++) {
		int total_water = 0;
		for (int h = 2; h <= height; h++) {
			total_water += dp[i][h];

			if (total_water <= mWater && total_water >=1) {
				if (ret.ID == 0 || h > ret.height) {
					ret.ID = IDs[i];
					ret.height = h;
					ret.used = total_water;
				}
				if (h == ret.height) {
					if (total_water > ret.used) {
						ret.ID = IDs[i];
						ret.used = total_water;
					}
					else if (total_water == ret.used && IDs[i] < ret.ID) {
						ret.ID = IDs[i];
						ret.used = total_water;
					}
					
				}
			}
		}
	}
	return ret;
}


static bool run()
{
	int query_num;
	scanf("%d", &query_num);

	int ans;
	bool ok = false;

	for (int q = 0; q < query_num; q++)
	{
		int query;
		scanf("%d", &query);
		if (query == CMD_INIT)
		{
			int N, mWidth, mHeight;
			scanf("%d %d %d", &N, &mWidth, &mHeight);
			for (int i = 0; i < N; i++)
			{
				scanf("%d", &mIDs[i]);
			}
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < mWidth; j++)
					scanf("%d", &mLengths_tanks[i][j]);
			}
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < mWidth; j++)
					scanf("%d", &mUpShapes_tanks[i][j]);
			}
			init(N, mWidth, mHeight, mIDs, mLengths_tanks, mUpShapes_tanks);
			ok = true;
		}
		else if (query == CMD_CHECK)
		{
			int mLengths[3];
			int mUpShapes[3];
			int mDownShapes[3];
			for (int i = 0; i < 3; i++)
				scanf("%d", &mLengths[i]);
			for (int i = 0; i < 3; i++)
				scanf("%d", &mUpShapes[i]);
			for (int i = 0; i < 3; i++)
				scanf("%d", &mDownShapes[i]);
			int ret = checkStructures(mLengths, mUpShapes, mDownShapes);
			scanf("%d", &ans);
			if (ans != ret)
			{
				ok = false;
			}
		}
		else if (query == CMD_ADD)
		{
			int mLengths[3];
			int mUpShapes[3];
			int mDownShapes[3];
			for (int i = 0; i < 3; i++)
				scanf("%d", &mLengths[i]);
			for (int i = 0; i < 3; i++)
				scanf("%d", &mUpShapes[i]);
			for (int i = 0; i < 3; i++)
				scanf("%d", &mDownShapes[i]);
			int ret = addStructures(mLengths, mUpShapes, mDownShapes);
			scanf("%d", &ans);
			if (ans != ret)
			{
				ok = false;
			}
		}
		else if (query == CMD_POUR)
		{
			int mWater;
			scanf("%d", &mWater);
			Result ret = pourIn(mWater);

			int ans_height = 0;
			int ans_used = 0;
			scanf("%d", &ans);
			if (ans != 0) {
				scanf("%d %d", &ans_height, &ans_used);
			}
			if (ans != 0 && (ans != ret.ID || ans_height != ret.height || ans_used != ret.used))
			{
				ok = false;
			}
			else if (ans == 0 && ret.ID != 0)
			{
				ok = false;
			}
		}
	}
	return ok;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);
	int T, MARK;
	scanf("%d %d", &T, &MARK);
	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}
	return 0;
}