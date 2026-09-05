#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#define MAX_M				(2000)
struct Result
{
	int mX, mY;
	int mMoveDistance;
	int mRideDistance;
};

// taxi number 1~M, (100 <= M <= 2000)
// city size N (10의 배수)
// distance L = N/10
// from the customer position, the nearest taxi (distance under L) will be called

int distL;
int citySize, taxiCnts;
Result taxiInfo[MAX_M + 1]; // taxi 번호로 접근
Result* taxiInfoPointer[MAX_M + 1];

void init(int N, int M, int L, int mXs[], int mYs[])
{
	distL = L;
	citySize = N;
	taxiCnts = M;

	for (int i = 0; i < M; i++)
	{
		// Fill in taxi infos.
		Result r;
		r.mX = mXs[i];
		r.mY = mYs[i];
		r.mMoveDistance = 0;
		r.mRideDistance = 0;
		taxiInfo[i + 1] = r;

		// Get pointers
		taxiInfoPointer[i + 1] = &taxiInfo[i + 1];
	}

	return;
}
int pickup(int mSX, int mSY, int mEX, int mEY)
{
	int taxiNum = -1, shortestDist = distL + 1;
	for (int i = 1; i <= taxiCnts; i++)
	{
		int taxiPosX = taxiInfo[i].mX;
		int taxiPosY = taxiInfo[i].mY;

		int dist = std::abs(mSX - taxiPosX) + std::abs(mSY - taxiPosY);
		if (dist <= distL)
		{
			// able to call a taxi
			// i is current taxi number
			if (dist < shortestDist)
			{
				shortestDist = dist;
				taxiNum = i;
			}
			else if (dist == shortestDist && i < taxiNum)
			{
				// compare taxi number if distance is same
				// smaller taxi num will be used
				taxiNum = i;
			}
		}

	}
	if (taxiNum > -1)
	{
		// Get move & ride distance.
		int prevX = taxiInfo[taxiNum].mX;
		int prevY = taxiInfo[taxiNum].mY;
		// Ride: From customer pickup to end destination.
		int ride = std::abs(mSX - mEX) + std::abs(mSY - mEY);
		// Move: From taxi start position to end destination.
		int move = ride + std::abs(prevX - mSX) + std::abs(prevY - mSY);
		taxiInfo[taxiNum].mMoveDistance += move;
		taxiInfo[taxiNum].mRideDistance += ride;
		taxiInfo[taxiNum].mX = mEX;
		taxiInfo[taxiNum].mY = mEY;
	}

	return taxiNum;
	//return -1;
}

Result reset(int mNo)
{
	Result res;
	res.mX = taxiInfo[mNo].mX;
	res.mY = taxiInfo[mNo].mY;
	res.mMoveDistance = taxiInfo[mNo].mMoveDistance;
	res.mRideDistance = taxiInfo[mNo].mRideDistance;
	taxiInfo[mNo].mMoveDistance = 0;
	taxiInfo[mNo].mRideDistance = 0;

	return res;
}

bool compare(const Result* a, const Result* b)
{
	if (a->mRideDistance != b->mRideDistance)
		return a->mRideDistance > b->mRideDistance;

	// Get taxi number using pointer.
	// pointer 주소 차이로 택시 번호 얻음
	int taxiA = a - taxiInfo;
	int taxiB = b - taxiInfo;
	return taxiA < taxiB;
}

//mNos : 손님을 태우고 이동한 총 거리가 가장 큰 택시들의 번호를 저장할 배열
void getBest(int mNos[])
{
	// 원본 taxiInfo 배열은 택시 번호 순서로 놓아두기 위해서 주소값의 배열을 정렬함
	// taxiInfoPointer index starts from 1
	std::partial_sort(taxiInfoPointer + 1, taxiInfoPointer + 6, taxiInfoPointer + taxiCnts + 1, compare);

	for (int i = 0; i < 5; i++)
	{
		Result* rP = taxiInfoPointer[i + 1];
		mNos[i] = rP - taxiInfo; // pointer 주소 차이로 택시 번호 얻음
	}
	return;
}