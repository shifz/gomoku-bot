#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>
using namespace std;

int BOARD_WIDTH=19;
int W[9]={10000000,1000000,300,10,200,10,150,5,1};
int get_count_index(int count, int block, bool broken){
    if (count == 5){
        return 0;
    }
    else if (count == 4){
        if (block == 0)
            return 1;
        else if (block == 1)
            return 2;
    }
    else if (count == 3){
        if (!broken){
            if (block == 1)
                return 3;
            else if (block == 0){
                return 4;
            }
        }
        else{
            if (block == 1)
                return 5;
            else if (block == 0){
                return 6;
            }
        }
    }
    else if (count == 2 && block == 0){
        return 7;
    }
    else if (count == 1){
        return 8;
    }
    else{
        return -1;
    }
}
// void sort_moves(vector<pair<int,int> >& moves, bool one)
int evaluate_horizontally(const vector<vector<int> >& board, bool one){
    int counts[9]={0};
    for (int i=0;i<BOARD_WIDTH;i++){
        // used for broken threes and broken fours. for example OXX_XX will have 
        // prev_count = 2 and prev_block = 1 when we reached the space in the middle.
        int count=0;
        int blocks=2;
        int index = -1;
        for (int j=0;j<BOARD_WIDTH;j++){
            if (board[i][j] == (one ? 1 : 0)){
                count++;
            }
            else if (board[i][j] == -1){
                if (count > 0){
                    blocks--;
                    index = get_count_index(count, blocks,false);
                    count = 0;
                }
                blocks = 1;
            }
            else if (count > 0){
                index = get_count_index(count, blocks,false);
                count = 0;
                blocks = 2;
            }
            else{
                blocks = 2;
            }
            if (index >= 0){
                counts[index]++;
                index = -1;
            }
        }
        if (count > 0){
            index = get_count_index(count, blocks,false);
            if (index >= 0){
                counts[index]++;
                index = -1;
            }
        }
    }
    int score=0;
    for (int i=0;i<9;i++){
        cout<<counts[i]<<',';
        score+=W[i]*counts[i];
    }
    return score;
}
/*
1. five-in-row,
2. straight-four
3. four-in-row
4. three-in-row with one side blocked
5. three-in-row both sides free
6. broken-three with one side blocked
7. broken-three both sides free
8. two-in-row both sides free
9. single marks
*/
int evaluate_vertically(const vector<vector<int> >& board, bool one){
    int counts[9] = {0};
    for (size_t j = 0; j < BOARD_WIDTH; j++){
        // used for broken threes and broken fours. for example OXX_XX will have
        // prev_count = 2 and prev_block = 1 when we reached the space in the middle.
        int count=0;
        int blocks=2;
        int index = -1;
        for (int i=0;i<BOARD_WIDTH;i++){
            if (board[i][j] == (one ? 1 : 0)){
                count++;
            }
            else if (board[i][j] == -1){
                if (count > 0){
                    blocks--;
                    index = get_count_index(count, blocks,false);
                    count = 0;
                }
                blocks = 1;
            }
            else if (count > 0){
                index = get_count_index(count, blocks,false);
                count = 0;
                blocks = 2;
            }
            else{
                blocks = 2;
            }
            if (index >= 0){
                counts[index]++;
                index = -1;
            }
        }
        if (count > 0){
            index = get_count_index(count, blocks,false);
            if (index >= 0){
                counts[index]++;
                index = -1;
            }
        }
    }
    int score=0;
    for (int i=0;i<9;i++){
        score+=W[i]*counts[i];
    }
    return score;
}

