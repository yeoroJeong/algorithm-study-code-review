#include <string>
#include <vector>
#include <sstream>
using namespace std;
int st_op,ed_op,end_time;
int cast_sec(string temp){
    string target=temp;
    string minute,second;
    stringstream ss(target);
    getline(ss,minute,':');
    getline(ss,second);
    int m=stoi(minute);
    int s=stoi(second);
    return m*60+s;
}
string cast_time(int temp){
    int s=temp%60;
    int m=temp/60;
    string minute=to_string(m);
    string second=to_string(s);
    return (m<10 ? "0":"")+to_string(m)+":"+(s<10 ? "0":"")+to_string(s);
}
int chk_op(int temp){
    int result=temp;
    if(st_op<=temp&&temp<=ed_op)
        result=ed_op;
    return result;
}
int move_pos(string command,int cur_pos){
    int result;
    if(command=="next"){
        result=(cur_pos+10>end_time ? end_time:cur_pos+10);
        result=chk_op(result);
    }
    else if(command==""){
        result=(cur_pos-10<0 ? 0:cur_pos-10);
        result=chk_op(result);
    }
    return result;
}
string solution(string video_len, string pos, string op_start, string op_end, vector<string> commands) {
    string answer = "";
    end_time=cast_sec(video_len);
    st_op=cast_sec(op_start);
    ed_op=cast_sec(op_end);
    int cur_pos=cast_sec(pos);
    cur_pos=chk_op(cur_pos);
    for(auto c : commands){
        cur_pos=move_pos(c,cur_pos);
    }
    answer=cast_time(cur_pos);
    return answer;
}