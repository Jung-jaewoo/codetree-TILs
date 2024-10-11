#include <iostream>
#include <cstring>
#include <queue>
#define fastio ios_base::sync_with_stdio(false), cin.tie(NULL)
#define MAXK 1000
using namespace std;

int R, C, K;
int graph[71][71]; //0: 비어있음, 1:골렘, 2:골렘탈출구, 10:골렘중심
bool visited[71][71];
int arr_c[MAXK];
int arr_d[MAXK];
int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1};

bool goDown(int x, int y);
bool turnLeftGoDown(int x, int y);
bool turnRightGoDown(int x, int y);
int bfs(int x, int y);

int main() {
    cin >> R >> C >> K;
    // for(int i=0; i<K; i++)
    //     cin >> arr_c[i] >> arr_d[i];
    int c, d, x, y, score = 0, tmp;
    while(K--){
        cin >> c >> d;
        x= -1, y = c;
        while(1){
            if(goDown(x,y)) x += 1;
            else if(turnLeftGoDown(x,y)){
                x+=1; 
                y-=1;
                d = (d + 3) % 4;
            }
            else if(turnRightGoDown(x,y)){
                x+=1;
                y+=1;
                d = (d + 1) % 4;
            }
            else
                break;
        }

        //더이상 이동 불가 숲 밖인지 체크
        if(x < 2){
            memset(graph,0,sizeof(graph));
            continue;
        }

        // 숲 안이면 1.그래프에 골렘표시, 2.정령이 움직이고 점수 체크
        graph[x-1][y] = graph[x][y-1] = graph[x][y+1] = graph[x+1][y] = 1;
        graph[x][y] = 10;
        graph[x+dx[d]][y+dy[d]] += 1;

        tmp = bfs(x,y);
        if(tmp ==-1) return -1;
        score += tmp;
        // cout << tmp << " ";
    }
    cout << score;
    return 0;
}

bool goDown(int x, int y){
    if(x==R-1) return false;
    if(graph[x+1][y-1] || graph[x+1][y+1] || graph[x+2][y]) return false;
    return true;
}

bool turnLeftGoDown(int x, int y){
    if(y<=2) return false;
    if(graph[x-1][y-1] || graph[x][y-2] || graph[x+1][y-1]) return false;
    return goDown(x,y-1);
}

bool turnRightGoDown(int x, int y){
    if(y>=C-1) return false;
    if(graph[x-1][y+1] || graph[x][y+2] || graph[x+1][y+1]) return false;
    return goDown(x,y+1);
}

bool checkInForest(int x, int y){
    if(x < 1 || x > R) return false;
    if(y < 1 || y > C) return false;
    return true;
}

int bfs(int x, int y){
    memset(visited,0,sizeof(visited));
    int deepthest = 0;
    visited[x][y] = true;

    queue<pair<int, int>> que;
    que.push(make_pair(x,y));
    while(que.size()>0){
        pair<int, int> cur, exit;
        cur = que.front();
        que.pop();

        //탈출구 찾고
        for(int i=0; i<4; i++){
            if(graph[cur.first+dx[i]][cur.second+dy[i]] == 2){
                exit.first = cur.first+dx[i];
                exit.second = cur.second+dy[i];
                break;
            }
        }

        //인접 골렘 찾기
        for(int i=0; i<4; i++){
            if(!checkInForest(exit.first+dx[i],exit.second+dy[i])) continue;
            if(graph[exit.first+dx[i]][exit.second+dy[i]] != 0 && graph[exit.first+dx[i]][exit.second+dy[i]] != 10){
                for(int j=0; j<4; j++){
                    if(!checkInForest(exit.first+dx[i]+dy[j],exit.second+dy[i]+dy[j])) continue;
                    if(graph[exit.first+dx[i]+dx[j]][exit.second+dy[i]+dy[j]] == 10 && !visited[exit.first+dx[i]+dx[j]][exit.second+dy[i]+dy[j]]){
                        visited[exit.first+dx[i]+dx[j]][exit.second+dy[i]+dy[j]] = true;
                        que.push(make_pair(exit.first+dx[i]+dx[j],exit.second+dy[i]+dy[j]));
                    }
                }
            }
        }
    }

    for(int i=R; i>=1; i--)
        for(int j=1; j<=C; j++)
            if(visited[i][j])
                return i+1;
    return -1;
}