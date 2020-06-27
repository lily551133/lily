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

void read_board(std::ifstream& cin) {
    cin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> board[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& cin) {
    int n_valid_spots;
    cin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        cin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}

void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    int index = (rand() % n_valid_spots);
    Point p = next_valid_spots[index];
    // Remember to flush the output to ensure the last action is written to file.
    fout << p.x << " " << p.y << std::endl;
    //fout << board[0][0] << " " << board[0][1] << std::endl;
    fout.flush();
}
int value(){
    Point now =
}

int main(int, char** argv) {
    std::ifstream cin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(cin);
    read_valid_spots(cin);
    write_valid_spot(fout);
    cin.close();
    fout.close();
    return 0;
}
