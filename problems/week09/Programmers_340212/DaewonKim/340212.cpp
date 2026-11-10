#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int n = 0;
long long maxLevel = 0;
long long minLevel = 0;

// 난이도, 소요 시간은 모두 양의 정수며, 숙련도도 양의 정수
// diff > level
// diff-level만큼 틀린다. + time_cur만큼의 시간을 사용한다.
// (diff-level) * (time_cur + time_prev) + time_cur

void binarySearch(int leftIdx, int rightIdx, vector<long long>& levels,
    vector<int>& diffs, vector<int>& times, long long &limit)
{
    if(leftIdx > rightIdx)
        return;
    
    long long midIdx = (leftIdx + rightIdx) /2;
    long long curLevel = levels[midIdx];
    long long totalTime =0;
    
    for(int i=0; i<n; ++i)
    {
        long long prevTime = 0;
        if(i > 0)
            prevTime = times[i-1];
        long long curDiff = diffs[i];
        long long curTime = times[i];
        
        if(curLevel >= curDiff)
            totalTime += curTime;
        else if (curLevel < curDiff)
        {
            // (diff-level) * (time_cur + time_prev) + time_cur
            totalTime += (curTime + ((curTime+prevTime) * (curDiff-curLevel)) );
        }
        
    }
    // 현재 수행한 레벨이 시간 내에 들어온다 -> 더 작은 레벨을 계산해본다.
    if(totalTime <= limit)
    {
        // 시간 안에 들어왔는데 최소 레벨보다 현재 레벨이 더 작으면.
        if(minLevel > curLevel)
        {
            minLevel = curLevel;
        }
        binarySearch(leftIdx, midIdx-1, levels, diffs, times, limit);
        
    }
    // totalTime > limit 시간 안에 수행 불가. -> 큰 레벨을 수행해본다
    else 
    {
        binarySearch(midIdx+1, rightIdx,levels, diffs, times, limit);
    }
    
}

int solution(vector<int> diffs, vector<int> times, long long limit) {
    int answer = 0;
    n = (int)diffs.size();      // 퍼즐 n개
    maxLevel = *max_element(diffs.begin(),diffs.end());
    minLevel = maxLevel;      // 초기 최소 숙련도는 최상으로 설정.

    // 현재 가능한 maxLevel을 기준으로
    // 이진 탐색으로 찾는다.
    vector<long long> levels (maxLevel+1, 0);
    for(int i=0; i<(int)levels.size(); ++i)
    {
        levels[i] = i;
    }
    
    long long leftIdx = 1;
    long long rightIdx = (int)levels.size()-1;
    binarySearch(leftIdx, rightIdx, levels, diffs, times, limit);
    answer = minLevel;
        
    return answer;
}