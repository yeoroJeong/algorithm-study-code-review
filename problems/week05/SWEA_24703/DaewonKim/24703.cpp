#define MAX_WIDTH 500
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <climits>
 
struct Result {
    int ID;         // 어항 ID
    int height;     // 물을 채운 최대 높이
    int used;       // 최대 물 높이에 사용한 물의 양
};
 
using namespace std;
 
struct FishTank
{
    int ID;
    int mWidth, mHeight;
 
    // check,add 용
    vector<int> colHeight;        // 열별 현재 쌓인 높이(바닥=0 기준)
    vector<int> colTopShape;  // 열별 현재 꼭대기 결합판 종류
 
    // pourIn 용
    multiset<int> heightSorted;   // colHeight와 항상 같은 값들을 정렬된 채로 유지(pourIn 전용)
 
    // 결합판이 (d0,d1,d2)로 정확히 맞는 시작열 저장
    // set을 사용하지 않으면 매번 최대 500개의 열을 계속 검사해야해서 시간초과 발생
    set<int> comb[4][4][4];
};
 
static vector<FishTank> Fts;
 
void init(int N, int mWidth, int mHeight, int mIDs[],
    int mLengths[][MAX_WIDTH], int mUpShapes[][MAX_WIDTH])
{
    Fts.clear();
 
    for (int i = 0; i < N; i++)
    {
        FishTank ft;
        ft.ID = mIDs[i];
        ft.mWidth = mWidth;
        ft.mHeight = mHeight;
        ft.colHeight.assign(mWidth, 0);
        ft.colTopShape.assign(mWidth, -1);
 
        for (int col = 0; col < mWidth; ++col)
        {
            ft.colHeight[col] = mLengths[i][col];       // 높이 기록
            ft.colTopShape[col] = mUpShapes[i][col];    // 결합판 종류 기록
        }
 
        ft.heightSorted.insert(ft.colHeight.begin(), ft.colHeight.end());
 
        // 결합판 조합 인덱스를 한 번에 구축(O(mWidth))
        for (int s = 0; s <= mWidth - 3; ++s)
        {
            ft.comb[ft.colTopShape[s]][ft.colTopShape[s + 1]][ft.colTopShape[s + 2]].insert(s);
        }
 
        Fts.push_back(std::move(ft));
    }
}
 
/*
    설치 가능한 구조물 조건
    1.3개의 구조물은 같은 어항에 주어진 순서대로 인접하게 설치
    2.인접한 구조물은 최소 한 셀의 면이 붙어야 함
    3.설치 후 어항 최대높이를 넘으면 안됨
    comb[][][]로 결합판이 맞는 시작열만 골라 검사하므로 매번 mWidth 전체를 검사하지 않음
*/
struct ScanResult
{
    int count;      // 설치가능 갯수->check 함수
    int bestID;     // 최적 어항 ID->add 함수
    int bestCol;    // 최적 어항의 최적 열->add 함수
};
 
ScanResult scanValidPositions(int mLengths[], int mUpShapes[], int mDownShapes[])
{
    ScanResult res;
    res.count = 0;          // 설치가능 위치 수
    res.bestID = INT_MAX;   // add에서 사용할 최소 ID
    res.bestCol = INT_MAX;  // 같은 ID일 때 최소 시작열
 
    // 모든 어항 검사
    for (int curFt = 0; curFt < (int)Fts.size(); ++curFt)
    {
        const FishTank& ft = Fts[curFt];
 
        // downShape와 현재 topShape가 일치하는 시작열만 가져옴
        // 모든 열을 검사하지 않고 comb에 저장된 후보만 검사
        const set<int>& cands = ft.comb[mDownShapes[0]][mDownShapes[1]][mDownShapes[2]];
 
        for (int stInside : cands) // set이라 이미 열 오름차순으로 순회됨
        {
            bool isTall = false;    // 어항 최대높이 초과 여부
            bool isNear = true;     // 인접 구조물의 면 접촉 여부
 
            // baseHeight:구조물 설치 시작높이,topHeight:구조물 설치 후 끝높이
            int baseHeight[3], topHeight[3];
 
            // 연속된 3개 구조물의 높이조건 검사
            for (int curSt = 0; curSt < 3; ++curSt)
            {
                int col = stInside + curSt;
 
                // 현재높이+구조물높이가 어항 최대높이를 넘으면 설치불가
                if (ft.colHeight[col] + mLengths[curSt] > ft.mHeight)
                {
                    isTall = true;
                    break;
                }
 
                baseHeight[curSt] = ft.colHeight[col];
                topHeight[curSt] = baseHeight[curSt] + mLengths[curSt];
            }
 
            if (isTall)
                continue;
 
            // 0번,1번 구조물 접촉검사:min(top)>max(base)여야 겹치는 면이 존재
            if (min(topHeight[0], topHeight[1]) <= max(baseHeight[0], baseHeight[1]))
                isNear = false;
 
            // 1번,2번 구조물도 동일하게 검사
            if (isNear && min(topHeight[1], topHeight[2]) <= max(baseHeight[1], baseHeight[2]))
                isNear = false;
 
            if (isNear)
            {
                res.count++; // 설치가능 위치 증가
 
                // add 우선순위:ID가 작을수록 우선,같은 ID면 시작열이 작을수록 우선
                if (ft.ID < res.bestID || (ft.ID == res.bestID && stInside < res.bestCol))
                {
                    res.bestID = ft.ID;
                    res.bestCol = stInside;
                }
            }
        }
    }
 
    // check는 count,add는 bestID/bestCol 사용
    return res;
}
 
int checkStructures(int mLengths[], int mUpShapes[], int mDownShapes[])
{
    return scanValidPositions(mLengths, mUpShapes, mDownShapes).count;
}
 
