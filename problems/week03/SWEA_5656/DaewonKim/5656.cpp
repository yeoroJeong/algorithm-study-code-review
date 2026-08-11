/*
    AI 사용
    - DFS+ backtracking , BFS로 중력 구현
    - gravity 부분에서 AI 사용

*/


#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
int n=0;

// destory 함수에서 queue에 넣을 자료형
struct Block
{
    int x;
    int y;
    int value = 0;
};

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

// 공을 떨어뜨릴 수 있는 위치 검사
vector<pair<int,int>> findHitRow(vector<vector<int>>& blocks)
{
    int width = (int)blocks.size();
    int height = (int)blocks[0].size();
    
    vector<pair<int,int>> canHitPos;
    for(int col = 0; col<height; ++col)
    {
        for(int row =0; row<width; ++row)
        {
            if(blocks[row][col] != 0)
            {
                canHitPos.push_back({row, col});
                break;
            }
        }
    }
    return canHitPos;
}

// queue를 사용해 BFS로 블록을 제거
void destroy(vector<vector<int>>& newBlocks, int curRow, int curCol)
{
    Block b;
    b.x = curRow; 
    b.y = curCol;
    b.value = newBlocks[curRow][curCol];

    queue<Block> q;
    q.push(b);

    while(!q.empty())
    {
        Block frontBlock = q.front();
        q.pop();
        newBlocks[frontBlock.x][frontBlock.y] = 0;
        int destroyRange = frontBlock.value;
        for(int i=0; i<4; ++i)
        {
            for(int dist=1; dist<destroyRange; ++dist)
            {
                int newRow = frontBlock.x + dx[i]*dist;   // dist를 곱해야 함
                int newCol = frontBlock.y + dy[i]*dist;
            
                if (newRow < 0 || newRow >= (int)newBlocks.size() ||
                    newCol < 0 || newCol >= (int)newBlocks[0].size())
                    break;   // 이 방향은 더 볼 필요 없음 (거리가 늘어날수록 더 멀어지므로)
            
                if (newBlocks[newRow][newCol] != 0)
                {
                    Block b1;
                    b1.x = newRow;
                    b1.y = newCol;
                    b1.value = newBlocks[newRow][newCol];
                    q.push(b1);
                }
            }
        }

    }

}

void gravity(vector<vector<int>>& newBlocks)
{
    int H = (int)newBlocks.size();
    int W = (int)newBlocks[0].size();

    for (int col = 0; col < W; ++col)
    {
        // 숫자를 작성할 위치 -> 맨 아래부터 설정
        // writeRow 위치에 이미 값이 있더라도 자기 자신이 작성되므로 별다른 처리가 필요 없음. 
        int writeRow = H - 1;   

        for (int row = H - 1; row >= 0; --row)
        {
            // 블록에 값이 있다면
            if (newBlocks[row][col] != 0)
            {
                // 맨 밑에 값이 있다면, 그대로 설정됨.
                newBlocks[writeRow][col] = newBlocks[row][col];
                if (writeRow != row)
                    newBlocks[row][col] = 0;
                
                // 작성할 위치 이동
                writeRow--;
            }
        }

        // 방어적 조치 
        while (writeRow >= 0)
        {
            newBlocks[writeRow][col] = 0;
            writeRow--;
        }
    }

}

// 남은 블록 갯수 세기
int countBricks(vector<vector<int>>& newBlocks)
{
    int cnt = 0;
    for(auto& row: newBlocks)
    {
        for(int col : row)
        {
            if(col != 0) cnt++;
        }
    }
    return cnt;
}

void dfs(vector<vector<int>>& blocks, int ballCnt, int &minLeftBlocks)
{
    // 열마다 발사 지점 탐색
    vector<pair<int,int>> hitPositions = findHitRow(blocks);

    // 발사횟수가 끝났을 때, 남은 블록 검사.
    if (ballCnt == 0 || hitPositions.empty())
    {
        minLeftBlocks = min(minLeftBlocks, countBricks(blocks));
        return;
    }
    
    // 블록을 떨어뜨리는 위치마다 dfs
    for (auto& pos : hitPositions)
    {
        // 임시배열로 원본에 손상 X
        // 매 위치마다 복사본 배열에 원본 복사본으로 원복
        vector<vector<int>> newBlocks = blocks;  
        
        // 파괴
        destroy(newBlocks, pos.first, pos.second);

        // 중력 적용
        gravity(newBlocks);
        
        dfs(newBlocks, ballCnt - 1, minLeftBlocks);
    }
    
}


int main(int argc, char** argv)
{
	int test_case;
	int T;

	cin>>T;

	for(test_case = 1; test_case <= T; ++test_case)
	{
        int width, height;
        cin >> n >> width >> height;

        // 남은 블럭의 갯수는 최댓값으로 초기화
        int minLeftBlocks = 2*width*height;
        vector<vector<int>> blocks(height, vector<int> (width, 0));
        for (int row = 0; row < height; ++row)
            for (int col = 0; col < width; ++col)
                cin >> blocks[row][col];

        dfs(blocks, n, minLeftBlocks);
        cout << "#" << test_case << " " << minLeftBlocks << "\n";
	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}