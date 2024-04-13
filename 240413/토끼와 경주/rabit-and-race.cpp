#include <iostream>
#include <cstring>
#define fastio ios::sync_with_stdio(0) cout.tie(0) cin.tie(0)
using namespace std;

struct Rabbit{
    int x=1, y=1;
    int pid;
    int dist;
    int count = 0;
    int score = 0;
};

int Q, N, M, P, K, S, L;
Rabbit rabbits[2001];
bool is_selected[2001];
int dx[4] = {-1,0,1,0};  //0상1좌2하3우
int dy[4] = {0,-1,0,1};
void Input();
void StartRace();
void AddS();
void ChangeDist(int pid_t);


int main() {
    fastio;
    cin >> Q;
    while(Q--){
        Input();
    }
    return 0;
}

void Input(){
    int input_case ,pid_t;
    cin >> input_case;
    switch(input_case){
        case 100:
            cin >> N >> M >> P;
            for(int i=1; i<=P; i++)
                cin >> rabbits[i].pid >> rabbits[i].dist;
            break;
        case 200:
            cin >> K >> S;
            memset(is_selected,false,sizeof(is_selected));
            for(int round=1; round<=K; round++)
                StartRace();
            AddS();
            break;
        case 300:
            cin >> pid_t >> L;
            ChangeDist(pid_t);
            break;
        case 400:
            int max = 0;
            for(int i=1; i<=P; i++)
                if(rabbits[i].score>max)
                    max = rabbits[i].score;
            cout << max;
            break;
    }
}

void StartRace(){

    //우선순위 토끼 찾기
    int max_idx = 1;
    for(int i=2; i<=P; i++){
        if(rabbits[i].count < rabbits[max_idx].count)
            max_idx = i;
        else if(rabbits[i].count == rabbits[max_idx].count){
            if(rabbits[i].x + rabbits[i].y < rabbits[max_idx].x + rabbits[max_idx].y)
                max_idx = i;
            else if(rabbits[i].x + rabbits[i].y == rabbits[max_idx].x + rabbits[max_idx].y){
                if(rabbits[i].x < rabbits[max_idx].x)
                    max_idx = i;
                else if(rabbits[i].x == rabbits[max_idx].x){
                    if(rabbits[i].pid < rabbits[max_idx].pid)
                        max_idx = i;
                }   
            }
        }
    }
    
    //이동할 방향 선택
    int max_x=0, max_y=0, nx, ny, cx=rabbits[max_idx].x, cy=rabbits[max_idx].y, d=rabbits[max_idx].dist;
    // cout << max_idx << " ";    //cout
    for(int i=0; i<4; i++){
        nx = cx;
        ny = cy;
        int dir = i;
        for(int j=0; j<d; j++){
            if(nx+dx[dir]==0 || nx+dx[dir]==N+1 || ny+dy[dir]==0 || ny+dy[dir]==M+1)
                dir = (dir + 2) % 4;
            nx += dx[dir];
            ny += dy[dir];
        }
        
        // 우선순위 큰값 업데이트
        if(nx + ny > max_x + max_y){
            max_x = nx; 
            max_y = ny;
        }else if(nx + ny == max_x + max_y){
            if(nx > max_x){
                max_x = nx;
                max_y = ny;
            }else if(nx == max_x){
                if(ny > max_y){
                    max_x = nx;
                    max_y = ny;
                }
            }
        }
    }

    // cout << max_x << "," << max_y << " ";//cout
    rabbits[max_idx].x = max_x;
    rabbits[max_idx].y = max_y;
    rabbits[max_idx].count++;
    is_selected[max_idx] = true;

    for(int i=1; i<=P; i++){
        if(i == max_idx)
            continue;
        rabbits[i].score += max_x + max_y;
    }
    // for(int i=1; i<=P; i++){
    //     cout <<rabbits[i].score<< " ";
    // }
    // cout << "\n";
}

void AddS(){

    //우선순위 높은 토끼 찾기
    int max_idx = P + 1;
    for(int i=1; i<=P; i++){
        if(is_selected[i]){
            max_idx = i;
            break;
        }
    }
    for(int i=max_idx+1; i<=P; i++){
        if(is_selected[i]){
            if(rabbits[i].x + rabbits[i].y > rabbits[max_idx].x + rabbits[max_idx].y)
                max_idx = i;
            else if(rabbits[i].x + rabbits[i].y == rabbits[max_idx].x + rabbits[max_idx].y){
                if(rabbits[i].x > rabbits[max_idx].x)
                    max_idx = i;
                else if(rabbits[i].x == rabbits[max_idx].x){
                    if(rabbits[i].y > rabbits[max_idx].y)
                        max_idx = i;
                    else if(rabbits[i].y > rabbits[max_idx].y){
                        if(rabbits[i].pid > rabbits[max_idx].pid)
                            max_idx = i;
                    }  
                }   
            }
        }   
    }

    //뽑힌 토끼 없을때
    if(max_idx==P+1)
        return;

    rabbits[max_idx].score += S;
}

void ChangeDist(int pid_t){
    for(int i=1; i<=P; i++){
        if(rabbits[i].pid == pid_t)
            rabbits[i].dist *= L;
    }
}