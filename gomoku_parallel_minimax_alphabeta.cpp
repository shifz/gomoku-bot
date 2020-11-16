//
// Created by Jiaxin Ye on 11/15/20.
//
#include "gomoku_utils.h"
#include <omp.h>
//#include <bits/stdc++.h>
using namespace std;

pair<int, int> minimax_parallel_distributed(vector<vector<int> >& board, int depth, int alpha, int beta, bool debug){
    vector<pair<int,int> > moves;
    get_moves(board,moves);
    int global_best_value = INT_MIN;
    pair<int, int> global_best_move = moves[0];
    #pragma omp parallel
    {
        int local_best_value = INT_MIN;
        pair<int, int> local_best_move = moves[0];
        int local_alpha = alpha;
        int local_beta = beta;
        vector<vector<int> > local_board = board;

        #pragma omp for schedule(static)
        for (size_t i = 0; i < moves.size(); i++){
            int value = 0;
            local_board[moves[i].first][moves[i].second] = 1;
            value = evaluate_states(local_board, depth - 1, false, local_alpha, local_beta, debug);
            if (value > local_best_value){
                local_best_value = value;
                local_best_move = moves[i];
                local_alpha = local_best_value;
            }
            local_board[moves[i].first][moves[i].second] = -1;
        }

        #pragma omp critical
        {
            if (local_best_value > global_best_value){
                global_best_value = local_best_value;
                global_best_move = local_best_move;
            }
        }

    }
    return global_best_move;
}

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
            value = max(minimax(board, depth-1, false, alpha, beta, debug), value);
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
        for (int i=0;i<moves.size();i++){
            board[moves[i].first][moves[i].second]=0;
            value=min(minimax(board, depth - 1, true, alpha, beta, debug), value);
            board[moves[i].first][moves[i].second] = -1;
            beta = min(beta, value);
            if (alpha >= beta){
                break;
            }
        }
        return value;
    }
}

pair<int,int> search_next_move(vector<vector<int> >& board){
    return minimax_parallel_distributed(board, 30, INT_MIN, INT_MAX, false);
}