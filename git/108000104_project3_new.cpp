#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <cassert>
#include <climits>
using namespace std;
const int SIZE = 8;
int valuebase[8][8]={500,0,30,30,30,30,0,500,
                    0,0,0,0,0,0,0,0,
                    30,0,0,0,0,0,0,30,
                    30,0,0,0,0,0,0,30,
                    30,0,0,0,0,0,0,30,
                    30,0,0,0,0,0,0,30,
                    0,0,0,0,0,0,0,0,
                    500,0,30,30,30,30,0,500
};
std::array<std::array<int, SIZE>, SIZE> board;
std::array<std::array<int, SIZE>, SIZE> Map = {
     30, 15, 15, 15, 15, 15, 15, 30,
     15, 1, 1, 1, 1, 1, 1, 15,
     15, 1, 1, 1, 1, 1, 1, 15,
     15, 1, 1, 1, 1, 1, 1, 15,
     15, 1, 1, 1, 1, 1, 1, 15,
     15, 1, 1, 1, 1, 1, 1, 15,
     15, 1, 1, 1, 1, 1, 1, 15,
     30, 15, 15, 15, 15, 15, 15, 30
};

std::array<int, 3> disc_count = {0,0,0};
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
std::vector<Point> next_valid_spots;
int player;
class OthelloBoard {
public:
    enum SPOT_STATE {
        EMPTY = 0,
        BLACK = 1,
        WHITE = 2
    };
    static const int SIZE = 8;
    const std::array<Point, 8> directions{{
        Point(-1, -1), Point(-1, 0), Point(-1, 1),
        Point(0, -1), Point(0, 1),
        Point(1, -1), Point(1, 0), Point(1, 1)
    }};

const array<Point, 4> corner{{
    Point(0, 0), Point(0, SIZE-1),
    Point(SIZE-1, 0), Point(SIZE-1, SIZE-1)
}};
    std::array<std::array<int, SIZE>, SIZE> board;
    std::vector<Point> next_valid_spots;
    std::array<int, 3> disc_count;

    int cur_player;
    bool done;
    int winner;
private:
    int get_next_player(int player) const {
        return 3 - player;
    }
    bool is_spot_on_board(Point p) const {
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
    }
    int get_disc(Point p) const {
        return board[p.x][p.y];
    }
    void set_disc(Point p, int disc) {
        board[p.x][p.y] = disc;
    }
    bool is_disc_at(Point p, int disc) const {
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
    bool is_spot_valid(Point center) const {
        if (get_disc(center) != EMPTY)
            return false;
        for (Point dir: directions) {
            // Move along the direction while testing.
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
    void flip_discs(Point center) {
        for (Point dir: directions) {
            // Move along the direction while testing.
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            std::vector<Point> discs({p});
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player)) {
                    for (Point s: discs) {
                        set_disc(s, cur_player);
                    }
                    disc_count[cur_player] += discs.size();
                    disc_count[get_next_player(cur_player)] -= discs.size();
                    break;
                }
                discs.push_back(p);
                p = p + dir;
            }
        }
    }
public:
    OthelloBoard() {
        reset();
    }
    void reset() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = EMPTY;
            }
        }
        board[3][4] = board[4][3] = BLACK;
        board[3][3] = board[4][4] = WHITE;
        cur_player = BLACK;
        disc_count[EMPTY] = 8*8-4;
        disc_count[BLACK] = 2;
        disc_count[WHITE] = 2;
        next_valid_spots = get_valid_spots();
        done = false;
        winner = -1;
    }
    std::vector<Point> get_valid_spots() const {
        std::vector<Point> valid_spots;
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
    bool put_disc(Point p) {
        if(!is_spot_valid(p)) {
            winner = get_next_player(cur_player);
            done = true;
            return false;
        }
        set_disc(p, cur_player);
        disc_count[cur_player]++;
        disc_count[EMPTY]--;
        flip_discs(p);
        // Give control to the other player.
        cur_player = get_next_player(cur_player);
        next_valid_spots = get_valid_spots();
        // Check Win
        if (next_valid_spots.size() == 0) {
            cur_player = get_next_player(cur_player);
            next_valid_spots = get_valid_spots();
            if (next_valid_spots.size() == 0) {
                // Game ends
                done = true;
                int white_discs = disc_count[WHITE];
                int black_discs = disc_count[BLACK];
                if (white_discs == black_discs) winner = EMPTY;
                else if (black_discs > white_discs) winner = BLACK;
                else winner = WHITE;
            }
        }
        return true;
    }


int heuristic() const{
        int heuris = 0;
        if(done){
            if(winner==player)
                heuris+=9000;
            else if(winner==3-player)
                heuris-=9000;
            else
                heuris-=200;
        }
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if(board[i][j] == player)
                    heuris+= valuebase[i][j];
                else if(board[i][j] == 3 - player)
                    heuris-= valuebase[i][j];

                Point point;
                point.x=i;point.y=j;
                for(auto p: corner){
                for(auto dir: directions){
                    if(!is_spot_on_board(p+dir))
                        continue;
                    if(point==p+dir){
                        if(!is_disc_at(p,player))
                            heuris-=50;
                        else
                            heuris+=50;
                    }
                }
                }
            }
        }


        if(cur_player==player)
            heuris += get_valid_spots().size()*2;
        else
            heuris -= get_valid_spots().size()*2;

        return  heuris;   //return最後的分數
    }
};

