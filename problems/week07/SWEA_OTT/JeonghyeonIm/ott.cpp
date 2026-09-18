#include <vector>
#include <map>
#include <set>

using namespace std;

struct RESULT
{
    int cnt;
    int IDs[5];
};

struct movie
{
    int id, genre, total, ord;
    bool live;
};

vector<movie> mv;
map<int, int> mp;   // 영화 ID -> mv 인덱스
int addCnt;

struct Cmp
{
    bool operator()(const int &a, const int &b) const
    {
        if (mv[a].total != mv[b].total) return mv[a].total > mv[b].total;
        return mv[a].ord > mv[b].ord;
    }
};

set<int, Cmp> gs[6];    // gs[0] = 전체, gs[1~5] = 장르별

vector<vector<pair<int, int>>> hist;   // 유저별 (영화 인덱스, 평점) 시청 순서대로
vector<set<int>> seen;                 // 유저별 시청한 영화 인덱스

void init(int N)
{
    for (int i = 0; i < 6; i++) gs[i].clear();
    mv.clear();
    mp.clear();
    hist.assign(N + 1, vector<pair<int, int>>());
    seen.assign(N + 1, set<int>());
    addCnt = 0;
}

int add(int mID, int mGenre, int mTotal)
{
    if (mp.count(mID)) return 0;

    movie m;
    m.id = mID;
    m.genre = mGenre;
    m.total = mTotal;
    m.ord = ++addCnt;
    m.live = true;

    mv.push_back(m);
    int idx = mv.size() - 1;
    mp[mID] = idx;

    gs[0].insert(idx);
    gs[mGenre].insert(idx);

    return 1;
}

int erase(int mID)
{
    auto it = mp.find(mID);
    if (it == mp.end()) return 0;

    int idx = it->second;
    if (!mv[idx].live) return 0;

    gs[0].erase(idx);
    gs[mv[idx].genre].erase(idx);
    mv[idx].live = false;

    return 1;
}

int watch(int uID, int mID, int mRating)
{
    auto it = mp.find(mID);
    if (it == mp.end()) return 0;

    int idx = it->second;
    if (!mv[idx].live) return 0;
    if (seen[uID].count(idx)) return 0;

    // 총점이 정렬 기준이라 뺐다가 다시 넣어야 함
    gs[0].erase(idx);
    gs[mv[idx].genre].erase(idx);
    mv[idx].total += mRating;
    gs[0].insert(idx);
    gs[mv[idx].genre].insert(idx);

    seen[uID].insert(idx);
    hist[uID].push_back({ idx, mRating });

    return 1;
}

RESULT suggest(int uID)
{
    RESULT res;
    res.cnt = 0;

    // 최근 시청한(삭제 안 된) 영화 5개 중 평점 최고인 영화의 장르
    int best = -1, g = 0, cnt = 0;
    for (int i = hist[uID].size() - 1; i >= 0 && cnt < 5; i--)
    {
        int idx = hist[uID][i].first;
        if (!mv[idx].live) continue;

        cnt++;
        if (hist[uID][i].second > best)  // 뒤에서부터 보니까 동점이면 앞에 잡힌게 더 최근
        {
            best = hist[uID][i].second;
            g = mv[idx].genre;
        }
    }

    for (int idx : gs[g])
    {
        if (seen[uID].count(idx)) continue;

        res.IDs[res.cnt++] = mv[idx].id;
        if (res.cnt == 5) break;
    }

    return res;
}