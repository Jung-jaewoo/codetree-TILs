#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

int N, M, K;
int wall_map[11][11];
int update_map[11][11];
int per_posi[11][2];
bool per_exit[11];
int ex_x, ex_y;
int dx[4] = {-1,1,0,0}; //상하좌우 순
int dy[4] = {0,0,-1,1};
int total_dist=0;

void Input();
int GetDist(int idx);
bool IsCanGo(int idx, int dir);
void MovePerson(int idx);
void RotateSquare();

int main() {
    Input();

    while(K--){
        for(int i=1; i<=M; i++){
            // cout << "0" << M << K << " ";
            MovePerson(i);
            // cout << i << " " << per_posi[i][0] << per_posi[i][1]  <<"\n";
        }
        int is_all_escaped = true;
        for(int i=1; i<=M; i++){
            if(per_exit[i]==0)
                is_all_escaped=false;
        }
        if(is_all_escaped) break;
        RotateSquare();
        // cout << "(";
        // cout << " " << ex_x << ex_y;
        // for(int i=1; i<=M; i++){
        //         cout << " " << per_posi[i][0] << per_posi[i][1] << per_exit[i]<< " ";
        //     }
        // cout <<")\n\n";
    }
    cout << total_dist << "\n";
    cout << ex_x << " " << ex_y << "\n";
    return 0;
}

void Input(){
    cin >> N >> M >> K;
    for(int i=1; i<=N; i++)
        for(int j=1; j<=N; j++)
            cin >> wall_map[i][j];

    for(int i=1; i<=M; i++)
        cin >> per_posi[i][0] >> per_posi[i][1];
    cin >> ex_x >> ex_y;
}

int GetDist(int idx){
    return abs(per_posi[idx][0]-ex_x)+abs(per_posi[idx][1]-ex_y);
}

bool IsCanGo(int idx, int dir){
    if(per_posi[idx][0]+dx[dir]<1 || per_posi[idx][0]+dx[dir]>N || per_posi[idx][1]+dy[dir] < 1 || per_posi[idx][1]+dy[dir] > N)
        return false;
    if(wall_map[per_posi[idx][0]+dx[dir]][per_posi[idx][1]+dy[dir]])
        return false;
    return true;
}

void MovePerson(int idx){
    if(per_exit[idx])
        return;

    // if(per_posi[idx][0]!=ex_x){
    //     int nx = per_posi[idx][0];
    //     int ny = per_posi[idx][1];

    //     if(ex_x>nx) nx++;
    //     else nx--;

    //     if(!wall_map[nx][ny]){
    //         per_posi[idx][0] = nx;
    //         per_posi[idx][1] = ny;
    //         total_dist++;
    //         if(GetDist(idx)==0) per_exit[idx] = true;
    //         // if(nx==ex_x && ny == ex_y) per_exit[idx] = true;
    //         return;
    //     }
    // }

    // if(per_posi[idx][1]!=ex_y){
    //     int nx = per_posi[idx][0];
    //     int ny = per_posi[idx][1];

    //     if(ex_y>ny) ny++;
    //     else ny--;

    //     if(!wall_map[nx][ny]){
    //         per_posi[idx][0] = nx;
    //         per_posi[idx][1] = ny;
    //         total_dist++;
    //         if(GetDist(idx)==0) per_exit[idx] = true;
    //         // if(nx==ex_x && ny == ex_y) per_exit[idx] = true;
    //         return;
    //     }
    // }

    int dist[4], bef_dist;
    bef_dist = GetDist(idx);

    //각 사분면의 방향으로 갔을때의 출구와의 거리 계산 최솟값 찾기
    int min_idx=-1, min=bef_dist; //전거리보다 작아야함
    for(int i=0; i<4; i++){
        if(IsCanGo(idx, i)){
            dist[i] = abs(per_posi[idx][0]+dx[i]-ex_x)+abs(per_posi[idx][1]+dy[i]-ex_y);
            if(dist[i]<min){
                min_idx = i;
                min = dist[min_idx];
            }
        }
    }
    if(min_idx == -1)
        return;

    per_posi[idx][0] += dx[min_idx];
    per_posi[idx][1] += dy[min_idx];
    total_dist += 1;

    // 탈출체크
    // cout << "Dist" << GetDist(idx) << " ";
    if(GetDist(idx)==0)
        per_exit[idx] = true;
}