struct Node{ //用一個struct node用來當作參數傳遞（因為這樣可以同時傳遞point跟分數）
    Point p;
    int score;
    Node(){};
    Node(Point _p, int _score) : p(_p), score(_score) {}
};



//Node minimax(const OthelloBoard, int, int, int, int);
Node maxv(const OthelloBoard, int, int, int); //這個function用來算minimax中max的那一層
Node minv(const OthelloBoard, int, int, int); //用來算minimax中min的那一層
Node maxv(const OthelloBoard NextState, int a, int b, int depth){
    if(depth==0 || NextState.done==1)
        return Node(Point(-1,-1), NextState.heuristic());
    int valmax = -99999;
    Point maxp = Point(0, 0);
        for(auto p : NextState.next_valid_spots){
            OthelloBoard now = NextState;
            now.put_disc(p);
            Node tmp;
            tmp = minv(now, a, b, depth - 1);
            if(tmp.score > valmax){ //如果這次算出來的分數比之前的最大還大，那就改maxscore，跟對應的下一步
            maxp = p;
            valmax = tmp.score;
            }
            a = max(a,  valmax);
            if(b<=a)
                break;
        }
    return Node(maxp, valmax); //回傳分數跟位置（point）
}

OthelloBoard beginboard;
Node minv(const OthelloBoard NextState, int a, int b, int depth){
    if(depth==0 || NextState.done==1)
        return Node(Point(-1,-1), NextState.heuristic());
    int valmin = 99999;
    Point minp = Point(-1, -1);
        for(auto p : NextState.next_valid_spots){
            OthelloBoard now = NextState;
            now.put_disc(p);
            Node tmp;
            tmp = maxv(now, a, b, depth - 1);
            if(tmp.score < valmin){ //如果這次算出來的分數比之前的最大還大，那就改maxscore，跟對應的下一步
            minp = p;
            valmin = tmp.score;
            }
            b = min(b,  valmin);
            if(b<=a)
                break;
        }
    return Node(minp, valmin); //回傳分數跟位置（point）
}
void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    Point finalp=beginboard.next_valid_spots.front();;
    Node ans = maxv(beginboard, -99999,99999, 4);
    finalp = ans.p;
    fout << finalp.x << " " << finalp.y << endl;
    fout.flush();

}
void read_board(ifstream& fin) {
    fin >> player;
    beginboard.cur_player = player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> beginboard.board[i][j];
            if(beginboard.board[i][j]==player)
                beginboard.disc_count[player]++;
            else if(beginboard.board[i][j]==3-player)
                beginboard.disc_count[3-player]++;
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
