#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

struct Turrent{
    int x;
    int y;
    int attack;
    int last_attack=0;
};

int board[11][11];
int back_x[11][11];
int back_y[11][11];
int seq[11][11];
int N, M, K, R;
int dx[4] = {0,1,0,-1}; //우하좌상
int dy[4] = {1,0,-1,0};
vector<Turrent> turrents; 

void Input();
int FindSubject();
int FindObject(int subject);
bool IsCanLazerAtk(int attacker, int obect);
bool TryLazerAtk(int attacker, int obect);
void MissileAtk(int attacker, int obect);

int main() {
    Input();

    int attacker, obect;
    int all_die_flag;
    for(R=1; R<=K; R++){
        all_die_flag = 0;
        attacker = FindSubject();
        obect = FindObject(attacker);

        //공격력 증가 및 공격 턴 업데이트
        turrents[attacker].attack+=N+M;
        board[turrents[attacker].x][turrents[attacker].y]+=N+M;
        turrents[attacker].last_attack = R;

        if(!TryLazerAtk(attacker, obect))
            MissileAtk(attacker, obect);

        //다 죽으면 즉시 종료
        for(int i=0; i<turrents.size(); i++){
            if(turrents[i].attack>0)
                all_die_flag++;
        }
        if(all_die_flag==1) break;
        // cout << attacker << " " << obect << "\n";
        // cout << turrents[attacker].x << turrents[attacker].y << " " << turrents[obect].x<< turrents[obect].y << " ";
        // if(R==9 || R== 10 || R==11|| R==77){
        //     cout << "(" << turrents[attacker].x << turrents[attacker].y << ")";
        //     cout << "(" << turrents[obect].x << turrents[obect].y << ") ";
        //     cout << "*" << turrents[attacker].attack << "*\n";
        //     for(int i=1; i<=N; i++){
        //         for(int j=1; j<=M; j++){
        //             cout << board[i][j] << " ";
        //         }
        //         cout << "\n";
        //     }
        //     cout << "\n";
        // }
    }
    // for(int i=1; i<=N; i++){
    //     for(int j=1; j<=M; j++){
    //         cout << seq[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    
    int max_idx = 0;
    for(int i=1; i<turrents.size(); i++)
        if(turrents[i].attack>turrents[max_idx].attack)
            max_idx = i;
    cout << turrents[max_idx].attack;

    return 0;
}

void Input(){
    cin >> N >> M >> K;
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            cin >> board[i][j];
            if(board[i][j]){
                Turrent turrent;
                turrent.x = i;
                turrent.y = j;
                turrent.attack = board[i][j];
                turrent.last_attack = 0;
                turrents.push_back(turrent);   
            }
        }
    }
}

//공격자 찾은 후 공격자 공격력 증가
int FindSubject(){
    int s_idx=0;
    for(int i=0; i<turrents.size(); i++){
        if(turrents[i].attack>0){
            s_idx = i;
            break;
        }
    }

    int min_idx = s_idx;
    for(int i=0; i<turrents.size(); i++){
        if(turrents[i].attack<=0)
            continue;
        if(turrents[i].attack<turrents[min_idx].attack)
            min_idx = i;
        else if(turrents[i].attack==turrents[min_idx].attack){
            if(turrents[i].last_attack>turrents[min_idx].last_attack)
                min_idx = i;
            else if(turrents[i].last_attack==turrents[min_idx].last_attack){
                if(turrents[i].x+turrents[i].y>turrents[min_idx].x>turrents[min_idx].y)
                    min_idx = i;
                else if(turrents[i].x+turrents[i].y==turrents[min_idx].x+turrents[min_idx].y){
                    if(turrents[i].y>turrents[min_idx].y)
                        min_idx = i;
                }
            }
        }
    }
    // turrents[min_idx].attack+=N+M;
    // board[turrents[min_idx].x][turrents[min_idx].y]+=N+M;
    // turrents[min_idx].last_attack=R;
    return min_idx;
}

//공격대상자 찾기
int FindObject(int subject){
    int s_idx=0;
    for(int i=0; i<turrents.size(); i++){
        if(subject == i)
            continue;
        if(turrents[i].attack>0){
            s_idx = i;
            break;
        }
    }

    int max_idx=s_idx;
    for(int i=0; i<turrents.size(); i++){
        if(turrents[i].attack<=0 || i==subject)
            continue;
        if(turrents[i].attack>turrents[max_idx].attack)
            max_idx = i;
        else if(turrents[i].attack==turrents[max_idx].attack){
            if(turrents[i].last_attack<turrents[max_idx].last_attack)
                max_idx = i;
            else if(turrents[i].last_attack==turrents[max_idx].last_attack){
                if(turrents[i].x+turrents[i].y<turrents[max_idx].x+turrents[max_idx].y)
                    max_idx = i;
                else if(turrents[i].x+turrents[i].y==turrents[max_idx].x+turrents[max_idx].y){
                    if(turrents[i].y<turrents[max_idx].y)
                        max_idx = i;
                }
            }
        }
    }
    return max_idx;
}

