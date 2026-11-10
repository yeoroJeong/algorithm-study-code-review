#include <string>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;
/*
    t초 동안 붕대감기
    1초마다 x만큼 회복
    t초를 다 감으면 y만큼 체력 추가로 회복

    기술 사용 중 공격 받 -> 기술 취소
    공격 당하는 순간 체력 회복 X
    공격 당해 기술 취소 or 기술이 끝나면 바로 붕대 감기 다시 사용 -> 연속 시간 0
    
    bandage[0] = t
    bandage[1] = x
    bandage[2] = y
    health = 최대체력

    attacks - 공격 시간을 기준으로 오름차순 정렬됨.
    attacks[0] = 공격시간
    attacks[1] = 피해량


    출력: 모든 공격이 끝난 직후 남은 체력
    죽으면 -1
    시간 복잡도 : O(n^2) 가능
*/
int solution(vector<int> bandage, int health, vector<vector<int>> attacks) {
    int answer = 0;
    // 진행 시간
    int curTime = 0;

    int t = bandage[0];     // 붕대 감기 시간
    int x = bandage[1];     // 1초 당 체력 회복
    int y = bandage[2];     // 추가 체력 회복
    int curHealth = health; // 현재 체력량
    int skillTime = 0;       // 공격받은 시간 체크
    queue<pair<int,int>> q;
    for(auto& it : attacks)
    {
        q.push({it[0], it[1]});
    }

    while(!q.empty())
    {
        auto it = q.front();
        // 공격시간이 아직 아니라면
        if(curTime < it.first)
        {
            curHealth += x;
            skillTime++;
        }
        else if(curTime == it.first)
        {
            curHealth -= it.second;
            skillTime = 0;
            q.pop();
        }
        if(skillTime == t)
        {
            curHealth += y;
            skillTime = 0;
        }
        
        // 최대 체력 제한
        if(curHealth >= health)
            curHealth = health;

        if(curHealth <= 0)
        {
            answer = -1;
            break;
        }
        answer = curHealth;
        curTime++;
    }
    
    return answer;
}