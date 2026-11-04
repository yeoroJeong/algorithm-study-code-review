/*
붕대 감기 기술
1. t초동안 붕대를 감으며 1초마다 x만큼의 체력 회복
2. t초 연속으로 붕대를 감는 데 성공하면 y만큼의 체력 추가 회복
단 최대 체력보다 커지는 것은 불가
조건
1. 기술을 쓰는 도중 몬스터에게 공격을 당하면 기술 취소, 공격을 당하는 순간에는 체력 회복 x
2. 몬스터에게 공격당해 기술이 취소 또는 기술이 끝나면 그 즉시 '붕대 감기'를 다시 사용, 연속 성공 시간이 0으로 초기화됨
3. 몬스터의 공격을 받으면 정해진 피해량 만큼 현재 체력이 줄어듦
이때 현재 체력이 0 이하가 되면 캐릭터 죽음. 더 이상 체력 회복 불가

=> 붕대감기 기술의 정보, 캐릭터가 가진 최대 체력과 몬스터의 공격 패턴 주어질 때 캐릭터가 끝까지 생존할 수 있냐
입력
bandage: 기술의 시전 시간, 1초당 회복량, 추가 회복량
health: 최대 체력
attacks: 몬스터의 공격 시간 및 피해량(공격시간, 피해량 순) => 공격시간 기준으로 정렬됨1
출력
기본: 남은 체력
체력이 0이하가 되어 죽을시 -1
*/

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> bandage, int health, vector<vector<int>> attacks) {
    int answer = -1;
    int len = attacks.size();
    
    int time = bandage[0];
    int heal = bandage[1];
    int plusheal = bandage[2];
    int curhealth = health;
    
    int cnt = 0; // 연속 성공 횟수
    int idx = 0; // 몬스터 공격 idx
    for(int i = 1; i <= attacks[len - 1][0]; i++){
        if(attacks[idx][0] == i){ // 1. 몬스터 공격 시
            cnt = 0; // 연속 공격 초기화
            curhealth -= attacks[idx][1]; // 공격
            if(curhealth <= 0){
                curhealth = -1;
                break;
            }
            idx += 1;
        }else{ // 2. 몬스터 공격 안할시
            curhealth = min(health, curhealth + heal);
            cnt++;
            if(cnt == time){
                curhealth = min(health, curhealth + plusheal);
                cnt = 0;
            }
        }
    }
    
    answer = curhealth;
    return answer;
}