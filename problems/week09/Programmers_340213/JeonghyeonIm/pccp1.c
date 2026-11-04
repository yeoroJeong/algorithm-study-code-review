/* PCCP1
동영상 재생기
기능:
10초전이동 => prev명령, 동영상의 재생 위치를 현재 위치에서 10초 전으로 이동, 현재 위치 10초 미만일 경우 영상의 처음 위치(0분 0초)
10초후이동 => next명령, 동영상의 재생 위치 현재 위치에서 10초 후로 이동, 남은 시간 10초 미만일 경우 영상의 마지막 위치(동영상길이)
오프닝 건너뛰기 => 현재 재생위치가 오프닝 구간 op_start <= x <= op_end인 경우 자동으로 오프닝 끝나는 위치로 이동

매개변수
동영상 길이 문자열 video_len
기능 수행 전 재생위치 문자열 pos
오프닝 시작 시간 문자열 op_start
오프닝 끝 시간 문자열 op_end
사용자의 입력 나타내는 1차원 문자열 배열 commands

사용자의 입력이 모두 끝난 후 동영상의 위치를 mm:ss형식으로 return하도록 완성
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// 시간 분 => 변환기
int trans(int m, int s){
    int sum = 0;
    sum += s;
    sum += m * 60;
    
    return sum;
}

// commands_len은 배열 commands의 길이입니다.
// 파라미터로 주어지는 문자열은 const로 주어집니다. 변경하려면 문자열을 복사해서 사용하세요.
char* solution(const char* video_len, const char* pos, const char* op_start, const char* op_end, const char* commands[], size_t commands_len) {
    // return 값은 malloc 등 동적 할당을 사용해주세요. 할당 길이는 상황에 맞게 변경해주세요.
    char* answer = (char*)malloc(6);
    
    int curm, curs; // 현재 시간 저장
    sscanf(pos, "%d:%d", &curm, &curs);
    int mxm, mxs; // 최대 시간 저장
    sscanf(video_len, "%d:%d", &mxm, &mxs);
    int opsm, opss; // 오프닝 시작
    sscanf(op_start, "%d:%d", &opsm, &opss);
    int opem, opes; // 오프닝 끝
    sscanf(op_end, "%d:%d", &opem, &opes);
    
    int c = trans(curm, curs);
    int s = trans(opsm, opss);
    int e = trans(opem, opes);
    
    if(c <= e && c >= s){
        curm = opem;
        curs = opes;
    }
    
    for(int i = 0; i < commands_len; i++){
        char* input = commands[i];
        if(strcmp(input, "prev") == 0){ // 이전 처리
            curs -= 10;
            if(curs < 0){
                curs += 60;
                curm -= 1;
                if(curm < 0){
                    curs = 0;
                    curm = 0;
                }
            }
        }else if(strcmp(input, "next") == 0){ // 이후 처리
            curs += 10;
            if(curs >= 60){
                curs -= 60;
                curm += 1;
            }
            
            if(trans(mxm, mxs) <= trans(curm, curs)){
                    curs = mxs;
                    curm = mxm;
            }
        }
        
        // 오프닝 처리
        int c = trans(curm, curs);
        int s = trans(opsm, opss);
        int e = trans(opem, opes);
    
        if(c <= e && c >= s){
            curm = opem;
            curs = opes;
        }
    }
    
    sprintf(answer, "%02d:%02d", curm, curs);
    
    return answer;
}