//
// Created by Jiaxin Ye on 11/15/20.
//
#include "gomoku_utils.h"
//#include <bits/stdc++.h>
int BOARD_WIDTH=19;
int W[11]={
        10000000,   //0. five-in-row,
        1000000,    //1. straight-four
        1000,       //3. four-in-row
        10,         //4. three-in-row with one side blocked
        800,        //5. three-in-row both sides free
        10,         //6. broken-three with one side blocked
        700,        //7. broken-three both sides free
        10,         //8. two-in-row both sides free
        0,          //9. single marks
        20,         //10. four-in-row with two sides blocked
        10          //11. three-in-row with two sides blocked
};

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
int check_broken(const vector<vector<int> >& board, int count, int block, pair<int,int> pos, pair<int,int> dir, bool one){
    int color=(one? 1:0);
    // if (count==1){
    int next_pos_x=pos.first+dir.first;
    int next_pos_y=pos.second+dir.second;
    int next_count=0;
    int next_block=1;
    while(next_pos_x<BOARD_WIDTH && next_pos_x>=0 &&
          next_pos_y<BOARD_WIDTH && next_pos_y>=0){
        if ( board[next_pos_x][next_pos_y]==color)
            next_count++;
        else if (board[next_pos_x][next_pos_y]==-1){
            next_block=0;
            break;
        }
        else{
            break;
        }
        next_pos_x+=dir.first;
        next_pos_y+=dir.second;
    }
    if (next_count==0){
        return -1;
    }
    if (count+next_count>=4){
        return get_count_index(4,1,false);
    }
    else if (count+next_count<=3){
        return get_count_index(count+next_count,block+next_block,true);
    }

    else return -1;
    // }
}

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
                    if ((index = check_broken(board,count,blocks,{i,j},{0,1},one))==-1){
                        index = get_count_index(count, blocks,false);
                    }
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
                    if ((index = check_broken(board,count,blocks,{i,j},{1,0},one))==-1){
                        index = get_count_index(count, blocks,false);
                    }
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
                    if ((index = check_broken(board,count,blocks,{i,j},{1,-1},one))==-1){
                        index = get_count_index(count, blocks,false);
                    }
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
                    if ((index = check_broken(board,count,blocks,{i,j},{1,1},one))==-1){
                        index = get_count_index(count, blocks,false);
                    }
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

int get_board_score_for_both(vector<vector<int> >& board){
    int black_score=get_board_score(board,true);
    int white_score=get_board_score(board,false);
    if (black_score >= W[0]){
        return INT_MAX;
    }
    else if (white_score >= W[0]){
        return INT_MIN;
    }
    else{
        return black_score-white_score;
    }
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

void get_moves(const vector<vector<int> >& board, vector<pair<int,int> >& moves){
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
                    moves.push_back({i,j});
                    break;
                }
            }
        }
    }
    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            if (board[i][j] >= 0) continue;
            for (int k = 0; k < 16; k++){
                int neighbour_i=i+directions_two[k][0];
                int neighbour_j=j+directions_two[k][1];
                if (0<=neighbour_i && neighbour_i<BOARD_WIDTH &&
                    0<=neighbour_j && neighbour_j<BOARD_WIDTH &&
                    board[neighbour_i][neighbour_j] >= 0){
                    moves.push_back({i,j});
                    break;
                }
            }
        }
    }
}


