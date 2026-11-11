#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


int toSeconds(string strTime)
{
    int sepPos = strTime.find(':', 0);
    int numMM = stoi(strTime.substr(0, sepPos));
    int numSS = stoi(strTime.substr(sepPos + 1));

    int time = 60 * numMM + numSS;
    return time;
}

// 오프닝 범위 내에 있을 때만 오프닝 끝을 리턴
int skipOpening(int time, int opStart, int opEnd)
{
    if (time <= opEnd && time >= opStart)
    {
        return opEnd;
    }
    return time;
}


string solution(string video_len, string pos, string op_start, string op_end, vector<string> commands) {
    string answer = "";

    int videoLenNum = toSeconds(video_len);
    int posNum = toSeconds(pos);
    int opStartNum = toSeconds(op_start);
    int opEndNum = toSeconds(op_end);

    // 현재 오프닝 범위이면 오프닝 스킵
    posNum = skipOpening(posNum, opStartNum, opEndNum);
    for (auto c : commands)
    {
        if (c == "next")
        {
            posNum = min(videoLenNum, posNum + 10);
        }
        else if (c == "prev")
        {
            posNum = max(0, posNum - 10);
        }
        posNum = skipOpening(posNum, opStartNum, opEndNum);
    }
    int minute = posNum / 60;
    int second = posNum % 60;
    string mm = minute < 10 ? "0" + to_string(minute) : to_string(minute);
    string ss = second < 10 ? "0" + to_string(second) : to_string(second);
    answer = mm + ":" + ss;
    return answer;
}