//
// Created by Jiaxin Ye on 11/15/20.
//
#include "gomoku_utils.h"
#include <omp.h>
//#include <bits/stdc++.h>
using namespace std;

int evaluate_states(vector<vector<int> >& board, int depth, bool maxplayer, int alpha, int beta, bool debug){
    if (depth == 0){
        int temp = get_board_score(board, true) - get_board_score(board, false);
        if (debug && temp < 0) cout << temp << endl;
        return temp;
    }
    if (maxplayer){
        int value = INT_MIN;
        vector<pair<int,int> > moves;
        get_moves(board,moves);
        for (int i = 0;i < moves.size(); i++){
            board[moves[i].first][moves[i].second] = 1;
            value = max(evaluate_states(board, depth-1, false, alpha, beta, debug), value);
            board[moves[i].first][moves[i].second]=-1;
            alpha = max(alpha,value);
            if (alpha >= beta){
                break;
            }
        }
        return value;
    }
    else{
        int value=INT_MAX;
        vector<pair<int,int> > moves;
        get_moves(board,moves);
        for (int i = 0;i < moves.size(); i++){
            board[moves[i].first][moves[i].second]=0;
            value=min(evaluate_states(board, depth - 1, true, alpha, beta, debug), value);
            board[moves[i].first][moves[i].second] = -1;
            beta = min(beta, value);
            if (alpha >= beta){
                break;
            }
        }
        return value;
    }
}

pair<int, int> minimax_parallel_distributed(vector<vector<int> >& board, int depth, int alpha, int beta, bool debug){
    vector<pair<int,int> > moves;
    get_moves(board,moves);
    int best_value = INT_MIN;
    pair<int, int> best_move = moves[0];
    #pragma omp parallel
    {
        vector<vector<int> > local_board = board;

        #pragma omp for schedule(static)
        for (size_t i = 0; i < moves.size(); i++){
            int value = 0;
            local_board[moves[i].first][moves[i].second] = 1;
            value = evaluate_states(local_board, depth - 1, false, alpha, beta, debug);
            #pragma omp critical
            {
                if (value > best_value){
                    best_value = value;
                    best_move = moves[i];
                    alpha = best_value;
                }
            }
            local_board[moves[i].first][moves[i].second] = -1;
        }
    }
    return best_move;
}

pair<int,int> search_next_move(vector<vector<int> >& board, int depth){
    return minimax_parallel_distributed(board, depth, INT_MIN, INT_MAX, false);
}

int main(){
    vector<vector<int> > board(19,vector<int>(19,-1));
    board[6][10]=board[7][11]=board[8][9]=board[9][6]=board[9][10]=board[10][7]=board[10][8]=board[10][10]=board[11][5]=0;
    board[7][9]=board[8][8]=board[8][10]=board[9][7]=board[9][8]=board[9][9]=board[9][11]=board[10][6]=board[11][7]=1;
    double start=omp_get_wtime();
    pair<int,int> move=search_next_move(board,10);
    double end=omp_get_wtime();
    cout<<move.first<<' '<<move.second<<endl;
    cout<<end-start<<endl;
    return 0;
}