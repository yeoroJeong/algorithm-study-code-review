#include <string>
#include <vector>
#include <sstream>
#include <set>
#include<algorithm>
using namespace std;
vector<string>express;

int valid(string left){
    int result=0;
    if (left=="X")
        return 0;
    for(int i=0;i<left.length();i++){
        int temp=left[i]-'0';
        result=max(result,temp);
    }
    
    return result+1;
}
int chk_max(){
    int result=0;
    for(auto exp:express){
        stringstream ss(exp);
        string left,op,right,equal,ans;
        ss>>left>>op>>right>>equal>>ans;
        result=max(result,valid(left));
        result=max(result,valid(right));
        result=max(result,valid(ans));
    }
    return result;
}
string to_base(int value,int base){
    if(value==0)
        return "0";
    string result="";
    while(value>0){
        result+=(value%base)+'0';
        value/=base;
    }
    reverse(result.begin(),result.end());
    return result;
}
vector<string> solution(vector<string> expressions) {
    vector<string> answer;
    express=expressions;
    int target=max(2,chk_max());
    set<int>valid_list;
    for(int i=target;i<10;i++){
        valid_list.insert(i);
    }
    for(int i=target;i<10;i++){
        bool is_ok=true;
        for(auto exp:expressions){  
            stringstream ss(exp);
            string left,op,right,ans,equal;
            ss>>left>>op>>right>>equal>>ans;
            if(ans=="X")
                continue;
            int l=stoi(left,nullptr,i);
            int r=stoi(right,nullptr,i);
            int a=stoi(ans,nullptr,i);
            if(op=="+"){
                if(a!=l+r){
                    is_ok=false;
                    break;
                }
            }
            else{
                if(a!=l-r){
                    is_ok=false;
                    break;
                }
            }
        }
        if(!is_ok){
            valid_list.erase(i);
        }
    }
    for(auto exp: expressions){
        stringstream ss(exp);
        string left,op,right,ans,equal;
        ss>>left>>op>>right>>equal>>ans;
        if(ans!="X")
            continue;
        set<string>chk;
        for(auto tar : valid_list){
            int l=stoi(left,nullptr,tar);
            int r=stoi(right,nullptr,tar);
            int a;
            if(op=="+"){
                a=l+r;
            }
            else{
                a=l-r;
            }
            chk.insert(to_base(a,tar));
        }
        string an=(chk.size()==1? *chk.begin():"?");
        answer.push_back(left+" "+op+" "+right+" = "+an);
    }
    return answer;
}