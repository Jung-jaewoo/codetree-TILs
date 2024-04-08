#include <iostream>
#include <vector>
using namespace std;

void input();
bool isCanMove(int number, int& direction);
void move(vector<int>& command);

vector<vector<int>> chessboard;
vector<vector<int>> knights;
vector<vector<int>> knights_temps;
vector<int> bef_lifes;
vector<vector<int>> commands;
vector<vector<int>> directions = {{-1,0},{0,1},{1,0},{0,-1}};
vector<bool> is_moved;
vector<int> damage;
int L, N, Q;

int main() {
    // 입력
    input();

    //동작 수행 및 데미지 계산
    for(int i=0; i<commands.size(); i++)
        move(commands[i]);

    //데미지 계산
    int solution = 0;
    for(int i=1; i<=N; i++){
        if(knights[i][4]<=0)
            continue;
        solution += bef_lifes[i] - knights[i][4];
    }
    cout << solution;

    return 0;
}

void input(){
    int n, temp;
    cin >> L >> N >> Q;
    
    chessboard.resize(L+1,vector<int>(L+1));
    for(int i=1; i<=L; i++){
        for(int j=1; j<=L; j++){
            cin >> n;
            chessboard[i][j]=n;
        }
    }

    is_moved.resize(N+1);
    damage.resize(N+1);
    knights.resize(N+1);
    knights_temps.resize(N+1,vector<int>(2));
    bef_lifes.resize(N+1);
    for(int i=1; i<=N; i++){
        for(int j=0; j<5; j++){
            cin >> temp;
            knights[i].push_back(temp);
            if(j==4)
                bef_lifes[i]=temp;
        }
    }
    
    commands.resize(Q);
    for(int i=0; i<Q; i++){
        for(int j=0; j<2; j++){
            cin >> temp;
            commands[i].push_back(temp);
        }
    }
}

bool isCanMove(int number, int& direction){
    bool result = true;
    is_moved[number] = true;

    int currentR = knights[number][0]+directions[direction][0];
    int currentC = knights[number][1]+directions[direction][1];
    int currentH = knights[number][2];
    int currentW = knights[number][3];
    knights_temps[number][0] = currentR;
    knights_temps[number][1] = currentC;

    if(currentR <= 0 || currentR + currentH -1>L || currentC <= 0 || currentC + currentW -1>L)
        return false;

    for(int i=currentR; i<currentR+currentH; i++){
        for(int j=currentC; j<currentC+currentW; j++){
            if(chessboard[i][j]==1)
                damage[number]+=1; 
            if(chessboard[i][j]==2)
                return false;
        }
    }
    
    for(int i=1; i<=N; i++){
        if(is_moved[i] || knights[i][4]<=0)
            continue;
        if(knights[i][0]>currentR+currentH-1 || currentR>knights[i][0]+knights[i][2]-1)
            continue;
        if(knights[i][1]>currentC+currentW-1 || currentC>knights[i][1]+knights[i][3]-1)
            continue;
        result = isCanMove(i,direction);
    }
    return result;
}

void move(vector<int>& command){
    if(knights[command[0]][4]<=0)
        return;

    //초기화
    for(int i=1; i<=N; i++){
        is_moved[i] = false;
        damage[i] = 0;
        knights_temps[i][0] = knights[i][0];
        knights_temps[i][1] = knights[i][1];
    }

    if(isCanMove(command[0],command[1])){
        damage[command[0]] = 0;
        for(int i=1; i<=N; i++){
            knights[i][0] = knights_temps[i][0];
            knights[i][1] = knights_temps[i][1];
            knights[i][4] -= damage[i];
        }
    }
}