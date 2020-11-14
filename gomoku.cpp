#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

int BOARD_WIDTH=19;
int W[11]={10000000,1000000,300,10,200,10,150,5,1,50,40};

int get_count_index(int count, int block, bool broken){
    if (count >= 5){
        return 0;
    }
    else if (count == 4){
        if (block == 0)
            return 1;
        else if (block == 1)
            return 2;
        else
            return 10;
    }
    else if (count == 3){
        if (!broken){
            if (block == 1)
                return 3;
            else if (block == 0)
                return 4;
            else
                return 11;
        }
        else{
            if (block == 1)
                return 5;
            else if (block == 0)
                return 6;
            else
                return -1;
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
    int counts[11]={0};
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
    for (int i=0;i<11;i++){
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
10. four-in-row with two sides blocked
11. three-in-row with two sides blocked
*/
int evaluate_vertically(const vector<vector<int> >& board, bool one){
    int counts[11] = {0};
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
    for (int i=0;i<11;i++){
        score+=W[i]*counts[i];
    }
    return score;
}

int evaluate_diagnally(const vector<vector<int> >& board, bool one){
    int counts[11] = {0};
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
    for (int i=0;i<11;i++){
        score+=W[i]*counts[i];
    }
    return score;
}

int get_board_score(vector<vector<int> >& board, bool one){
    return evaluate_horizontally(board, one) + evaluate_vertically(board, one) + evaluate_diagnally(board, one);
}

bool moveComp(const pair<pair<int, int>, int>& move_1, const pair<pair<int, int>, int>& move_2){
    return move_1.second > move_2.second;
}

void sort_moves(vector<vector<int> >& board, vector<pair<int,int> >& moves, bool one) {
    vector<pair<pair<int, int>, int> > move_with_score;
    // vector<vector<int> > tmp_board = board;
    for (auto m : moves){
        // assert(tmp_board[m.first][m.second] == -1);
        board[m.first][m.second]= (one ? 1 : 0);
        // tmp_board[m.first][m.second] = (one ? 1 : 0);
        // int board_score = get_board_score(tmp_board, one);
        int board_score = get_board_score(board, one);
        move_with_score.push_back(make_pair(m, board_score));
        // tmp_board[m.first][m.second] = -1;
        board[m.first][m.second]=-1;
    }
    sort(move_with_score.begin(), move_with_score.end(), moveComp);
    for (size_t i = 0; i < moves.size(); i++){
        moves[i] = move_with_score[i].first;
    }
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

int minimax(vector<vector<int> >& board, int depth, bool maxplayer, int alpha, int beta, bool debug){
    if (depth == 0){
        int temp=get_board_score(board,true)-get_board_score(board,false);
        if (debug&&temp<0) cout<<temp<<endl;
        return temp;
    }
    if (maxplayer){
        int value=INT_MIN;
        vector<vector<pair<int,int> > > moves(2,vector<pair<int,int> >());
        get_moves(board,moves);
        bool cutoff=false;
        for (int k=0;k<2;k++){
            if (cutoff) break;
            for (int i=0;i<moves[k].size();i++){
                board[moves[k][i].first][moves[k][i].second]=1;
                value=max(minimax(board,depth-1,false,0,0,debug),value);
                board[moves[k][i].first][moves[k][i].second]=-1;
                alpha = max(alpha,value);
                if (alpha>=beta){
                    cutoff=true;
                    break;
                }
            }
        }
        return value;
    }
    else{
        int value=INT_MAX;
        vector<vector<pair<int,int> > > moves(2,vector<pair<int,int> >());
        get_moves(board,moves);
        bool cutoff=false;
        for (int k=0;k<2;k++){
            if (cutoff) break;
            for (int i=0;i<moves[0].size();i++){
                board[moves[0][i].first][moves[0][i].second]=0;
                value=min(minimax(board,depth-1,true,0,0,debug),value);
                board[moves[0][i].first][moves[0][i].second]=-1;
                beta = min(beta,value);
                if (alpha>=beta){
                    cutoff=true;
                    break;
                }
            }
        }
        return value;
    }
}
pair<int,int> search_next_move(vector<vector<int> >& board){
    vector<vector<pair<int,int> > > moves(2,vector<pair<int,int> >());
    get_moves(board,moves);
    int max_value=INT_MIN;
    pair<int,int> best_move;
    for (int k=0;k<moves.size();k++){
        for (int i=0;i<moves[k].size();i++){
            board[moves[k][i].first][moves[k][i].second]=1;
            int value;
            value=minimax(board,20,false,INT_MIN,INT_MAX,false);
            if (value>max_value){
                max_value=value;
                best_move=moves[k][i];
            }
            board[moves[k][i].first][moves[k][i].second]=-1;
        }
    }
    return best_move;
}
int main(){
    vector<vector<int> > board(19,vector<int>(19,-1));
    // vector<int> test_column = {-1,-1,-1,-1,-1,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    // for (size_t i = 0; i < BOARD_WIDTH; i++){
    //     board[0][i] = test_column[i];
    // }
    // cout << evaluate_diagnally(board,true)<<endl;
    // board[10][10]=1;
    // board[10][11]=0;
    // vector<vector<pair<int,int> > > moves(2,vector<pair<int,int> >());
    // get_moves(board,moves);
    // sort_moves(board, moves[0], true);

    // cout<<"sorted: "<<endl;
    // for (auto i : moves[0]){
    //     cout<<i.first<<" "<<i.second<<endl;
    // }
    // pair<int,int> p=search_next_move(board);
    // cout<<p.first<<' '<<p.second<<endl;

//    vector<uint32_t> board(BOARD_WIDTH,0);
//    vector<uint32_t> occupied(BOARD_WIDTH,0);
//    board[0]=0b0000000000000000000;
//    occupied[0]=0b1101010101010101010;
//    cout<<evaluate_horizontally(board,occupied,false);
    // board[10][6]=board[10][11]=board[9][10]=board[11][12]=board[8][9]=0;
    // board[10][7]=board[10][8]=board[10][9]=board[10][10]=1;
    // cout<<get_board_score(board,false);
    // print_board(board);
    // pair<int,int> p=search_next_move(board);
    // cout<<p.first<<' '<<p.second<<endl;
    board[9][9]=1;
    while(true){
        string cmd;
        cout<<"> ";
        getline(cin,cmd);
        if (cmd=="print"){
            print_board(board);
        }
        else if (cmd=="end"){
            break;
        }
        else{
            stringstream ss;
            ss.str(cmd);
            int x;
            int y;
            ss>>x>>y;
            if (board[x][y]!=-1){
                cout<<"The spot is taken"<<endl;
            }
            else{
                board[x][y]=0;
                pair<int,int> p=search_next_move(board);
                cout<<p.first<<' '<<p.second<<endl;
                board[p.first][p.second]=1;
            }
        }
    }
    return 0;
}