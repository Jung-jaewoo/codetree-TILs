#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
#define fastio ios_base::sync_with_stdio(false), cin.tie(NULL)
#define MAXM 300

using namespace std;

int K, M, supplyPtr = 0;
int treasureMap[5][5]; // 0,0 부터 시작
int restTreasure[MAXM];
bool visited[5][5];
bool supplyCheck[5][5];
int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1};

void input();
void rotateMap(int x, int y);
int bfs(int x, int y);
vector<pair<int,int>> checkSupply(int x, int y);
int getScore();
vector<vector<pair<int,int>>> getSupplies();
int choosePointAndRotate();
void supplyTreasure();

void print(){
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++)
            cout << treasureMap[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    fastio;
    input();
    int score, addition;
    bool flag = true;
    
    while(K--){
        score = choosePointAndRotate();
        while(1){
            // print();
            // cout<< "("<< score<< ")";
            supplyTreasure();
            // print();
            addition = getScore();
            if(addition==0) break;
            score += addition;
        }
        if(score!=0)
            cout << score << " ";
        // cout << "\n";
    }

    return 0;
}

void input(){
    cin >> K >> M;
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            cin >> treasureMap[i][j];
    for(int i=0; i<M; i++)
        cin >> restTreasure[i];   
}

void rotateMap(int x, int y){ // x, y는 중심값
    int tmpMap[3][3];
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            tmpMap[j][2-i] = treasureMap[x-1+i][y-1+j];
    
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            treasureMap[x-1+i][y-1+j] = tmpMap[i][j];
}

int bfs(int x, int y){
    queue<pair<int,int>> que;
    que.push(make_pair(x,y));
    int num = treasureMap[x][y];
    int cnt = 0;
    while(que.size()!=0){
        pair<int,int> cur = que.front();
        que.pop();
        for(int i=0; i<4; i++){
            if(cur.first + dx[i] < 0 || cur.first + dx[i] > 4) continue;
            if(cur.second + dy[i] < 0 || cur.second + dy[i] > 4) continue;
            if(treasureMap[cur.first+dx[i]][cur.second+dy[i]] == num && !visited[cur.first+dx[i]][cur.second+dy[i]]){
                que.push(make_pair(cur.first+dx[i],cur.second+dy[i]));
                visited[cur.first+dx[i]][cur.second+dy[i]] = true;
                cnt++;
            }
        }
    }
    if(cnt < 3) return 0;
    return cnt;
}

vector<pair<int,int>> checkSupply(int x, int y){
    queue<pair<int,int>> que;
    vector<pair<int,int>> supplies;
    que.push(make_pair(x,y));
    int num = treasureMap[x][y];
    int cnt = 0;
    while(que.size()!=0){
        pair<int,int> cur = que.front();
        que.pop();
        for(int i=0; i<4; i++){
            if(cur.first + dx[i] < 0 || cur.first + dx[i] > 4) continue;
            if(cur.second + dy[i] < 0 || cur.second + dy[i] > 4) continue;
            if(treasureMap[cur.first+dx[i]][cur.second+dy[i]] == num && !visited[cur.first+dx[i]][cur.second+dy[i]]){
                que.push(make_pair(cur.first+dx[i],cur.second+dy[i]));
                visited[cur.first+dx[i]][cur.second+dy[i]] = true;
                cnt++;
                supplies.push_back(make_pair(cur.first+dx[i],cur.second+dy[i]));
            }
        }
    }
    if(cnt < 3) return {};
    return supplies;
}

int getScore(){
    memset(visited, 0, sizeof(visited));
    int score = 0;
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(!visited[i][j]){
                score += bfs(i,j);
            }
        }
    }
    return score;
}

 vector<vector<pair<int,int>>> getSupplies(){
    memset(visited, 0, sizeof(visited));
    vector<vector<pair<int,int>>> supplies;
    vector<pair<int,int>> tmp;
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(!visited[i][j]){
                tmp = checkSupply(i,j);
                if(tmp.size()!=0)
                    supplies.push_back(tmp);
            }
        }
    }
    return supplies;
}

int choosePointAndRotate(){
    pair<int, int> max_point;
    int max_value = 0, max_angle = 4, tmp;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<4; k++){
                rotateMap(1+j, 1+i); //열 낮은순부터, 행 낮은순부터
                if(k==3) break;
                tmp = getScore();
                if(max_value < tmp || (max_value == tmp && max_angle > k)){
                    max_value = tmp;
                    max_angle = k;
                    max_point.first = 1+j;
                    max_point.second = 1+i;
                }
            }
        }
    }
    for(int i=0; i<max_angle+1; i++)
        rotateMap(max_point.first, max_point.second);
    return max_value;
}

void supplyTreasure(){
    memset(supplyCheck,0,sizeof(supplyCheck));
    vector<vector<pair<int,int>>> supplies = getSupplies();
    for(auto i : supplies)
        for(auto j : i)
            supplyCheck[j.first][j.second] = true;

    for(int i=0; i<5; i++){
        for(int j=5; j>=0; j--){
            if(supplyCheck[j][i]){
                treasureMap[j][i] = restTreasure[supplyPtr++];
            }
        }
    }
}