bool IsCanLazerAtk(int attacker, int obect){
    //BFS로 출발지부터 도착지로 갈 수 있는지 확인
    queue<int> q;
    q.push(attacker);

    memset(seq,0,sizeof(seq));
    // fill(&seq[0][0],&seq[N][M]+1,0);
    seq[turrents[attacker].x][turrents[attacker].y] = 1;

    while(q.size()){
        int cur = q.front(); q.pop();
    
        for(int i=0; i<4; i++){
            int nx = turrents[cur].x+dx[i];
            int ny = turrents[cur].y+dy[i];
            if(nx==0) nx = N;
            else if(nx==N+1) nx = 1;
            if(ny==0) ny = M;
            else if(ny==M+1) ny = 1;

            if(board[nx][ny]>0){
                int next_idx;
                for(int i=0; i<turrents.size(); i++){
                    if(cur==i)
                        continue;
                    if(turrents[i].x==nx&&turrents[i].y==ny)
                        next_idx = i;
                }
                if(seq[nx][ny])
                    continue;

                back_x[nx][ny] = turrents[cur].x;
                back_y[nx][ny] = turrents[cur].y;
                seq[nx][ny] = seq[turrents[cur].x][turrents[cur].y]+1;
                if(next_idx==obect)
                    return true;
                q.push(next_idx);
            }
        }
    }
    return false;
}

bool TryLazerAtk(int attacker, int obect){
    //lazer 공격 가능시 공격력 업데이트
    if(IsCanLazerAtk(attacker,obect)){
        int dmg = turrents[attacker].attack;
        board[turrents[obect].x][turrents[obect].y]-=dmg;
        int cx = turrents[obect].x;
        int cy = turrents[obect].y;
        bool flag = true;
        //board만 업데이트
        int nx = back_x[cx][cy];
        int ny = back_y[cx][cy];
        while(!(nx==turrents[attacker].x&&ny==turrents[attacker].y)){
            board[nx][ny] -= dmg/2;
            int next_cx = nx;
            int next_cy = ny;
            nx = back_x[next_cx][next_cy];
            ny = back_y[next_cx][next_cy];
        }

        //turrent배열값에서 업데이트
        for(int i=0; i<turrents.size(); i++){
            //살아남은 애들 +1, 맞은애들 데미지 업데이트
            if(i==attacker) continue;
            if(turrents[i].attack == board[turrents[i].x][turrents[i].y] && turrents[i].attack>0){
                board[turrents[i].x][turrents[i].y] += 1;
                turrents[i].attack += 1;
            }
            else if(turrents[i].attack != board[turrents[i].x][turrents[i].y]){
                turrents[i].attack = board[turrents[i].x][turrents[i].y];
            }
        }

        return true;    
    }
    return false;
}

void MissileAtk(int attacker, int obect){
    int ox = turrents[obect].x;
    int oy = turrents[obect].y;
    int ax = turrents[attacker].x;
    int ay = turrents[attacker].y;
    int dmg = turrents[attacker].attack;

    board[ox][oy] -= dmg;
    for(int i=ox-1; i<=ox+1; i++){
        for(int j=oy-1; j<=oy+1; j++){
            int nx = i;
            int ny = j;
            if(nx==0) nx = N;
            else if(nx==N+1) nx = 1;
            if(ny==0) ny = M;
            else if(ny==M+1) ny = 1;
            
            if(board[nx][ny]<=0 || (nx==ox && ny==oy) || (nx==ax && ny==ay))
                continue;
            board[nx][ny] -= dmg/2;
        }
    }

    //turrent배열값에서 업데이트
    for(int i=0; i<turrents.size(); i++){
        //살아남은 애들 +1, 맞은애들 데미지 업데이트
        if(i==attacker) continue;
        if(turrents[i].attack == board[turrents[i].x][turrents[i].y] && turrents[i].attack>0){
            board[turrents[i].x][turrents[i].y] += 1;
            turrents[i].attack += 1;
        }
        else if(turrents[i].attack != board[turrents[i].x][turrents[i].y]){
            turrents[i].attack = board[turrents[i].x][turrents[i].y];
        }
    }
}