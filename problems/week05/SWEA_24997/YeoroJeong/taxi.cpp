#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT			(100)
#define CMD_PICKUP			(200)
#define CMD_RESET			(300)
#define CMD_GET_BEST		(400)

#define MAX_M				(2000)

extern void init(int N, int M, int L, int mXs[], int mYs[]);
extern int pickup(int mSX, int mSY, int mEX, int mEY);
extern void getBest(int mNos[]);



static long long mSeed;
static int pseudo_rand()
{
	mSeed = (mSeed * 1103515245LL + 12345) % 2147483647;
	return (int)(mSeed >> 16);
}

static int mXs[MAX_M];
static int mYs[MAX_M];

#include<vector>
#include<algorithm>
#include<unordered_map>
#include<queue>
#include<set>
#include<cmath>
#include<algorithm>
#include<climits>
#include<iostream>
using namespace std;

struct Result
{
	int mX, mY;
	int mMoveDistance;
	int mRideDistance;
};
struct Taxi
{
	int x;
	int y;
	int move_alone;
	int move_cust;
};

int n, m, l;
unordered_map<int, Taxi>taxi_list;
set<pair<int, int>>taxi_rank;
set<int>area[10][10];
int dx[9] = { 0,0,0,1,-1,1,-1,1,-1 };
int dy[9] = { 0,1,-1,0,0,1,-1,-1,1 };

void init(int N, int M, int L, int mXs[], int mYs[])
{
	n = N;
	m = M;
	l = L;
	taxi_list.clear();
	taxi_rank.clear();
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			area[i][j].clear();
		}
	}


	for (int i = 1; i <= M; i++) {
		taxi_list[i] = { mXs[i-1],mYs[i-1],0,0 };
		taxi_rank.insert({ 0,i });

		int ar_x = mXs[i - 1] / l;
		int ar_y = mYs[i - 1] / l;
		area[ar_x][ar_y].insert(i);
	}
	return;
}

int pickup(int mSX, int mSY, int mEX, int mEY)
{
	
	int temp_dist = l+1;
	int temp_taxi_id = INT_MAX;
	int ride_dist = abs(mSX - mEX) + abs(mSY - mEY);



	for (int i = 0; i < 9; i++) {
		int area_sx = mSX / l;
		int area_sy = mSY / l;
		area_sx += dx[i];
		area_sy += dy[i];
		if (area_sx < 0 || area_sx >= 10 || area_sy < 0 || area_sy >= 10) continue;
		for (auto &temp : area[area_sx][area_sy]) {
			Taxi& taxi = taxi_list[temp];
			int taxi_x = taxi.x;
			int taxi_y = taxi.y;
			int taxi_id = temp;
			int dist = abs(taxi_x - mSX) + abs(taxi_y - mSY);
			if (dist < temp_dist) {
				temp_dist = dist;
				temp_taxi_id = taxi_id;
			}
			else if (dist <= l && dist == temp_dist && taxi_id < temp_taxi_id) {
				temp_dist = dist;
				temp_taxi_id = taxi_id;
			}
		}
	}
	
	if (temp_taxi_id == INT_MAX) return -1;
	Taxi& called = taxi_list[temp_taxi_id];
	area[called.x / l][called.y / l].erase(temp_taxi_id);

	called.x = mEX;
	called.y = mEY;
	area[called.x / l][called.y / l].insert(temp_taxi_id);
	auto cur_rank = taxi_rank.find({ -called.move_cust,temp_taxi_id });

	if (cur_rank != taxi_rank.end()) {
		taxi_rank.erase(cur_rank);
	}


	called.move_alone += temp_dist;
	called.move_cust += ride_dist;
	taxi_rank.insert({ -(called.move_cust),temp_taxi_id });

	return temp_taxi_id;
}

Result reset(int mNo)
{
	Result res;

	res.mX = res.mY = res.mMoveDistance = res.mRideDistance = -1;
	
	Taxi& temp = taxi_list[mNo];
	int before_ride = temp.move_cust;
	taxi_rank.erase({ -before_ride,mNo });
	taxi_rank.insert({ 0,mNo });
	res.mX = temp.x;
	res.mY = temp.y;
	res.mMoveDistance = temp.move_alone+temp.move_cust;
	temp.move_alone = 0;
	res.mRideDistance = temp.move_cust;
	temp.move_cust = 0;
	//cout << mNo << " " <<res.mX<<" "<<res.mY<<" "<<res.mMoveDistance<<" " << res.mRideDistance << "\n";

	return res;
}

void getBest(int mNos[])
{
	int i = 0;
	for (auto [dist, idx] : taxi_rank) {
		if (i> 4) break;
		mNos[i] = idx;
		//cout << idx << " ";
		i++;
	}
	//cout << "\n";
	return;
}



static bool run()
{
	int Q, S;
	int N, M, L, mNo;
	int mSX, mSY, mEX, mEY;
	int ret = -1, ans;

	Result res;
	int x, y, mdist, rdist;

	int mNos[5];

	scanf("%d %d", &Q, &S);

	mSeed = (long long)S;

	bool okay = false;

	for (int q = 0; q < Q; ++q)
	{
		int cmd;
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d %d", &N, &M);
			L = N / 10;
			for (int i = 0; i < M; ++i)
			{
				mXs[i] = pseudo_rand() % N;
				mYs[i] = pseudo_rand() % N;
			}
			init(N, M, L, mXs, mYs);
			okay = true;
			break;
		case CMD_PICKUP:
			do
			{
				mSX = pseudo_rand() % N;
				mSY = pseudo_rand() % N;
				mEX = pseudo_rand() % N;
				mEY = pseudo_rand() % N;
			} while (mSX == mEX && mSY == mEY);
			ret = pickup(mSX, mSY, mEX, mEY);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_RESET:
			scanf("%d", &mNo);
			res = reset(mNo);
			scanf("%d %d %d %d", &x, &y, &mdist, &rdist);
			if (res.mX != x || res.mY != y || res.mMoveDistance != mdist || res.mRideDistance != rdist)
				okay = false;
			break;
		case CMD_GET_BEST:
			getBest(mNos);
			for (int i = 0; i < 5; ++i)
			{
				scanf("%d", &ans);
				if (mNos[i] != ans)
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