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
struct Point {
    int x, y;
};

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
int evaluate(int board[][8],int x,int y,int ma){
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
                //ans= ans+Check_Straight_Army(x,y,i,ma);
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
                //ans= ans+Check_Straight_Army(x,y,i,ma);
            }
            return (ans+envaluebase[x][y]*10);
                //return ans;
    }
}
int heuristic(int board[][8],int x,int y, int whoseTurn)
{
    char opponent = 1;
        if (whoseTurn == 1)
        opponent = 0;

    int ourScore = evaluate(board, x,y,whoseTurn);
    int opponentScore = evaluate(board,x,y, opponent);
    return (ourScore - opponentScore);
}
int minimaxValue(int board[][8], int originalTurn, int currentTurn, int x,int y,int searchPly){
    int n_valid_spots = next_valid_spots.size();
    if ((searchPly == 5) || n_valid_spots==0) {// Change to desired ply lookahead{
        return heuristic(board,x,y ,originalTurn);
    }
    int moveX[60], moveY[60];
    int numMoves;
    int opponent = 0;
    if (currentTurn == 0)
    opponent = 0;
    //getMoveList(board, moveX, moveY, numMoves, currentTurn);
    if (n_valid_spots == 0) {// if no moves skip to next player's turn{
        return minimaxValue(board, originalTurn, opponent,x,y, searchPly + 1);
    }
    else{
    // Remember the best move
        int bestMoveVal = -99999; // for finding max
        if (originalTurn != currentTurn)
        bestMoveVal = 99999; // for finding min
            // Try out every single move
            for (int i = 0; i < n_valid_spots; i++){
            // Apply the move to a new board
               int  tempBoard[8][8];
                //copyBoard(board, tempBoard);
                //makeMove(tempBoard, moveX[i], moveY[i], currentTurn);
                for(int a=0;a<8;a++){
                for(int b=0;b<8;b++){
                   tempBoard[a][b]= board[a][b];
                }
                }
                Point p = next_valid_spots[i];
                // Recursive call
                int val = minimaxValue(tempBoard, originalTurn, opponent,p.x,p.y,
                 searchPly + 1);
                // Remember best move
                if (originalTurn == currentTurn){
                // Remember max if it's the originator's turn
                    if (val > bestMoveVal)
                    bestMoveVal = val;
                }
                else{
                // Remember min if it's opponent turn
                    if (val < bestMoveVal)
                    bestMoveVal = val;
                }
            }
        return bestMoveVal;
        }
    return -1; // Should never get here
}
Point minimaxDecision(int board[][8], int whoseTurn, int &x, int &y,std::ofstream& fout)
{
    Point fp;
    int moveX[60], moveY[60];
    int numMoves;
    char opponent = 0;
    if (whoseTurn == 0)
    opponent = 1;
//    getMoveList(board, moveX, moveY, numMoves, whoseTurn);
    int n_valid_spots = next_valid_spots.size();
    //srand(time(NULL));
    if (numMoves == 0) {
        x = -1;
        y = -1;
    }
    else{
        // Remember the best move
        int bestMoveVal = -99999;
        int bestX = moveX[0], bestY = moveY[0];
        // Try out every single move
        for (int i = 0; i < n_valid_spots; i++)
        {
        // Apply the move to a new board
            int tempBoard[8][8];
            //copyBoard(board, tempBoard);
            for(int a=0;a<8;a++){
                for(int b=0;b<8;b++){
                   tempBoard[a][b]= board[a][b];
                }
            }
            Point p = next_valid_spots[i];
            //makeMove(tempBoard, moveX[i], moveY[i], whoseTurn);
            // Recursive call, initial search ply = 1
            int val = minimaxValue(tempBoard, whoseTurn, opponent,p.x,p.y ,1);
            // Remember best move
            if (val > bestMoveVal){
                bestMoveVal = val;
                bestX = p.x;
                bestY = p.y;
                fp=p;
            }
        }
        // Return the best x/y
        x = bestX;
        y = bestY;
       // fout << bestX << " " << bestY << std::endl;
       //     fout.flush();
    }
    return fp;
}


void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    int a=-100;
    int b=100;
    int finaly=0,finalx=0;
    //int v=0;
    int board1[8][8];
    for(int a=0;a<8;a++){
        for(int b=0;b<8;b++){
            board1[a][b]=board[a][b];
        }
    }

    for(int i=0;i<n_valid_spots;i++){
        Point p = next_valid_spots[i];
        Point v = minimaxDecision( board1 ,player , p.x ,p.y,fout);
       //v= negamax(p.x,p.y, 3, a, b,player);
      // v=evaluate(p.x,p.y,1);
        //for(int )
        finalx=v.x;finaly=v.y;
    }
    fout <<finalx << " " << finaly << std::endl;
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
