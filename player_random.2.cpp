#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <cassert>
int mevaluebase[8][8]={20,1,7,7,7,7,1,20,
                    1,1,5,5,5,5,1,1,
                    7,5,3,3,3,3,5,7,
                    7,5,3,3,3,3,5,7,
                    7,5,3,3,3,3,5,7,
                    7,5,3,3,3,3,5,7,
                    1,1,5,5,5,5,1,1,
                    20,1,7,7,7,7,1,20
};
int valuebase[8][8]={20,1,7,7,7,7,1,20,
                    1,1,5,5,5,5,1,1,
                    7,5,5,5,5,5,5,7,
                    7,5,5,5,5,5,5,7,
                    7,5,5,5,5,5,5,7,
                    7,5,5,5,5,5,5,7,
                    1,1,5,5,5,5,1,1,
                    20,1,7,7,7,7,1,20
};

struct Point {
    int x, y;
	Point() : Point(0, 0) {}
	Point(float x, float y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};
/*
struct Point {
    int x, y;
};
*/
int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;


 bool is_spot_on_board(Point p)  {
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
    }
int get_disc(Point p)  {
        return board[p.x][p.y];
    }
bool is_disc_at(Point p, int disc)  {
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
std::array<Point, 8> directions{{
        Point(-1, -1), Point(-1, 0), Point(-1, 1),
        Point(0, -1), /*{0, 0}, */Point(0, 1),
        Point(1, -1), Point(1, 0), Point(1, 1)
    }};

bool is_spot_valid(Point center)  {
        if (get_disc(center) != 0)
            return false;
        for (Point dir: directions) {
            // Move along the direction while testing.
            Point p = center + dir;
            if (!is_disc_at(p, 3-player))
                continue;
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != 0) {
                if (is_disc_at(p, 3-player))
                    return true;
                p = p + dir;
            }
        }
        return false;
    }
std::vector<Point> get_valid_spots()  {
        std::vector<Point> valid_spots;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                Point p = Point(i, j);
                if (board[i][j] != 0)
                    continue;
                if (is_spot_valid(p))
                    valid_spots.push_back(p);
            }
        }
        return valid_spots;
    }

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
int nextlevel(){
int n_valid_spots = next_valid_spots.size();
    int ans=0;
    int finalx,finaly;
    int mebase=0;
    for(int i=0;i<n_valid_spots;i++){
        int x1=0,y1=0;
        Point p = next_valid_spots[i];
        x1=p.x;y1=p.y;
        mebase=valuebase[x1][y1];

          int enemymap1[8][8],enemymap2[8][8];
            for(int a=0;a<8;a++){
                for(int b=0;b<8;b++){
                    enemymap1[a][b]=board[a][b];
                    enemymap2[a][b]=board[a][b];
                            }
            }
            player=player-3;
            enemymap1[x1][y1]=player;

            for(int a=0;a<8;a++){
                for(int b=0;b<8;b++){
                    board[a][b]=enemymap1[a][b];
                }
            }
            std::vector<Point> valid_spots ;
            valid_spots= get_valid_spots();
            int n = valid_spots.size();
            int enx,eny;
            int enmaxbase=0;
            for(int i2=0;i2<n;i2++){
                int x2=0,y2=0;
                Point p2 = valid_spots[i2];
                x2=p2.x;y2=p2.y;
                if(enmaxbase<valuebase[x2][y2]){
                    enmaxbase=valuebase[x2][y2];
                    enx=x2;
                    eny=y2;
                }
            }

            if(ans<mebase-enmaxbase){
                ans=mebase-enmaxbase;
            }
            for(int a=0;a<8;a++){
                for(int b=0;b<8;b++){
                    board[a][b]=enemymap2[a][b];
                }
            }

        }
            return ans;
    }
void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));

    int ansf=0;
    int finalx,finaly;
    int mebase=0;

    for(int i=0;i<n_valid_spots;i++){
        int ans=0;
        //¦Û¤v³o¼h
        int x=0,y=0;
        Point p = next_valid_spots[i];
        x=p.x;y=p.y;

         //if(x==0||y==0||x==7||y==7){
          int enemymap1[8][8],enemymap2[8][8],prvaluebase[8][8];
            for(int a=0;a<8;a++){
                for(int b=0;b<8;b++){
                    enemymap1[a][b]=board[a][b];
                    enemymap2[a][b]=board[a][b];
                    prvaluebase[a][b]=mevaluebase[a][b];
                            }
            }
            if((x==0&&y==0)||board[0][0]==player)
                mevaluebase[1][0]=mevaluebase[0][1]=mevaluebase[1][1]=8;
            else if((x==7&&y==0)||board[7][0]==player)
                mevaluebase[6][0]=mevaluebase[7][1]=mevaluebase[6][1]=8;
            else if((x==0&&y==7)||board[0][7]==player)
                mevaluebase[0][6]=mevaluebase[1][7]=mevaluebase[1][6]=8;
            else if((x==7&&y==7)||board[7][7]==player)
                mevaluebase[7][6]=mevaluebase[6][7]=mevaluebase[6][6]=8;

            enemymap1[x][y]=player;
            player=player-3;
            for(int a=0;a<8;a++){
                for(int b=0;b<8;b++){
                    board[a][b]=enemymap1[a][b];
                }
            }
            std::vector<Point> valid_spots ;
            valid_spots= get_valid_spots();
            int n = valid_spots.size();
            int enx,eny;
            int enmaxbase=0;
            for(int i2=0;i2<n;i2++){
                int x2=0,y2=0;
                Point p2 = valid_spots[i2];
                x2=p2.x;y2=p2.y;
                if(enmaxbase<valuebase[x2][y2]){
                    enmaxbase=valuebase[x2][y2];
                    enx=x2;
                    eny=y2;
                }
            }
            //for(int i=0;i<n_valid_spots;i++){
            if(ans<mebase-enmaxbase){
                ans=mebase-enmaxbase;
            }/*

        */
            player=player-3;
            for(int a=0;a<8;a++){
                for(int b=0;b<8;b++){
                    board[a][b]=enemymap2[a][b];
                    mevaluebase[a][b]=prvaluebase[a][b];
                }
            }
            int ans2=0;
            ans2=nextlevel();
            if(ansf<ans+ans2){
                ansf=ans+ans2;
                finalx =x;
                finaly =y;

            if(x==0&&y==0)
                mevaluebase[1][0]=mevaluebase[0][1]=mevaluebase[1][1]=8;
            else if(x==7&&y==0)
                mevaluebase[6][0]=mevaluebase[7][1]=mevaluebase[6][1]=8;
            else if(x==0&&y==7)
                mevaluebase[0][6]=mevaluebase[1][7]=mevaluebase[1][6]=8;
            else if(x==7&&y==7)
                mevaluebase[7][6]=mevaluebase[6][7]=mevaluebase[6][6]=8;
            fout << finalx << " " << finaly << std::endl;
            fout.flush();
            }


        }

}
//};

int main(int, char** argv){
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