int evaluate_diagnally(const vector<vector<int> >& board, bool one){
    int counts[9] = {0};
    for (int k=0; k <= 2 * (BOARD_WIDTH-1); k++){
        int start = max(0, k - BOARD_WIDTH + 1);
        int end = min(BOARD_WIDTH - 1, k);
        int count=0;
        int blocks=2;
        int index = -1;
        for (int i=start;i<=end;i++){
            int j=k-i;
            if (board[i][j] == (one ? 1 : 0)){
                count++;
            }
            else if (board[i][j] == -1){
                if (count > 0){
                    blocks--;
                    index = get_count_index(count, blocks,false);
                    count = 0;
                }
                blocks = 1;
            }
            else if (count > 0){
                index = get_count_index(count, blocks,false);
                count = 0;
                blocks = 2;
            }
            else{
                blocks = 2;
            }
            if (index >= 0){
                counts[index]++;
                index = -1;
            }
        }
        if (count > 0){
            index = get_count_index(count, blocks,false);
            if (index >= 0){
                counts[index]++;
                index = -1;
            }
        }
    }
    for (int k=1-BOARD_WIDTH; k < BOARD_WIDTH; k++){
        int start = max(0, k);
        int end = min(BOARD_WIDTH + k - 1, BOARD_WIDTH - 1);
        int count=0;
        int blocks=2;
        int index = -1;
        for (int i=start;i<=end;i++){
            int j=i-k;
            if (board[i][j] == (one ? 1 : 0)){
                count++;
            }
            else if (board[i][j] == -1){
                if (count > 0){
                    blocks--;
                    index = get_count_index(count, blocks,false);
                    count = 0;
                }
                blocks = 1;
            }
            else if (count > 0){
                index = get_count_index(count, blocks,false);
                count = 0;
                blocks = 2;
            }
            else{
                blocks = 2;
            }
            if (index >= 0){
                counts[index]++;
                index = -1;
            }
        }
        if (count > 0){
            index = get_count_index(count, blocks,false);
            if (index >= 0){
                counts[index]++;
                index = -1;
            }
        }
    }
    int score=0;
    for (int i=0;i<9;i++){
        score+=W[i]*counts[i];
    }
    return score;
}

void print_board(const vector<vector<int> >& board){
    for (int i = 0;i < BOARD_WIDTH; i++){
        for (int j = 0;j < BOARD_WIDTH;j++){
            if (board[i][j] == -1){
                cout<<' ';
            }
            else{
                if (board[i][j] ==1){
                    cout<<'x';
                }
                else{
                    cout<<'o';
                }
            }
        }
        cout<<endl;
    }
}

void get_moves(const vector<vector<int> >& board, vector<vector<pair<int,int> > >& moves){
    int directions_one[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    int directions_two[16][2]={{2,0},{2,-1},{2,-2},{1,-2},{0,-2},{-1,-2},{-2,-2},{-1,-1},
                                {-2,0},{-2,1},{-2,2},{-1,2},{0,2},{1,2},{2,2},{2,1}};
    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            if (board[i][j] >= 0) continue;
            for (int k = 0; k < 8; k++){
                int neighbour_i=i+directions_one[k][0];
                int neighbour_j=j+directions_one[k][1];
                if (0<=neighbour_i && neighbour_i<BOARD_WIDTH && 
                    0<=neighbour_j && neighbour_j<BOARD_WIDTH &&
                    board[neighbour_i][neighbour_j] >= 0){
                        moves[0].push_back({i,j});
                        break;
                }
            }
            for (int k = 0; k < 16; k++){
                int neighbour_i=i+directions_two[k][0];
                int neighbour_j=j+directions_two[k][1];
                if (0<=neighbour_i && neighbour_i<BOARD_WIDTH && 
                    0<=neighbour_j && neighbour_j<BOARD_WIDTH &&
                    board[neighbour_i][neighbour_j] >= 0){
                        moves[1].push_back({i,j});
                        break;
                }
            }
        }
    }
}
int main(){
    vector<vector<int> > board(19,vector<int>(19,-1));
    vector<int> test_column = {-1,1,1,-1,0,-1,0,1,-1,1,1,1,1,1,0,-1,1,1,1};
    for (size_t i = 0; i < BOARD_WIDTH; i++){
        board[18-i][i] = test_column[i];
    }
    cout << evaluate_diagnally(board,true)<<endl;
    board[10][10]=1;
    board[10][11]=0;
    vector<vector<pair<int,int> > > moves(2,vector<pair<int,int> >());
    get_moves(board,moves);
    
//    vector<uint32_t> board(BOARD_WIDTH,0);
//    vector<uint32_t> occupied(BOARD_WIDTH,0);
//    board[0]=0b0000000000000000000;
//    occupied[0]=0b1101010101010101010;
//    cout<<evaluate_horizontally(board,occupied,false);
//    while(true){
//        string cmd;
//        cout<<"> ";
//        getline(cin,cmd);
//        if (cmd=="print"){
//            print_board(board,occupied);
//        }
//        else if (cmd=="end"){
//            break;
//        }
//        else{
//            stringstream ss;
//            ss.str(cmd);
//            int x;
//            int y;
//            ss>>x>>y;
//            if ((occupied[x]<<y)%2==1){
//                cout<<"The spot is taken"<<endl;
//            }
//            else{
//                occupied[x]+=(1<<y);
//
//            }
//        }
//
//    }
    return 0;
}