bool CheckBox(int x, int y, int sqr_n){
    for(int i=1; i<=M; i++){
        if(per_exit[i])
            continue;
        if(x<=per_posi[i][0] && per_posi[i][0]<x+sqr_n && y<=per_posi[i][1] && per_posi[i][1]<y+sqr_n) 
            return true;
    }
    return false;
}

void RotateSquare(){
    //사각형 찾기
    // int sqr_x, sqr_y, sqr_n;
    // bool flag = false;
    // for(int sz = 2; sz<=N; sz++){
    //     for(int x1 =1; x1 <= N - sz + 1; x1++){
    //         for(int y1 = 1; y1<=N - sz + 1; y1++){
    //             int x2 = x1 + sz - 1;
    //             int y2 = y1 + sz - 1;

    //             if(!(x1 <= ex_x && ex_x <=x2 && y1 <= ex_y && ex_y <= y2)){
    //                 continue;
    //             }

    //             bool is_traveler_in = false;
    //             for(int l = 1; l <= M; l++){
    //                 if(x1 <= per_posi[l][0] && per_posi[l][0] <=x2 && y1 <= per_posi[l][1] && per_posi[l][1]<=y2){
    //                     if(!(per_exit[l]))
    //                         is_traveler_in = true;
    //                 }
    //             }

    //             if(is_traveler_in){
    //                 sqr_x = x1;
    //                 sqr_y = y1;
    //                 sqr_n = sz;
    //                 flag = true;
    //                 break;
    //             }
    //         }
    //         if(flag) break;
    //     }
    //     if(flag) break;
    // }

    bool find = false;
    int sqr_n = 2;
    int sqr_x, sqr_y;
    while(sqr_n<=N){
        for(int i = ex_x - sqr_n + 1; i <= ex_x; i++){
            for(int j = ex_y - sqr_n + 1; j <= ex_y; j++){
                // cout << "ij" << i << j << " n" << sqr_n<< " ";
                if(!(i==ex_x-sqr_n+1||i==ex_x||j==ex_y-sqr_n+1||j==ex_y))
                    continue;
                if(i<1 || i+sqr_n-1>N || j<1 || j+sqr_n-1>N)
                    continue;
                if(CheckBox(i,j,sqr_n)){
                    sqr_x = i;
                    sqr_y = j;
                    find = true;
                    break;
                }
            }
            if(find)
                break;
        }
        if(find)
            break;
        sqr_n+=1;
    }
    if(sqr_n>N){
        cout << "ERR\n";
        return;
    }
        
    //사각형 돌리기 새로운보드에 옮기기
    int zero_i, zero_j, new_i, new_j;
    for(int i=sqr_x; i<sqr_x+sqr_n; i++){
        for(int j=sqr_y; j<sqr_y+sqr_n; j++){
            if(wall_map[i][j]!=0) wall_map[i][j]--;
            zero_i = i-sqr_x; zero_j = j-sqr_y;
            new_i = zero_j; new_j = sqr_n - zero_i - 1;
            update_map[new_i+sqr_x][new_j+sqr_y] = wall_map[i][j];
        }
    }
    //원래보드로 옮기기
    for(int i=sqr_x; i<sqr_x+sqr_n; i++){
        for(int j=sqr_y; j<sqr_y+sqr_n; j++){
            wall_map[i][j] = update_map[i][j];
        }
    }
    //모든 참가자들 및 출구 회전
    for(int i=1; i<=M; i++){
        int x= per_posi[i][0], y=per_posi[i][1];
        if(sqr_x<=x && x<sqr_x+sqr_n && sqr_y<=y && y<sqr_y+sqr_n && !per_exit[i]){
            zero_i = x - sqr_x;  zero_j = y - sqr_y;
            new_i = zero_j;  new_j = sqr_n - zero_i - 1;
            per_posi[i][0] = new_i + sqr_x;
            per_posi[i][1] = new_j + sqr_y;
        }
    }
    zero_i = ex_x - sqr_x;
    zero_j = ex_y - sqr_y;
    new_i = zero_j;
    new_j = sqr_n - zero_i - 1;
    ex_x = new_i + sqr_x;
    ex_y = new_j + sqr_y;
}