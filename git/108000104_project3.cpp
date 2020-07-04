#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits.h>
#include <algorithm>
#include <cmath>

int valuebase[8][8]={500,0,30,30,30,30,0,500,
                    0,-1,1,1,1,1,-1,0,
                    30,0,0,0,0,0,0,30,
                    30,0,0,0,0,0,0,30,
                    30,0,0,0,0,0,0,30,
                    30,0,0,0,0,0,0,30,
                    0,-1,1,1,1,1,-1,0,
                    500,0,30,30,30,30,0,500
};
using namespace std;

struct Point {
    int x, y;
	Point() : Point(0, 0) {}
	Point(int x, int y) : x(x), y(y) {}
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
const int SIZE = 8;
const int max_depth = 6;
int player;
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};
const array<Point, 8> directions{{
    Point(-1, -1), Point(-1, 0), Point(-1, 1),
    Point(0, -1), /*{0, 0}, */Point(0, 1),
    Point(1, -1), Point(1, 0), Point(1, 1)
}};
const array<Point, 4> corner{{
    Point(0, 0), Point(0, SIZE-1),
    Point(SIZE-1, 0), Point(SIZE-1, SIZE-1)
}};
int get_next_player(int player){
    return 3 - player;
}

class OthelloBoard {
public:
    array<array<int, SIZE>, SIZE> board;
    vector<Point> next_valid_spots;
    array<int, 3> disc_count;
    vector<Point> discs;
    int cur_player, winner = -1;
    bool done;
    OthelloBoard() {
        reset();
    }

    OthelloBoard(array<array<int, SIZE>, SIZE> pre_board) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = pre_board[i][j];
            }
        }
        done = false;
        winner = -1;
    }

    void reset() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = EMPTY;
            }
        }
        board[3][4] = board[4][3] = BLACK;
        board[3][3] = board[4][4] = WHITE;
        disc_count[EMPTY] = 8*8-4;
        disc_count[BLACK] = 2;
        disc_count[WHITE] = 2;
        next_valid_spots = get_valid_spots();
        done = false;
        winner = -1;
    }
    bool is_spot_on_board(Point p){
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
    }
    int get_disc(Point p){
        return board[p.x][p.y];
    }
    void set_disc(Point p, int disc) {
        board[p.x][p.y] = disc;
    }
    bool is_disc_at(Point p, int disc){
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
    //can put this point
    bool is_spot_valid(Point center){
        if (get_disc(center) != EMPTY)
            return false;
        for (Point dir: directions) {
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player))
                    return true;
                p = p + dir;
            }
        }
        return false;
    }
    //change board and count
    void flip_discs(Point center) {
        for (Point dir: directions) {
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            discs.push_back(p);
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player)) {
                    for (Point s: discs)
                        set_disc(s, cur_player);
                    disc_count[cur_player] += discs.size();
                    disc_count[get_next_player(cur_player)] -= discs.size();
                    break;
                }
                discs.push_back(p);
                p = p + dir;
            }
        }
    }
    //how much can put and point
    vector<Point> get_valid_spots(){
        vector<Point> valid_spots;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                Point p = Point(i, j);
                if (board[i][j] != EMPTY)
                    continue;
                if (is_spot_valid(p))
                    valid_spots.push_back(p);
            }
        }
        return valid_spots;
    }
    //put one p and change all
    void put_disc(Point p) {
        if(!is_spot_valid(p)) {
            winner = get_next_player(cur_player);
            done = true;
            return;
        }
        set_disc(p, cur_player);
        disc_count[cur_player]++;
        disc_count[EMPTY]--;
        flip_discs(p);
        cur_player = get_next_player(cur_player);
        next_valid_spots = get_valid_spots();
        if (next_valid_spots.size() == 0) {
            cur_player = get_next_player(cur_player);
            next_valid_spots = get_valid_spots();
            if (next_valid_spots.size() == 0){
                done = true;
                if (disc_count[WHITE] == disc_count[BLACK]) winner = EMPTY;
                else if (disc_count[BLACK] > disc_count[WHITE]) winner = BLACK;
                else winner = WHITE;
            }
        }
        return;
    }
    //
    int purning(Point point, int depth, int a, int b, bool type)
    {

        OthelloBoard NextState(board);
        //現在檢查的   原本的
        NextState.cur_player = get_next_player(cur_player);
        NextState.disc_count[EMPTY] = disc_count[EMPTY];
        NextState.disc_count[cur_player] = disc_count[cur_player];
        NextState.disc_count[get_next_player(cur_player)] = disc_count[get_next_player(cur_player)];
        NextState.put_disc(point);


        if(depth==0 || NextState.done)
            return NextState.heuristic(point);
        if(type){
            int valmax = -99999;
            for(auto p : NextState.next_valid_spots){
                valmax = max( valmax, NextState.purning(p, depth-1, a, b, false));
                a = max(a,  valmax);
                if(b<=a)
                    break;
            }
            return valmax;
        }
        else{
            int valmin = 99999;
            for(auto p : NextState.next_valid_spots){
                valmin = min(valmin, NextState.purning(p, depth-1, a, b, true));
                b = min(b, valmin);
                if(b<=a)
                    break;
            }
            return valmin;
        }

    }
    //
    int heuristic(Point point)
    {   put_disc(point);
        int heuris = 0;
         heuris+=valuebase[point.x][point.y];

        for(auto p: corner){
                for(auto dir: directions){
                    if(!is_spot_on_board(p+dir))
                        continue;
                    if(point==p+dir){
                        if(!is_disc_at(p, cur_player))
                            heuris-=50;
                        else
                            heuris+=50;
                    }
                }
        }


        if(done){
            if(winner==cur_player)
                heuris+=9000;
            else if(winner==get_next_player(cur_player))
                heuris-=9000;
            else
                heuris-=200;
        }

        heuris+=disc_count[cur_player];
        cur_player=get_next_player(cur_player);
        heuris-=next_valid_spots.size()*2;
        cur_player=get_next_player(cur_player);
        return heuris;
    }
};

OthelloBoard beginboard;
//初始 beginboard.cur_player beginboard.disc_count[player] beginboard.disc_count[get_next_player(player)]
void read_board(ifstream& fin) {
    fin >> player;
    beginboard.cur_player = player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> beginboard.board[i][j];
            if(beginboard.board[i][j]==player)
                beginboard.disc_count[player]++;
            else if(beginboard.board[i][j]==get_next_player(player))
                beginboard.disc_count[get_next_player(player)]++;
        }
    }
}
void read_valid_spots(ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        beginboard.next_valid_spots.push_back({x, y});
    }
}
void write_valid_spot(ofstream& fout) {
    srand(time(NULL));
    int valmax=-99999;
    Point finalp = beginboard.next_valid_spots.front();
    for(auto p : beginboard.next_valid_spots){
        int heuris = beginboard.purning(p, 9, -99999, 99999, false);
        if(heuris>valmax ){
           finalp = p;
           valmax = heuris;
        }
    }
    fout << finalp.x << " " << finalp.y << endl;
    fout.flush();
}
int main(int, char** argv) {
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
