/*
물류센터 => 로봇 이용한 자동 운송 시스템
규칙
1. (r, c)와 같이 2차원 좌표로 나타낼 수 있는 n개의 포인트 존재
각 포인트는 1~n까지의 서로 다른 번호를 가짐
2. 로봇마다 정해진 운송 경로 존재
운송 경로는 m개의 포인트로 구성되고,
로봇은 첫 포인트에서 시작해 할당된 포인트를 순서대로 방문
3. 사용되는 로봇은 x대이고, 모든 로봇은 0초에 동시 출발
로봇은 1초마다 r 좌표와 c 좌표중 하나가 1만큼 감소하거나 증가한 좌표로 이동 가능
4. 다음 포인트로 이동할 때는 항상 최단 경로로 이동하며 최단 경로가 여러가지일 경우
r 좌표가 변하는 이동을 c 좌표가 변하는 이동보다 먼저
r부터 변동시킨다음에 c변동
5. 마지막 포인트에 도착한 로봇은 운송을 마치고 물류 센터를 벗어남

이동 중 같은 좌표에 로봇이 2대 이상 모인다면 충돌 가능성이 있는 위험 상황으로 판단
현재 설정대로 로봇이 움직일 때 위험한 상황이 총 몇 번 일어나는지 알고 싶음
만약 어떤 시간에 여러 좌표에서 위험 상황 발생시 그 횟수 모두 더함

위험한 상황의 횟수 return

입력
운송 포인트n개의 좌표를 담은 2차원 정수 배열 points
로봇 x대의 운송 경로를 담은 2차원 정수 배열 routes
*/

#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
using namespace std;

int solution(vector<vector<int>> points, vector<vector<int>> routes) {
    int answer = 0;
    
    int n = points.size();
    int x = routes.size();
    
    vector<vector<pair<int,int>>> r(x); 
    for(int i = 0; i < x; i++){ // i번째 로봇
        for(int j = 0; j < routes[i].size() - 1; j++){ // i번째 로봇의 루트
            int start = routes[i][j] - 1;
            int end = routes[i][j + 1] - 1;
            
            int sr = points[start][0];
            int sc = points[start][1];
            int er = points[end][0];
            int ec = points[end][1];
            
            while(sr != er){ // r좌표 이동
                r[i].push_back({sr, sc});
                if(sr < er){
                    sr += 1;
                }else if(sr > er){
                    sr -= 1;
                }
            }
            while(sc != ec){ // c좌표 이동
                r[i].push_back({sr, sc});
                if(sc < ec){
                    sc += 1;
                }else if(sc > ec){
                    sc -= 1;
                }
            }
            if(j == routes[i].size() - 2){
                r[i].push_back({sr, sc}); // 마지막 물류창고 좌표까지 추가
            }
        }
    }
    
    int mx = 0;
    for(auto& path : r){ // 경로 최대시간
        mx = max(mx, (int)path.size());
    }
    
    for(int t = 0; t < mx; t++){ // 시간당
        map<pair<int, int>, int> cnt;
        
        for (int i = 0; i < r.size(); i++){ // 로봇이
            if (t >= r[i].size()){ // 이미 도착한 로봇이면 처리 제외
                continue;
            }
            cnt[r[i][t]]++;
        }
        
        for (auto& [k, v] : cnt){
            if(v >= 2){
                answer++;
            }
        }
    }
    
    return answer;
}

