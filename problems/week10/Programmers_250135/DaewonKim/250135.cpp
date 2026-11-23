#include <string>
#include <vector>
#include <iostream>

using namespace std;
/*
    초침이 시침 or 분침과 겹칠 때 마다 알림이 울림.

    h1, m1, s1 -> 시작  시 분 초
    h2, m2, s2 -> 끝    시 분 초

    출력 : 알람이 울리는 횟수

    시침 분침 초침이 모두 겹칠 때 -> 알람은 1번만

    현재 초침이 어딧는지 알아야 함.
    시침의 경우-> 초나 분처럼 변환을 해줘야 함. 1시인 경우 -> 5분이나 5초와 동일 
    시침 * 5를 해줘야 함.
    
    for나 while의 주기 설정을 어떻게? 
    종료 -> h2 - h1 , m2 - m1 , s2 -s1 
    -> 전부 초로 계산
*/
int toSecond(int h, int m, int s)
{
    return h * 3600 + m * 60 + s;
}

int getSecondHand(int t)
{
    return (t * 720) % 43200;
}

int getMinuteHand(int t)
{
    return (t * 12) % 43200;
}

int getHourHand(int t)
{
    return t % 43200;
}

bool isOverlapNow(int t)
{
    int second = getSecondHand(t);
    int minute = getMinuteHand(t);
    int hour = getHourHand(t);

    return second == minute || second == hour;
}

int solution(int h1, int m1, int s1, int h2, int m2, int s2) 
{
    int answer = 0;

    int start = toSecond(h1, m1, s1);
    int end = toSecond(h2, m2, s2);

    // 시작 시각에 이미 겹쳐 있으면 1번 카운트
    if(isOverlapNow(start))
    {
        answer++;
    }

    for(int t = start; t < end; ++t)
    {
        int secondCur = getSecondHand(t);
        int minuteCur = getMinuteHand(t);
        int hourCur = getHourHand(t);

        int secondNext = getSecondHand(t + 1);
        int minuteNext = getMinuteHand(t + 1);
        int hourNext = getHourHand(t + 1);

        // 초침이 59초 -> 00초로 넘어가면 값이 0이 되므로,
        // 비교를 위해 한 바퀴 끝인 43200으로 처리
        if(secondNext == 0)
        {
            secondNext = 43200;
        }

        bool crossMinute = false;
        bool crossHour = false;

        // 초침이 분침을 따라잡거나 추월한 경우
        if(secondCur < minuteCur && secondNext >= minuteNext)
        {
            crossMinute = true;
        }

        // 초침이 시침을 따라잡거나 추월한 경우
        if(secondCur < hourCur && secondNext >= hourNext)
        {
            crossHour = true;
        }

        if(crossMinute)
        {
            answer++;
        }

        if(crossHour)
        {
            answer++;
        }

        // 시침, 분침, 초침이 동시에 겹치는 경우는 알람 1번만
        // 대표적으로 12:00:00
        if(crossMinute && crossHour && (t + 1) % 43200 == 0)
        {
            answer--;
        }
    }

    return answer;
}