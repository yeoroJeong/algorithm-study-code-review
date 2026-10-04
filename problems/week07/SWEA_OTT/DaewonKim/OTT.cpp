#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct RESULT
{
	int cnt;
	int IDs[5];
};


// 영화 자체의 정보
struct Movie
{
	int id;
	int genre;       // 1 ~ 5
	int score;       // 영화의 현재 총점
	int whenOpen;    // 영화 등록 순서
	bool isAvail;    // 삭제 여부
};


// 특정 사용자가 영화를 시청했을 때의 정보
struct WatchInfo
{
	int movieID;
	int rating;      // 해당 사용자가 준 평점
};


// 사용자 정보
struct User
{
	// 전체 시청 기록
	// vector 뒤로 갈수록 최근 시청
	vector<WatchInfo> watchHistory;

	// 과거에 시청한 영화인지 빠르게 확인
	unordered_set<int> watchedMovie;
};


// 영화 추천 순위용
struct MovieRank
{
	int id;
	int score;
	int whenOpen;
	int genre;

	bool operator<(const MovieRank& other) const
	{
		// 1. 총점 높은 영화 우선
		if (score != other.score)
			return score > other.score;

		// 2. 총점이 같으면 최근 등록 영화 우선
		if (whenOpen != other.whenOpen)
			return whenOpen > other.whenOpen;

		// 혹시 모를 완전 동일 상황 방지
		return id > other.id;
	}
};


int openNum;

vector<User> users;
vector<Movie> movies;

// movieIndexs[mID] = movies에서 해당 영화의 index
unordered_map<int, int> movieIndexs;


// 영화 순위의 경우, 값이 바뀌면 insert, erase를 한다
// O(log N)의 시간복잡도를 가지기 때문에 삽입, 삭제에 대한 부담이 적다.
// 전체 영화 순위
set<MovieRank> movieRank;

// 장르별 영화 순위
set<MovieRank> genreRank[6];


// 초기화
void init(int N)
{
	openNum = 0;

	users.clear();
	movies.clear();
	movieIndexs.clear();
	movieRank.clear();

	for (int genre = 1; genre <= 5; ++genre)
	{
		genreRank[genre].clear();
	}

	// user ID가 1 ~ N
	users.resize(N + 1);
}



int add(int mID, int mGenre, int mTotal)
{
	// 삭제된 영화의 ID로 다시 등록되는 경우도 없으므로
	// map에 존재하기만 하면 이미 등록된 적이 있는 영화
	if (movieIndexs.find(mID) != movieIndexs.end())
		return 0;


	Movie movie;

	movie.id = mID;
	movie.genre = mGenre;
	movie.score = mTotal;
	movie.whenOpen = openNum++;
	movie.isAvail = true;


	int movieIndex = (int)movies.size();
	movies.push_back(movie);
	movieIndexs[mID] = movieIndex;


	MovieRank rank;

	rank.id = mID;
	rank.genre = mGenre;
	rank.score = mTotal;
	rank.whenOpen = movie.whenOpen;


	// 전체 순위
	movieRank.insert(rank);

	// 해당 장르 순위
	genreRank[mGenre].insert(rank);


	return 1;
}



int erase(int mID)
{
	auto it = movieIndexs.find(mID);

	// 등록된 적이 없는 영화
	if (it == movieIndexs.end())
		return 0;


	int movieIndex = it->second;

	// 이미 삭제된 영화
	if (!movies[movieIndex].isAvail)
		return 0;


	Movie& movie = movies[movieIndex];

	// 현재 set 안에 들어있는 정보를 생성
	MovieRank rank;

	rank.id = movie.id;
	rank.genre = movie.genre;
	rank.score = movie.score;
	rank.whenOpen = movie.whenOpen;


	// 추천 순위에서 제거
	movieRank.erase(rank);
	genreRank[movie.genre].erase(rank);


	// 실제 영화는 삭제 표시만
	movie.isAvail = false;

	return 1;
}



int watch(int uID, int mID, int mRating)
{
	auto it = movieIndexs.find(mID);

	// 등록된 적 없는 영화
	if (it == movieIndexs.end())
		return 0;


	int movieIndex = it->second;
	Movie& movie = movies[movieIndex];

	// 삭제된 영화
	if (!movie.isAvail)
		return 0;

	// 이미 사용자가 본 영화
	if (users[uID].watchedMovie.count(mID))
		return 0;

	/*
		기존 movieRank에 있는 영화의 점수를 먼저 삭제해야 한다.

		set은 정렬 기준이 score이므로
		score를 바꾼 뒤에는 기존 원소를 정상적으로 찾을 수 없다.
	*/
	MovieRank oldRank;

	oldRank.id = movie.id;
	oldRank.genre = movie.genre;
	oldRank.score = movie.score;
	oldRank.whenOpen = movie.whenOpen;


	movieRank.erase(oldRank);
	genreRank[movie.genre].erase(oldRank);


	// 사용자의 시청 기록 저장
	WatchInfo watchInfo;

	watchInfo.movieID = mID;
	watchInfo.rating = mRating;

	users[uID].watchHistory.push_back(watchInfo);

	users[uID].watchedMovie.insert(mID);


	// 영화 총점 증가
	movie.score += mRating;


	// 변경된 총점으로 다시 순위에 삽입
	MovieRank newRank;

	newRank.id = movie.id;
	newRank.genre = movie.genre;
	newRank.score = movie.score;
	newRank.whenOpen = movie.whenOpen;


	movieRank.insert(newRank);
	genreRank[movie.genre].insert(newRank);


	return 1;
}



RESULT suggest(int uID)
{
	RESULT res;
	res.cnt = 0;


	/*
		추천 장르 결정
		- 최근 시청한 영화부터 역순으로 확인한다.
		- 삭제되지 않은 영화를 기준으로 최대 5개만 확인한다.
	*/

	int suggestGenre = 0;

	int maxRating = -1;

	// 삭제되지 않은 시청 영화 개수
	int validWatchCnt = 0;

	vector<WatchInfo>& history = users[uID].watchHistory;

	for (int i = (int)history.size() - 1;
		i >= 0 && validWatchCnt < 5;
		--i)
	{
		int movieID = history[i].movieID;
		int rating = history[i].rating;


		int movieIndex = movieIndexs[movieID];


		// 삭제된 영화는 현재 시청 목록에 존재하지 않는 것으로 처리
		if (!movies[movieIndex].isAvail)
			continue;


		validWatchCnt++;


		/*
			최근 영화부터 확인하고 있기 때문에
			rating이 같은 경우에는 갱신하면 안 된다.
			먼저 발견한 영화가 더 최근에 시청한 영화이기 때문이다.
		*/
		if (rating > maxRating)
		{
			maxRating = rating;
			suggestGenre = movies[movieIndex].genre;
		}
	}



		// 시청 목록에 유효한 영화가 하나도 없음 ->장르 상관없이 전체 영화 중 추천
	if (validWatchCnt == 0)
	{
		for (const auto& movie : movieRank)
		{
			// 이미 시청한 영화 제외
			if (users[uID].watchedMovie.count(movie.id))
				continue;


			res.IDs[res.cnt++] = movie.id;


			if (res.cnt == 5)
				break;
		}
	}

	//최근 시청 영화가 있음->  결정된 장르에서만 추천
	else
	{
		for (const auto& movie : genreRank[suggestGenre])
		{
			// 이미 사용자가 본 영화 제외
			if (users[uID].watchedMovie.count(movie.id))
				continue;


			res.IDs[res.cnt++] = movie.id;


			if (res.cnt == 5)
				break;
		}
	}


	return res;
}