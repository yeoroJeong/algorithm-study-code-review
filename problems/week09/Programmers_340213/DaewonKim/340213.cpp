#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int toSec(string time)
{
    int mm = stoi(time.substr(0, 2));
    int ss = stoi(time.substr(3, 2));

    return mm * 60 + ss;
}

string toTime(int time)
{
    int mm = time / 60;
    int ss = time % 60;

    string result = "";

    if(mm < 10) result += "0";
    result += to_string(mm);

    result += ":";

    if(ss < 10) result += "0";
    result += to_string(ss);

    return result;
}

string solution(string video_len,string pos,string op_start,string op_end,vector<string> commands)
{
    int videoLen = toSec(video_len);
    int cur = toSec(pos);
    int opStart = toSec(op_start);
    int opEnd = toSec(op_end);

    // 명령 실행 전에 현재 위치 검사
    if(opStart <= cur && cur <= opEnd)
    {
        cur = opEnd;
    }

    for(string command : commands)
    {
        if(command == "prev")
        {
            cur = max(0, cur - 10);
        }
        else if(command == "next")
        {
            cur = min(videoLen, cur + 10);
        }

        // 명령 실행 후 오프닝 검사
        if(opStart <= cur && cur <= opEnd)
        {
            cur = opEnd;
        }
    }

    return toTime(cur);
}