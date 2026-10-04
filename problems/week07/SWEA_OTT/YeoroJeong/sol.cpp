#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT        (100)
#define CMD_ADD         (200)
#define CMD_ERASE       (300)
#define CMD_WATCH       (400)
#define CMD_SUGGEST     (500)

struct RESULT
{
    int cnt;
    int IDs[5];
};

extern void init(int N);
extern int add(int mID, int mGenre, int mTotal);
extern int erase(int mID);
extern int watch(int uID, int mID, int mRating);
extern RESULT suggest(int uID);

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

#include<algorithm>
#include<queue>
#include<unordered_map>
#include<set>
#include<unordered_set>

using namespace std;

// 영화의 현재 상태.
// genre가 0이면 unordered_map의 operator[]로 만들어진 미등록 영화로 판단한다.
struct Movie
{
    int id;
    int genre=0;
    int total_grade;
    int version;
    int regist_num;
    bool is;
};

// 추천 우선순위: 총점 내림차순, 동점이면 최근 등록(regist_num이 큰 값) 우선.
// set은 이 순서를 항상 유지하므로 추천할 때 앞에서부터 최대 5개만 고르면 된다.
struct CompareMovie
{
    bool operator()(const Movie& a, const Movie& b)const {
        if (a.total_grade != b.total_grade)
            return a.total_grade > b.total_grade;
        return a.regist_num > b.regist_num;
    }

};
struct History
{
    int movie_id;
    int grade;
};

struct User {
    // 이미 본 영화인지 평균 O(1)에 검사하기 위한 집합.
    unordered_set<int> watched;

    // 시청 순서와 당시 평점을 보존한다. 뒤에서부터 보면 최근 시청 순서가 된다.
    vector<History>watched_list;
};

// mID의 범위가 크므로 배열 대신 해시 맵으로 영화의 현재 상태를 관리한다.
unordered_map<int, Movie> movie_list;

// 장르별 순위와 전체 순위를 따로 관리한다.
// suggest에서 선호 장르가 있으면 genre_movie, 없으면 all_movie를 사용한다.
set<Movie,CompareMovie> genre_movie[6];
set<Movie,CompareMovie>all_movie;


vector<User>user_list;

int movie_cnt;

void init(int N)
{
    // regist_num은 호출 순서를 나타내므로 테스트케이스마다 0부터 다시 시작한다.
    movie_cnt = 0;

    // 이전 테스트케이스의 영화 순위와 상태를 제거한다.
    for (int i = 1; i <= 5; i++) {
        genre_movie[i].clear();
    }
    all_movie.clear();
    movie_list.clear();

    // uID가 1부터 시작하므로 N + 1칸을 준비한다.
    user_list.clear();
    user_list.resize(N + 1);
    return;
}

int add(int mID, int mGenre, int mTotal)
{
    // 등록된 상태의 같은 ID가 존재하면 추가할 수 없다.
    if (movie_list[mID].genre != 0 && movie_list[mID].is) {
        return 0;
    }

    // movie_cnt가 클수록 더 최근에 등록된 영화이다.
    Movie new_movie = { mID,mGenre,mTotal,0,movie_cnt,true };
    movie_list[mID] = new_movie;

    // 장르별 추천과 전체 추천 양쪽에 모두 등록한다.
    genre_movie[mGenre].insert(new_movie);
    all_movie.insert(new_movie);

    movie_cnt++;
    return 1;
}

int erase(int mID)
{
    // 미등록 영화이거나 이미 삭제된 영화라면 실패한다.
    if (movie_list[mID].genre != 0 && movie_list[mID].is) {
        int temp_genre = movie_list[mID].genre;

        // 추천 순위에서는 즉시 제외하고, 원본에는 삭제 상태를 남긴다.
        // 사용자의 시청 기록은 실제로 지우지 않고 suggest에서 is를 확인해 건너뛴다.
        genre_movie[temp_genre].erase(movie_list[mID]);
        all_movie.erase(movie_list[mID]);
        movie_list[mID].is = false;
        return 1;
    }
    return 0;
}

int watch(int uID, int mID, int mRating)
{
    auto it = user_list[uID].watched.find(mID);
    int temp_genre = movie_list[mID].genre;

    // 미등록/삭제 영화이거나 사용자가 이미 본 영화라면 시청할 수 없다.
    if (temp_genre == 0 || !movie_list[mID].is || it != user_list[uID].watched.end())
        return 0;

    // 중복 시청 검사와 최근 시청 순서 조회의 목적이 달라 두 자료구조에 함께 기록한다.
    user_list[uID].watched.insert(mID);
    user_list[uID].watched_list.push_back({ mID ,mRating});

    // set의 정렬 기준(total_grade)이 바뀌므로 반드시 기존 값을 먼저 제거한다.
    // 점수를 수정한 뒤 다시 삽입하면 두 set의 정렬 순서가 갱신된다.
    genre_movie[temp_genre].erase(movie_list[mID]);
    all_movie.erase(movie_list[mID]);

    movie_list[mID].total_grade += mRating;
    genre_movie[temp_genre].insert(movie_list[mID]);
    all_movie.insert(movie_list[mID]);

    return 1;
}

RESULT suggest(int uID)
{
    RESULT res;
    int fav_movie = 0;
    int temp_rating = -1;
    vector<History>& temp = user_list[uID].watched_list;
    int cnt = 0;

    // 최근 기록부터 삭제되지 않은 영화 최대 5개를 확인한다.
    // 역순이므로 평점이 같은 경우 먼저 발견한(더 최근의) 영화를 유지하기 위해 '>'만 쓴다.
    for (int i = temp.size() - 1; i >= 0&&cnt<5; i--) {
        if (!movie_list[temp[i].movie_id].is) continue;
        if (temp[i].grade > temp_rating) {
            temp_rating = temp[i].grade;
            fav_movie = temp[i].movie_id;
        }
        cnt++;
    }

    // 추천 후보를 순회할 때 이미 본 영화인지 평균 O(1)에 확인한다.
    unordered_set<int>& temp_watched = user_list[uID].watched;
    cnt = 0;

    if (fav_movie != 0) {
        // 유효한 시청 기록이 있으면 선호 영화와 같은 장르에서만 추천한다.
        int fav_genre = movie_list[fav_movie].genre;
        for (auto & mov : genre_movie[fav_genre]) {
            if (cnt >= 5) break;
            int mov_id = mov.id;
            if (temp_watched.find(mov_id) != temp_watched.end())
                continue;
            res.IDs[cnt] = mov_id;
            cnt++;
        }
    }
    else {
        // 유효한 시청 기록이 없으면 장르와 관계없이 전체 영화에서 추천한다.
        int fav_genre = movie_list[fav_movie].genre;
        for (auto& mov : all_movie) {
            if (cnt >= 5) break;
            int mov_id = mov.id;
            if (temp_watched.find(mov_id) != temp_watched.end())
                continue;
            res.IDs[cnt] = mov_id;
            cnt++;
        }
    }

    // set이 이미 추천 우선순위로 정렬되어 있으므로 저장된 순서가 곧 정답 순서이다.
    res.cnt = cnt;

    return res;
}
