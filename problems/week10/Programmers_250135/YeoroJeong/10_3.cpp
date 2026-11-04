#include <string>
#include <vector>
#include <iostream>
using namespace std;
int calc(int t){
    
    return (
        59*t/3600+
        719*t/(3600*12)-
        t/(3600*12)
    );
}
int solution(int h1, int m1, int s1, int h2, int m2, int s2) {
    int answer = 0;
    int t1=h1*3600+m1*60+s1;
    int t2=h2*3600+m2*60+s2;
    answer=calc(t2)-calc(t1);
    if(59*t1%3600==0||719*t1%(3600*12)==0) answer++;
    
    return answer;
}