/*
아날로그시계
시침 0~23
분침 0~59
초침 0~59

=>
초침속도는 초당 6도
분침속도는 초당 0.1도
시침속도는 초당 1/120도
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int count(int T) {
    int res = (59 * T / 3600) + (719 * T / 43200);
    if (T >= 43200) res--; // 12시 정각 중복 제거
    return res;
}


int solution(int h1, int m1, int s1, int h2, int m2, int s2) {
    int answer = 0;
    
    int start = h1 * 3600 + m1 * 60 + s1;
    int end = h2 * 3600 + m2 * 60 + s2;
    
    answer = count(end) - count(start);
    
    if (start * 59 % 3600 == 0 || start * 719 % 43200 == 0) {
        answer++;
    }
    
    
    return answer;
}