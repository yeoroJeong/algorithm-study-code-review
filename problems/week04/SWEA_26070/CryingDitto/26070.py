# SWEA 26070. 보석 수집 로봇
# https://swexpertacademy.com/main/code/userProblem/userProblemDetail.do?fromProbList=N&deleteYn=N&contestProbId=AZwmBfua3q3HBIT3&topPath=code&lastPath=problemDetail&secondPath=problem&menuBreakDown=swea.code.menu&menuBreakDown=swea.code.problem.menu&menuDesc=swea.code.desc&menuDesc=swea.code.problem.desc&contextPath=%2Fmain&locale=ko-kr%2Cko%3Bq%3D0.9%2Cen-us%3Bq%3D0.8%2Cen%3Bq%3D0.7&serverName=localhost&localeLanguage=ko_KR&localeLanguage2=Ko_KR&remoteAddr=175.209.203.83&scripts=%2Fjs%2Finit%2Fjquery-debug.js&scripts=%2Fjs%2Finit%2Fjquery-ui.js&scripts=%2Fjs%2Finit%2Fjquery.validate.js&scripts=%2Fjs%2Fcommon.js&NOTICE_NEW_COUNT=0&ssoLogin=false&hasSDPAdminLinkAuth=false&systemAdmin=false&backendAdmin=false&isTechBlogManager=false&CURRENT_MENU_AUTHORIZATION=READ&CURRENT_MENU_AUTHORIZATION=UPDATE&CURRENT_MENU_AUTHORIZATION=EXECUTE&CURRENT_MENU_AUTHORIZATION=DOWNLOAD&logoMainfileName=logo_company.png

# 1부터 M까지 번호 매긴 M개의 보석
# 오른쪽으로만 회전 가능한 로봇(0, 0)에서 출발
# 회전하면 오른쪽, 아래, 왼쪽, 위 순서로 전진 방향 바뀜

# N 지도 크기(5 ≤ N ≤ 10)
# M 보석 개수(2 ≤ M ≤ 10)

# 오른쪽, 아래, 왼쪽, 위
dx = [0, 1, 0, -1]
dy = [1, 0, -1, 0]
def search(x, y, targetX, targetY, dir, jewelCount) :
    rotCnt = 0
    while True :
        if x == targetX and y == targetY :
            jewelCount += 1
            if jewelCount < jCnt :
                targetX = jewels[jewelCount + 1][0]
                targetY = jewels[jewelCount + 1][1]
            else :
                break

                nx = x + dx[dir]
                ny = y + dy[dir]

                if nx >= sizeN or nx < 0 or ny >= sizeN or ny < 0:
dir = (dir + 1) % 4
rotCnt += 1
                else:
if dir == 0 and ny > targetY:
if nx < targetX :
    dir = (dir + 1) % 4
    rotCnt += 1
    elif dir == 1 and nx > targetX :
    if ny > targetY:
dir = (dir + 1) % 4
rotCnt += 1
elif dir == 2 and ny < targetY :
    if nx > targetX :
        dir = (dir + 1) % 4
        rotCnt += 1
        elif dir == 3 and nx < targetX :
        if ny < targetY :
            dir = (dir + 1) % 4
            rotCnt += 1

            x += dx[dir]
            y += dy[dir]

            return rotCnt

            T = int(input())
            for tc in range(T) :
                sizeN = int(input())
                mapToMove = [list(map(int, input().split())) for _ in range(sizeN)]
                jewels = {}
                jCnt = 0
                for i in range(sizeN) :
                    for j in range(sizeN) :
                        if mapToMove[i][j] > 0:
jewels[mapToMove[i][j]] = (i, j)
jCnt += 1

rotCnt = search(0, 0, jewels[1][0], jewels[1][1], 0, 0)
print(f"#{tc+1} {rotCnt}")