int addStructures(int mLengths[], int mUpShapes[], int mDownShapes[])
{
    ScanResult res = scanValidPositions(mLengths, mUpShapes, mDownShapes);
 
    if (res.count == 0)
        return 0;
 
    int id = res.bestID;
    int col = res.bestCol;
 
    for (int i = 0; i < (int)Fts.size(); ++i)
    {
        if (Fts[i].ID != id)
            continue;
 
        FishTank& ft = Fts[i];
 
        // topShape가 바뀌는 열은 col,col+1,col+2
        // 해당 열을 포함하는 3칸 window의 시작열은 [col-2,col+2]이므로 최대 5개만 갱신
        /*
            2,3,4열에 설치할 경우:
            012
            123
            234
            345
            456
            위 5개 window의 결합판 조합이 달라질 수 있음
            기존 comb에서 제거->topShape 변경->변경된 조합으로 다시 삽입
        */
        int lo = max(0, col - 2);
        int hi = min(ft.mWidth - 3, col + 2);
 
        // 1)변경 전 shape 기준으로 영향받는 시작열 제거
        for (int s = lo; s <= hi; ++s)
            ft.comb[ft.colTopShape[s]][ft.colTopShape[s + 1]][ft.colTopShape[s + 2]].erase(s); // log N
 
        // 2)실제 설치 반영
        for (int k = 0; k < 3; ++k)
        {
            int oldH = ft.colHeight[col + k];
 
            ft.heightSorted.erase(ft.heightSorted.find(oldH));  // 같은 높이 중 하나만 제거
            ft.colHeight[col + k] += mLengths[k];               // 새 구조물 높이만큼 증가
            ft.colTopShape[col + k] = mUpShapes[k];             // topShape 갱신
            ft.heightSorted.insert(ft.colHeight[col + k]);      // 정렬된 높이 동기화
        }
 
        // 3)변경 후 shape 기준으로 같은 시작열 다시 삽입
        for (int s = lo; s <= hi; ++s)
            ft.comb[ft.colTopShape[s]][ft.colTopShape[s + 1]][ft.colTopShape[s + 2]].insert(s); // log N
 
        break;
    }
 
    return id * 1000 + (col + 1);
}
 
/*
    물 채우기 우선순위
    1.더 높은 높이까지 채울 수 있는 어항
    2.같은 높이면 사용한 물의 양이 많은 어항
    3.높이와 사용량이 같으면 ID가 작은 어항
    heightSorted를 그대로 순회
*/
Result pourIn(int mWater)
{
    Result ret;
    ret.ID = ret.height = ret.used = 0;
 
    // 현재까지 가장 좋은 어항의 높이,사용량,ID
    int bestHeight = -1;
    int bestUsed = -1;
    int bestID = 0;
 
    // 모든 어항에 동일한 mWater를 부었을 때 결과 계산
    for (int ft = 0; ft < (int)Fts.size(); ++ft)
    {
        // 열높이가 정렬된 multiset.예:[5,2,7,2]->[2,2,5,7]
        const multiset<int>& h = Fts[ft].heightSorted;
 
        int mWidth = (int)h.size();
        int mHeight = Fts[ft].mHeight;
 
        auto it = h.begin();
        int curH = *it; // 현재 수면높이
        int used = 0;   // 실제 사용한 물
        int cnt = 1;    // 현재 수면까지 합류한 열 개수
 
        // 정렬된 다음 높이까지 필요한 물을 한 번에 계산
        for (int i = 1; i <= mWidth; ++i)
        {
            int nextH;
 
            // 다음 열높이를 목표로 하고,마지막에는 어항 최대높이를 목표로 함
            if (i < mWidth) { ++it; nextH = *it; }
            else nextH = mHeight; // 마지막 한 번은 mHeight까지
 
            // 현재높이에서 다음높이까지의 차이
            int gap = nextH - curH;
 
            // 같은 높이면 물을 쓰지 않고 현재 수면 그룹에 열만 추가
            if (gap <= 0) // 같은 높이(중복값)->비용 없이 합류
            {
                if (i < mWidth) cnt++;
                continue;
            }
 
            // cnt개의 열을 gap만큼 올리는 데 필요한 물
            int need = cnt * gap;
 
            // 아직 사용할 수 있는 물
            int remain = mWater - used;
 
            if (remain >= need)
            {
                // 다음 높이까지 올릴 수 있는 경우
                used += need;
                curH = nextH;
 
                // 실제 다음 열높이에 도달했으므로 해당 열도 수면 그룹에 합류
                if (i < mWidth) cnt++;
            }
            else
            {
                // 다음높이까지 못 가면 remain/cnt만큼의 완전한 높이만 상승가능
                int delta = remain / cnt; // 완전한 행만 채울 수 있으므로 정수 나눗셈
                curH += delta;
                used += delta * cnt;
                break; // 더는 못 올라감
            }
 
            // 어항 최대높이에 도달하면 종료
            if (curH >= mHeight)
            {
                curH = mHeight;
                break;
            }
        }
 
        // 물을 한 층도 올리지 못했다면 후보에서 제외
        if (used <= 0)
            continue; // 규칙 1 위반:이 어항은 후보에서 제외
 
        int id = Fts[ft].ID;
 
        // 우선순위:높이->사용량->작은 ID
        bool better =
            (curH > bestHeight) ||
            (curH == bestHeight && used > bestUsed) ||
            (curH == bestHeight && used == bestUsed && id < bestID);
 
        if (better)
        {
            bestHeight = curH;
            bestUsed = used;
            bestID = id;
        }
    }
 
    // 후보가 존재하면 최종 결과 저장
    if (bestHeight >= 0)
    {
        ret.ID = bestID;
        ret.height = bestHeight;
        ret.used = bestUsed;
    }
 
    return ret;
}