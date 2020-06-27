#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<string.h>


struct Point {
    int x, y;
};
int directionx[8]={0,1,0,-1,1,1,-1,-1};
int directiony[8]={1,0,-1,0,-1,1,1,-1};
//基本值 哪個位子好
int valuebase[8][8]={200,-100,50,50,50,50,-100,200,
                    -100,-200,10,10,10,10,-200,-100,
                    50,10,10,5,5,10,10,50,
                    50,10,5,5,5,5,10,50,
                    50,10,5,5,5,5,10,50,
                    50,10,10,5,5,10,10,50,
                    -100,-200,10,10,10,10,-200,-100,
                    200,-100,50,50,50,50,-100,200
};
//bool is_spot_valid(Point center) const;
int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}


void read_valid_spots(std::ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}

void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    int finalx,finaly;
    for(int times=0;times<10;times++){
    int memaxbase=-300;
    for(int i=0;i<n_valid_spots;i++){
        int x=0,y=0;
        Point p = next_valid_spots[i];
        x=p.x;y=p.y;
        if(memaxbase<valuebase[x][y]){
            memaxbase=valuebase[x][y];
            finalx=x;
            finaly=y;
        }
    }
    int enemymap1[8][8],enemymap2[8][8];
    for(int a=0;a<8;a++){
        for(int b=0;b<8;b++){
            enemymap1[a][b]=board[a][b];
            enemymap2[a][b]=board[a][b];
                    }
    }
    player=player-3;
    enemymap1[finalx][finaly]=player;
    for(int a=0;a<8;a++){
        for(int b=0;b<8;b++){
            board[a][b]=enemymap1[a][b];
        }
    }
    //std::vector<Point> get_valid_spots() const {
        std::vector<Point> valid_spots;
        for (int q = 0; q < 8; q++) {
            for (int w = 0; w < 8; w++) {
                Point pp;
                 pp.x =q;
                 pp.y =w;
                if (board[q][w] != 0)
                    continue;
               // if (is_spot_valid(pp))
                    valid_spots.push_back(pp);
            }
        }
       // return valid_spots;
    //}
    /*
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
    }
    // Remember to flush the output to ensure the last action is written to file.
    fout << finalx<< " " << finaly << std::endl;
    fout.flush();*/
}}

bool is_spot_valid(Point center)  {
        int can=0;
        can=board[center.x][center.y];
        if (can != 0)
            return false;
        //for (Point dir: directions) {
            // Move along the direction while testing.
        for(int i=0;i<8;i++){
            Point p ;
            p.x=center.x+directionx[i];
            p.y=center.y+directionx[i];
            if (!is_disc_at(p, get_next_player(player-3)))
                continue;
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p,playe-3))
                    return true;
                p = p + dir;
            }
        //}
        return false;
}

//判斷放這格可以把幾個敵人變自己的
int countenemy(int mex,int mey){

}
//最後確定值
int finalvalue(int base,int counte){

}


int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
