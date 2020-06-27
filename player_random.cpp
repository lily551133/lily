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
const int d[8][2] =
{
    -1, 0,
    1, 0,
     0, -1,
      0, 1,
    -1, -1,
     -1, 1,
      1, -1,
       1, 1
};
const int dr[8] = { 0, 0, 1, 1, 1, -1, -1, -1 }, dc[8] = { 1, -1, 0, 1, -1, 0, 1, -1 };
int valuebase[8][8]={20,1,10,10,10,10,1,20,
                    1,0,5,5,5,5,0,1,
                    10,5,5,5,5,5,5,10,
                    10,5,5,5,5,5,5,10,
                    10,5,5,5,5,5,5,10,
                    10,5,5,5,5,5,5,10,
                    1,0,5,5,5,5,0,1,
                    20,1,10,10,10,10,1,20
};
int envaluebase[8][8]={20,1,10,10,10,10,1,20,
                    1,1,5,5,5,5,1,1,
                    10,5,5,5,5,5,5,10,
                    10,5,5,5,5,5,5,10,
                    10,5,5,5,5,5,5,10,
                    10,5,5,5,5,5,5,10,
                    1,1,5,5,5,5,1,1,
                    20,1,10,10,10,10,1,20
};

/*
int valuebase[8][8]={200,10,70,70,70,70,10,200,
                    10,1,50,50,50,50,1,10,
                    70,50,50,50,50,50,50,70,
                    70,50,50,50,50,50,50,70,
                    70,50,50,50,50,50,50,70,
                    70,50,50,50,50,50,50,70,
                    10,1,50,50,50,50,1,10,
                    200,10,70,70,70,70,10,200
};*/
/*
int valuebase[8][8]={0,70,10,10,10,10,70,0,
                    70,1000,50,50,50,50,1000,70,
                    10,50,50,50,50,50,50,10,
                    10,50,50,50,50,50,50,10,
                    10,50,50,50,50,50,50,10,
                    10,50,50,50,50,50,50,10,
                    70,1000,50,50,50,50,1000,70,
                    0,70,10,10,10,10,70,0
};
*/
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


/*
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Point {
    int x, y;
};

int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;
*/
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
int In_Board(int x, int y){
    if( x >= 0 && x < 8 && y >= 0 && y < 8 )
        return 1;
    else
        return 0;
}
int Check_Straight_Army(int x, int y, int d, int ma){
    int me = ma;
    int army = 3 - me;
    int army_count=0;
    int found_flag=0;
    int flag[ 8 ][ 8 ] = {{0}};

    int i, j;
    int tx, ty;

    tx = x;
    ty = y;

    for(i=0 ; i<8 ; i++){
        tx += dr[d];
        ty += dc[d];

        if(In_Board(tx,ty) ){
            if( board[tx][ty] == army ){
                army_count ++;
                flag[tx][ty] = 1;
            }
            else if( board[tx][ty] == me){
                found_flag = 1;
                break;
            }
            else
                break;
        }
        else
            break;
    }

    if( (found_flag == 1) && (army_count > 0))
        return army_count;

    else return 0;
}



int evaluate(int x,int y,int ma){
    int me,en;
    int cont;
    if(ma==1){

    if(board[0][0]==player)
                valuebase[1][0]=valuebase[0][1]=valuebase[1][1]=8;
            else if(board[7][0]==player)
                valuebase[6][0]=valuebase[7][1]=valuebase[6][1]=8;
            else if(board[0][7]==player)
                valuebase[0][6]=valuebase[1][7]=valuebase[1][6]=8;
            else if(board[7][7]==player)
                valuebase[7][6]=valuebase[6][7]=valuebase[6][6]=8;

                //return valuebase[x][y];*/
                int ans=0;
                for(int i=0;i<8;i++){
                ans= ans+Check_Straight_Army(x,y,i,ma);
                }
                return (ans+valuebase[x][y]*10);
                //return ans;
    }

    else{
        if(board[0][0]==3-player)
                envaluebase[1][0]=envaluebase[0][1]=envaluebase[1][1]=8;
            else if(board[7][0]==3-player)
                envaluebase[6][0]=envaluebase[7][1]=envaluebase[6][1]=8;
            else if(board[0][7]==3-player)
                envaluebase[0][6]=envaluebase[1][7]=envaluebase[1][6]=8;
            else if(board[7][7]==3-player)
                envaluebase[7][6]=envaluebase[6][7]=envaluebase[6][6]=8;

                //return -envaluebase[x][y];
                int ans=0;
                for(int i=0;i<8;i++){
                ans= ans+Check_Straight_Army(x,y,i,ma);
                }
                return (ans+envaluebase[x][y]*10);
                //return ans;
    }
}
int minimax(int x,int y, int depth, int a, int b, bool max)
{
    int n_valid_spots = next_valid_spots.size();
    if (depth == 0||n_valid_spots==0) {
        return evaluate(x,y,max);
    }
    if (max) {
        for (int i=0;i<n_valid_spots;i++) {
            Point p = next_valid_spots[i];
                a = std::max(a, minimax(p.x,p.y, depth - 1, a, b, false));
                if (b <= a)
                return b;
        }// £] cutoff.
        return a;
    }
    else {
        for (int i=0;i<n_valid_spots;i++) {
           Point p = next_valid_spots[i];
             b = std::min(b, minimax(p.x,p.y, depth - 1, a, b, true));
            if (b <= a)
                return a; // £\ cutoff.
        }
        return b;
        //return b;
    }
}
int negamax(int x,int y, int depth, int a, int b,int play)
{
    int n_valid_spots = next_valid_spots.size();
    if (depth == 0||n_valid_spots==0) {
        return evaluate(x,y,play);
    }
    //std::vector<GameState> possibleMoves;
    //state.findPossibleMoves(possibleMoves);
   for (int i=0;i<n_valid_spots;i++) {
            Point p = next_valid_spots[i];
        a = std::max(a, -negamax(p.x,p.y, depth - 1, -b, -a,3-player));
        if (b <= a)
            return b; // £] cutoff.
    }
    return a;
}
void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    int a=-100;
    int b=100;
    int finaly=0,finalx=0;
    int v=0;
    for(int i=0;i<n_valid_spots;i++){
        Point p = next_valid_spots[i];
        v = minimax(p.x,p.y, 2, a, b, false);
       //v= negamax(p.x,p.y, 3, a, b,player);
      // v=evaluate(p.x,p.y,1);
        if (v > a) {
            a = v;
            finalx=p.x;
            finaly=p.y;

        }
    }
    fout << finalx << " " << finaly << std::endl;
            fout.flush();

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
