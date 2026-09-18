#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#define CMD_INIT        (100)
#define CMD_ADD         (200)
#define CMD_ERASE       (300)
#define CMD_WATCH       (400)
#define CMD_SUGGEST     (500)

//extern void init(int N);
//extern int add(int mID, int mGenre, int mTotal);
//extern int erase(int mID);
//extern int watch(int uID, int mID, int mRating);
//extern RESULT suggest(int uID);

using namespace std;


struct RESULT
{
    int cnt;
    int IDs[5];
};


struct Movie {
    int mID;
    int mGenre;
    int mTotal;
    int order;
    bool deleted;
};

struct Watch {
    int mID;
    int rating;
};

struct Compare {
    bool operator()(const Movie& a, const Movie& b) const {
        if (a.mTotal != b.mTotal)
            return a.mTotal > b.mTotal;
        if (a.order != b.order)
            return a.order > b.order;
        return a.mID < b.mID;
    }
};

// 전체 영화 추천 순위 총점 > 등록 최신순
// watch 할 때 mTotal에 대해서 두 개 자료구조 모두 신경쓸 것
set<Movie, Compare> genreTree[6];
// 전체 영화 저장 mID로 찾기 수월
unordered_map<int, Movie> movies;



// 시청은 시간 순으로 추가, 최근 기록은 뒤에서부터 확인가능 -> 추천을 위함
// 영화가 삭제된 영화인지는 movies와 대조, 본적 있는 영화인지는 watched와 비교
vector<Watch> history[1001];
// 사용자가 본 적 있는 영화인가? -> 시청을 위함
unordered_set<int> watched[1001];
// 등록 순서를 위한 전역 카운터
int movieOrder;

void init(int N)
{
    movieOrder = 1;
    movies.clear();
    for (int i = 0; i < 1001; i++) {
        watched[i].clear();
        history[i].clear();
    }
    for (int i = 0; i < 6; i++) {    // 영화 tree 비우기
        genreTree[i].clear();
    }
    return;
}

int add(int mID, int mGenre, int mTotal)
{
    Movie movie = { mID, mGenre, mTotal, movieOrder, false };
    if (movies.find(mID) != movies.end()) {
        return 0;
    }
    
    genreTree[mGenre].insert(movie);
    movies.insert({ mID, movie });
    movieOrder++;
    return 1;
}

int erase(int mID)
{
    unordered_map<int, Movie>::iterator it = movies.find(mID);
    if (it != movies.end()) {
        // 존재
        return 0;
    }
    // set에서 지우기 위해 현재 영화 정보 저장
    Movie movie = it->second;

    // 추천 후보에서 제거
    genreTree[movie.mGenre].erase(movie);

    // 삭제 상태로 변경
    it->second.deleted = true;

    return 1;
}

int watch(int uID, int mID, int mRating)
{
    unordered_map<int, Movie>::iterator it = movies.find(mID);

    // 1. 등록된 적 없는 영화
    if (it == movies.end()) {
        return 0;
    }

    // 2. 이미 삭제된 영화
    if (it->second.deleted) {
        return 0;
    }

    // 3. 사용자가 이미 시청한 영화
    if (watched[uID].find(mID) != watched[uID].end()) {
        return 0;
    }

    // 점수가 바뀌기 전 Movie 정보
    Movie oldMovie = it->second;

    // set은 mTotal을 정렬 기준으로 사용하므로
    // 점수 변경 전에 기존 값을 먼저 삭제
    genreTree[oldMovie.mGenre].erase(oldMovie);

    // 영화 총점 증가
    it->second.mTotal += mRating;

    // 변경된 점수로 다시 set에 삽입
    genreTree[it->second.mGenre].insert(it->second);

    // 사용자 시청 기록 추가
    history[uID].push_back({ mID, mRating });

    // 사용자가 본 영화로 표시
    watched[uID].insert(mID);

    return 1;
}

RESULT suggest(int uID)
{
    RESULT res;

    res.cnt = -1;

    return res;
}



static bool run()
{
    int Q, N;
    int mID, mGenre, mTotal, mRating, uID;

    int ret = -1, cnt, ans;

    RESULT res;

    scanf("%d", &Q);

    bool okay = false;

    for (int q = 0; q < Q; ++q)
    {
        int cmd;
        scanf("%d", &cmd);

        switch (cmd)
        {
        case CMD_INIT:
            scanf("%d", &N);
            init(N);
            okay = true;
            break;
        case CMD_ADD:
            scanf("%d %d %d", &mID, &mGenre, &mTotal);
            ret = add(mID, mGenre, mTotal);
            scanf("%d", &ans);
            if (ret != ans)  
                okay = false;
            break;
        case CMD_ERASE:
            scanf("%d", &mID);
            ret = erase(mID);
            scanf("%d", &ans);
            if (ret != ans)
                okay = false;
            break;
        case CMD_WATCH:
            scanf("%d %d %d", &uID, &mID, &mRating);
            ret = watch(uID, mID, mRating);
            scanf("%d", &ans);
            if (ret != ans)
                okay = false;
            break;
        case CMD_SUGGEST:
            scanf("%d", &uID);
            res = suggest(uID);
            scanf("%d", &cnt);
            if (res.cnt != cnt)
                okay = false;
            for (int i = 0; i < cnt; ++i)
            {
                scanf("%d", &ans);
                if (res.IDs[i] != ans)
                    okay = false;
            }
            break;
        default:
            okay = false;
            break;
        }
    }

    return okay;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int TC, MARK;

    scanf("%d %d", &TC, &MARK);
    for (int tc = 1; tc <= TC; ++tc)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}