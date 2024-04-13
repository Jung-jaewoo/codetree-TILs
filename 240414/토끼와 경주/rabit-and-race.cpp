#include <iostream>
#include <cstring>
#include <queue>
#include <map>
#define fastio ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;

struct Rabbit{
    int x=1, y=1;
    int pid;
    int dist;
    int count = 0;
    long long score = 0;

    bool operator <(const Rabbit &b) const {
        if(count != b.count) return count > b.count;
        if(x + y != b.x + b.y) return x + y > b.x + b.y;
        if(x != b.x) return x > b.x;
        if(y != b.y) return y > b.y;
        return pid > b.pid;
    }
};

int Q, N, M, P, K, S, L;
long long total_sum;
Rabbit rabbits[2001];
bool is_selected[2001];
map<int, int> id_to_idx;
int dx[4] = {-1,0,1,0};  //0상1좌2하3우
int dy[4] = {0,-1,0,1};
void Input();
void StartRace();
bool Cmp(Rabbit a, Rabbit b);
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
            for(int i=1; i<=P; i++){
                cin >> rabbits[i].pid >> rabbits[i].dist;
                id_to_idx[rabbits[i].pid] = i;
            }
            break;
        case 200:
            cin >> K >> S;
            memset(is_selected,false,sizeof(is_selected));
            StartRace();
            AddS();
            break;
        case 300:
            cin >> pid_t >> L;
            ChangeDist(pid_t);
            break;
        case 400:
            long long ans = 0;
            for(int i=1; i<=P; i++)
                ans = max(ans, rabbits[i].score + total_sum) ;
            cout << ans;
            break;
    }
}

void StartRace(){

    priority_queue <Rabbit> rabbit_pq;

    for(int i=1; i<=P; i++)
        rabbit_pq.push(rabbits[i]);
    
    //우선순위 토끼 찾기
    // int max_idx = 1;
    // for(int i=2; i<=P; i++){
    //     if(rabbits[i].count < rabbits[max_idx].count)
    //         max_idx = i;
    //     else if(rabbits[i].count == rabbits[max_idx].count){
    //         if(rabbits[i].x + rabbits[i].y < rabbits[max_idx].x + rabbits[max_idx].y)
    //             max_idx = i;
    //         else if(rabbits[i].x + rabbits[i].y == rabbits[max_idx].x + rabbits[max_idx].y){
    //             if(rabbits[i].x < rabbits[max_idx].x)
    //                 max_idx = i;
    //             else if(rabbits[i].x == rabbits[max_idx].x){
    //                 if(rabbits[i].pid < rabbits[max_idx].pid)
    //                     max_idx = i;
    //             }   
    //         }
    //     }
    // }
    while(K--){
        Rabbit next_rabbit = rabbit_pq.top(); rabbit_pq.pop();
        //이동할 방향 선택
        int max_x=0, max_y=0, nx, ny, ndx, ndy, cx=next_rabbit.x, cy=next_rabbit.y, d=next_rabbit.dist;
        // cout << id_to_idx[next_rabbit.pid] << " ";                      //cout
        for(int i=0; i<4; i++){ //상좌하우
            nx = cx;
            ny = cy;
            int dir = i;

            ndx = d % ((N-2)*2+2);
            ndy = d % ((M-2)*2+2);
            if(dir==0){
                if(nx+ndx*dx[dir]<=0){
                    ndx -= nx - 1;
                    nx = 1;
                    dir = (dir+2)%4;
                    if(ndx>0 && nx+ndx*dx[dir]>N){
                        nx = N;
                        ndx -= N - 1;
                        dir = (dir+2)%4;
                    }
                }
                nx += ndx*dx[dir];
            }else if(dir==1){
                if(ny+ndy*dy[dir]<=0){
                    ndy -= ny - 1;
                    ny = 1;
                    dir = (dir+2)%4;
                    if(ndy>0 && ny+ndy*dy[dir]>M){
                        ny = M;
                        ndy -= M - 1;
                        dir = (dir+2)%4;
                    }
                }
                ny += ndy*dy[dir];
            }else if(dir==2){
                if(nx+ndx*dx[dir]>N){
                    ndx -= N - nx;
                    nx = N;
                    dir = (dir+2)%4;
                    if(ndx>0 && nx+ndx*dx[dir]<=0){
                        nx = 1;
                        ndx -= N - 1;
                        dir = (dir+2)%4;
                    }
                }
                nx += ndx*dx[dir];
            }else if(dir==3){
                if(ny+ndy*dy[dir]>M){
                    ndy -= M - ny;
                    ny = M;
                    dir = (dir+2)%4;
                    if(ndy>0 && ny+ndy*dy[dir]<=0){
                        ny = 1;
                        ndy -= M - 1;
                        dir = (dir+2)%4;
                    }
                }
                ny += ndy*dy[dir];
            }
            // for(int j=0; j<ndx; j++){
            //     if(nx+dx[dir]==0 || nx+dx[dir]==N+1)
            //         dir = (dir + 2) % 4;
            //     nx += dx[dir];
            // }
            // for(int j=0; j<ndy; j++){
            //     if(ny+dy[dir]==0 || ny+dy[dir]==M+1)
            //         dir = (dir + 2) % 4;
            //     ny += dy[dir];
            // }
            // for(int j=0; j<d; j++){
            //     if(nx+dx[dir]==0 || nx+dx[dir]==N+1 || ny+dy[dir]==0 || ny+dy[dir]==M+1)
            //         dir = (dir + 2) % 4;
            //     nx += dx[dir];
            //     ny += dy[dir];
            // }
            // cout << " (" << nx << ny <<") ";                //cout
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

        // cout << max_x << "," << max_y << " ";                //cout
        next_rabbit.x = max_x;
        next_rabbit.y = max_y;
        next_rabbit.count++;
        next_rabbit.score -= max_x + max_y;
        rabbit_pq.push(next_rabbit);

        rabbits[id_to_idx[next_rabbit.pid]].x = max_x;
        rabbits[id_to_idx[next_rabbit.pid]].y = max_y;
        rabbits[id_to_idx[next_rabbit.pid]].count++;
        rabbits[id_to_idx[next_rabbit.pid]].score -= max_x + max_y;
        is_selected[id_to_idx[next_rabbit.pid]] = true;

        total_sum += max_x + max_y;
        
        // for(int i=1; i<=P; i++){                                //cout
        //     cout <<rabbits[i].score<< " ";
        // }
        // cout << "\n";
    }
    
}

bool Cmp(Rabbit a, Rabbit b){
    if(a.x + a.y != b.x + b.y) return a.x + a.y < b.x + b.y;
    if(a.x != b.x) return a.x < b.x;
    if(a.y != b.y) return a.y < b.y;
    return a.pid < b.pid;
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
        if(is_selected[i])
            if(Cmp(rabbits[max_idx],rabbits[i]))
                max_idx = i;
    }

    //뽑힌 토끼 없을때
    if(max_idx==P+1)
        return;

    rabbits[max_idx].score += S;
}

void ChangeDist(int pid_t){
    int idx = id_to_idx[pid_t];
    rabbits[idx].dist *= L;
}