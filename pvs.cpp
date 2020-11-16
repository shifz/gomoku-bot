#include "gomoku_utils.h"
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int local_minimax(vector<vector<int> >& board, bool max, int depth, int alpha, int beta){
    vector<pair<int,int>> moves;
    get_moves(board);
    if (max){
        int best_score=INT_MIN;
        int score=0;
        for (int i=0;i<moves.size();i++){
            board[moves[i].first][moves[i].second]=1;
            if (depth==0){
                score=get_board_score(board,true)-get_board_score(baord,false);
            }
            else{
                score=local_minimax(board,false,depth-1,alpha,beta);
            }
            best_score=max(score,best_score);
            if (score>alpha){
                alpha=score;
                if (alpha>beta){
                    board[moves[i].first][moves[i].second]=-1;
                    break;
                }
            }
            board[moves[i].first][moves[i].second]=-1;
        }
        return best_score;
    }
    else{
        int best_score=INT_MAX;
        int score=0;
        for (int i=0;i<moves.size();i++){
            board[moves[i].first][moves[i].second]=0;
            if (depth==0){
                score=get_board_score(board,true)-get_board_score(baord,false);
            }
            else{
                score=local_minimax(board,true,depth-1,alpha,beta);
            }
            best_score=min(score,best_score);
            if (score<beta>){
                beta=score;
                if (alpha>beta){
                    board[moves[i].first][moves[i].second]=-1;
                    break;
                }
            }
            board[moves[i].first][moves[i].second]=-1;
        }
        return best_score;
    }
}
int PVS(vector<vector<int> >& board, bool max, int depth, int alpha, int beta){
    vector<pair<int,int>> moves;
    get_moves(board);
    int global_best_score;
    if (max){
        board[moves[0].first][moves[0].second]=1;
        if (depth==0){
            global_best_score=get_board_score(board,true)-get_board_score(baord,false);
        }
        else{
            global_best_score=PVS(board,false,depth-1,alpha,beta);
        }
        if (global_best_score>beta){
            board[moves[0].first][moves[0].second]=-1;
            return beta;
        }
        if (global_best_score>alpha){
            alpha=global_best_score;
        }
        board[moves[0].first][moves[0].second]=-1;
        #pragma omp parallel
        {
            int local_best_score=INT_MIN;
            int local_alpha=alpha;
            int local_beta=beta;
            vector<vector<int> > local_board=board;
            #pragma omp for schedule(static)
            for (int i=1;i<moves.size();i++){
                board[moves[i].first][moves[i].second]=1;
                int score=0;
                if (depth==0){
                    score=get_board_score(board,true)-get_board_score(baord,false);
                }
                else{
                    score=local_minimax(board,false,depth-1,alpha,beta);
                }
                if (score>local_best_score){
                    local_best_score=score;
                    local_alpha=local_best_score;
                }
                board[moves[i].first][moves[i].second]=-1;
            }
            #pragma omp critical
            {
                global_best_score=max(local_best_score,global_best_score);
            }
        }
    }
    else{
        board[moves[0].first][moves[0].second]=0;
        if (depth==0){
            global_best_score=get_board_score(board,true)-get_board_score(baord,false);
        }
        else{
            global_best_score=PVS(board,true,depth-1,alpha,beta);
        }
        if (global_best_score>beta){
            board[moves[0].first][moves[0].second]=-1;
            return beta;
        }
        if (global_best_score>alpha){
            alpha=global_best_score;
        }
        board[moves[0].first][moves[0].second]=-1;
        #pragma omp parallel
        {
            int local_best_score=INT_MAX;
            int local_alpha=alpha;
            int local_beta=beta;
            vector<vector<int> > local_board=board;
            #pragma omp for schedule(static)
            for (int i=1;i<moves.size();i++){
                board[moves[i].first][moves[i].second]=0;
                int score=0;
                if (depth==0){
                    score=get_board_score(board,true)-get_board_score(baord,false);
                }
                else{
                    score=local_minimax(board,true,depth-1,alpha,beta);
                }
                if (score<local_best_score){
                    local_best_score=score;
                    local_beta=local_best_score;
                }
                board[moves[i].first][moves[i].second]=-1;
            }
            #pragma omp critical
            {
                global_best_score=min(local_best_score,global_best_score);
            }
        }
    }
    return global_best_score;
}
pair<int,int> search_next_move(vector<vector<int> >& board){
    vector<pair<int,int>> moves;
    get_moves(board);
    pair<int,int> global_best_move=moves[0];
    board[moves[0].first][moves[0].second]=1;
    int alpha=INT_MIN;
    int beta=INT_MAX;
    int global_best_score=PVS(board,false,30,INT_MIN,INT_MAX);
    alpha=global_best_score;
    board[moves[0].first][moves[0].second]=-1;
    #pragma omp parallel
    {
        int local_best_score=INT_MIN;
        int local_best_move=moves[1];
        int local_alpha=alpha;
        int local_beta=beta;
        vector<vector<int> > local_board=board;
        #pragma omp for schedule(static)
        for (int i=1;i<moves.size();i++){
            board[moves[i].first][moves[i].second]=1;
            int score=local_minimax(board,false,30,local_alpha,local_beta);
            if (score>local_best_score){
                local_best_score=score;
                local_best_move=moves[i];
                local_alpha=local_best_score;
            }
            board[moves[i].first][moves[i].second]=-1;
        }
        #pragma omp critical
        {
            if (local_best_score>global_best_score){
                global_best_score=local_best_score;
                global_best_move=local_best_move;
            }
        }
    }
    return global_best_move;
}
int main(){
    vector<vector<int> > board(19,vector<int>(19,-1));
    board[6][10]=board[7][11]=board[8][9]=board[9][6]=board[8][10]=board[10][7]=board[10][8]=board[10][10]=board[11][5]=0;
    board[7][9]=board[8][8]=board[8][10]=board[9][7]=board[9][8]=board[9][9]=board[9][11]=board[10][6]=board[11][7]=1;
    pair<int,int> move=search_next_move(board);
    cout<<move.first<<' '<<move.second<<endl;
    return 0;
}