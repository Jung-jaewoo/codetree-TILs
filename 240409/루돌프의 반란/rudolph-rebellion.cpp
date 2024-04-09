#include <iostream>
#include <vector>

#define INF 987654321

using namespace std;

void input();
void checkCrush(int index, int direction);
void moveR();
void moveS(int index);

int N,M,P,C,D;
int map[51][51];
int Rr, Rc, Sr[31], Sc[31];
int point[31];
int alive[31]; //0 죽음 1살다 2기절
int dx[8] = {-1,-1,0,1,1,1,0,-1};
int dy[8] = {0,1,1,1,0,-1,-1,-1};
int distanceRS[31];

int main() {

    //입력
    input();
    
    while(M--){
        //루돌프 산타 움직임
        moveR();
        for(int i=1; i<=P; i++){
            moveS(i);
        }
        //라운드 끝 살아있는 산타 +1점
        bool gameEnd=true;
        for(int i=1; i<=P; i++){
            if(alive[i]==0)
                continue;
            gameEnd=false;
            point[i]+=1;
        }

        if(gameEnd)
            break;
    }

    for(int i=1; i<=P; i++)
        cout << point[i] << " ";

    return 0;
}

void input(){
    cin >> N >> M >> P >> C >> D;
    cin >> Rr >> Rc;

    int tmp;
    for(int i=1; i<=P; i++){
        cin >> tmp;
        cin >> Sr[tmp] >> Sc[tmp];
        alive[i] = 1;
    }
}

void checkCrush(int index, int direction){
    if(Sr[index]<1||Sr[index]>N||Sc[index]<1||Sc[index]>N){
        alive[index]=0;
        return;
    }
    for(int i=1; i<=P; i++){
        if(alive[i]&&index!=i){
            if(Sr[index]==Sr[i]&&Sc[index]==Sc[i]){
                Sr[i]+=dx[direction];
                Sc[i]+=dy[direction];
                checkCrush(index,direction);
                break;
            }
        }
    }
}

void moveR(){
    //거리계산
    for(int i=1; i<=P; i++){
        if(alive[i]==0){
            distanceRS[i] = INF;
            continue;
        }
        distanceRS[i] = (Rr-Sr[i])*(Rr-Sr[i])+(Rc-Sc[i])*(Rc-Sc[i]);
    }
    
    //최소값찾기
    int minIndex, tmp=INF;
    for(int i=1; i<=P; i++){
        if(distanceRS[i]<tmp){
            minIndex = i;
            tmp = distanceRS[i];
        }
        else if(distanceRS[i]==tmp)
            if(Sr[minIndex]<Sr[i] || (Sr[minIndex]==Sr[i]&&Sc[minIndex]<Sc[i]))
                minIndex = i;
    }
    
    //이동방향찾기
    int direction;
    if(Rc==Sc[minIndex]){
        //위로
        if(Rr>Sr[minIndex])
            direction = 0;
        //아래로 
        if(Rr<Sr[minIndex])
            direction = 4;
    }
    else if(Rr==Sr[minIndex]){
        //왼쪽으로
        if(Rc>Sc[minIndex])
            direction = 6;
        //오른쪽으로
        if(Rc<Sc[minIndex])
            direction = 2;
    }
    else if(Rc<Sc[minIndex]){
        //오른쪽위로
        if(Rr>Sr[minIndex])
            direction = 1;
        //오른쪽아래로
        if(Rr<Sr[minIndex])
            direction = 3;
    }
    else if(Rc>Sc[minIndex]){
        //왼쪽위로
        if(Rr>Sr[minIndex])
            direction = 7;
        if(Rr<Sr[minIndex])
            direction = 5;
    }
    Rr += dx[direction];
    Rc += dy[direction];

    //충돌발생
    if(distanceRS[minIndex]<=2){
        Sr[minIndex] += C*dx[direction];
        Sc[minIndex] += C*dy[direction];
        point[minIndex] += C;
        alive[minIndex] = 3;
        checkCrush(minIndex,direction);
    }
}

bool isCanGo(int index, int direction){
    int nSr = Sr[index] + dx[direction];
    int nSc = Sc[index] + dy[direction];
    if(nSr<1 || nSr>N || nSc<1 || nSc>N)
        return false;
    for(int i=1; i<=P; i++){
        if(i==index)
            continue;
        if(nSr==Sr[i]&&nSc==Sc[i])
            return false;
    }
    return true;
}

void moveS(int index){
    if(alive[index]==0)
        return;
    if(alive[index]==3){
        alive[index]=2;
        return;
    }
    if(alive[index]==2){
        alive[index]=1;
        return;
    }
    
    //갈수있는 방향별 거리초기화
    int distance[4], bef_distance, minIndex=-1, tmp=INF;
    for(int i=0; i<4; i++){
        if(isCanGo(index,i*2))
            distance[i] = (Sr[index]+dx[i*2]-Rr)*(Sr[index]+dx[i*2]-Rr)+(Sc[index]+dy[i*2]-Rc)*(Sc[index]+dy[i*2]-Rc);
        else
            distance[i] = INF;
        if(distance[i]<tmp){
            minIndex=i;
            tmp = distance[i];
        }
    }

    bef_distance = (Sr[index]-Rr)*(Sr[index]-Rr)+(Sc[index]-Rc)*(Sc[index]-Rc);
    if(minIndex!=-1 && distance[minIndex]<bef_distance){
        Sr[index]+=dx[minIndex*2];
        Sc[index]+=dy[minIndex*2];
    }

    if(distance[minIndex]==0){
        Sr[index] += D*dx[((minIndex+2)%4)*2];
        Sc[index] += D*dy[((minIndex+2)%4)*2];
        point[index] += D;
        alive[index] = 3;
        checkCrush(index,((minIndex+2)%4)*2);
    }
}