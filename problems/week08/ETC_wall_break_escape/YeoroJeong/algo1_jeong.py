"""
문제: 벽을 한 번 부수고 탈출하기 (첨부 코드 기반 설명형 제목)
작성자: 정현수
원본 파일: algo1_jeong.py
공식 문제 번호·링크: 미제공
알고리즘: BFS, 상태 분리
시간 복잡도: O(N*M)
공간 복잡도: O(N*M)
검증: 로컬 경계값 및 무작위 비교; 공식 채점 결과 미확인
"""

# ==============================================
# 코드 제출시 아래 2줄은 반드시 주석처리 하여 제출
# import sys
#
# sys.stdin = open('algo1_sample_in.txt')
# ==============================================

# 아래에 코드를 작성하세요.
# BFS로 구현하는 것이 좋은 문제이기에, 큐로 사용할 deque를 import 해준다.
from collections import deque

T = int(input())
# dr, dc를 통해 4 방향 이동을 구현한다.
dr = [0, 1, 0, -1]
dc = [1, 0, -1, 0]
for test_case in range(1, T + 1):
    # 우선 너비와 높이를 받아준 뒤
    N, M = map(int, input().split())
    # 건물 평면도를 받아오고
    arr = [list(map(int, input().split())) for _ in range(N)]
    # BFS시 이미 지나간 위치에 재접근 하지 않도록 visited를 만들어준다.
    # 이때 벽을 딱 한번 부술 수 있다는 점에서, 벽을 부순 적 있는 상태로 도달한 적 있는지,
    # 벽을 아직 안 부순 상태로 도달한적 있는지를 따로 두기 위해여
    # visited[부술수 있는지][r 좌표][c 좌표]
    # 로 접근할 수 있게 만들고 0으로 초기화 시켜준다.
    visited = [[[0] * M for _ in range(N)] for _ in range(2)]
    st_r, st_c = -1, -1
    # 우선 3번에 해당하는 출발 위치를 찾고
    for r in range(N):
        for c in range(M):
            if arr[r][c] == 3:
                st_r, st_c = r, c
    q = deque([])
    # 해당위치를 큐로 사용할 '덱' 에 집어넣는다
    # 이때 r,c, 남은 벽을 부술 수 있는 횟수, 현재까지 걸린 시간
    # 형식으로 저장해 준다.
    q.append((st_r, st_c, 1, 0))
    # 이후 출발점의 visited를 1 로 만들어준다
    visited[1][st_r][st_c] = 1
    # 모든 경로를 판정했음에도 ans 가 바뀌지 않으면 출구까지의 경로가 존재하지 않는다는
    # 뜻이므로 초기값을 -1로 선정한다.
    ans = -1
    # 큐가 다 빌때까지 while문을 돌려준다.
    while q:
        # 큐의 앞에서 현재 상태들을 불러온다, 현재위치, 벽 부술수 있는지 등등
        cur_r, cur_c, can_break, cnt = q.popleft()
        # 만약 현재위치가 출구라면 출구에 도착한 것이기에 지금까지 관리하고 있던
        # 현재까지 걸린 시간 cnt를 ans로 만들어주고 반복문을 탈출.
        # 체크 안해도 되는 이유은 BFS 이기때문에 먼저 도달하는 값이 무조건
        # 최적해임을 보장하기 때문.
        if arr[cur_r][cur_c] == 2:
            ans = cnt
            break

        # 네 방향에 대하여
        for i in range(4):
            nr = cur_r + dr[i]
            nc = cur_c + dc[i]
            # 범위를 벗어나면 무시
            if nr < 0 or nr >= N or nc < 0 or nc >= M:
                continue
            # 벽을 만나면
            if arr[nr][nc] == 1:
                # 부술 여력이 있고, 벽을 부순 적 있는 채로 해당위치에 도달한 적 없으면
                if can_break == 1 and visited[0][nr][nc] == 0:
                    # 벽 부수고 이동
                    visited[0][nr][nc] = 1
                    q.append((nr, nc, 0, cnt + 1))
                # 이동 또는 이동하지 않은 뒤 다음 방향 판정하기 위해 continue
                continue

            # 만난게 벽이 아닐시 (출발점, 도착지, 0) 해당 위치를 현재 상태로 들른적 없으면 이동.
            # 출발점도 벽 파괴 여부에 따라 서로 다른 상태로 방문할 수 있다.
            if visited[can_break][nr][nc] == 0:
                visited[can_break][nr][nc] = 1
                q.append((nr, nc, can_break, cnt + 1))

    print(f"#{test_case} {ans}")
