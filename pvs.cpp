#include "gomoku_utils.h"
#include <omp.h>
using namespace std;

int local_minimax(vector<vector<int> >& board, bool maxplayer, int depth, int alpha, int beta,bool debug){
    // if (debug){
    //     cout<<"info: "<<depth<<' '<<alpha<<' '<<beta<<endl;
    //     print_board(board);
    // }
    vector<pair<int,int>> moves;
    get_moves(board,moves);
    // sort_moves(board,moves,maxplayer);
    if (maxplayer){
        int best_score=INT_MIN;
        int score=0;
        for (int i=0;i<moves.size();i++){
            
            board[moves[i].first][moves[i].second]=1;
            if (depth==0){
                score=get_board_score_for_both(board);
            }
            else{
                // if (depth==3 && moves[i].first==8 && moves[i].second==10&& debug){
                //     score=local_minimax(board,false,depth-1,alpha,beta,true);
                // }
                // else{
                    score=local_minimax(board,false,depth-1,alpha,beta,false);
                // }
                // if (depth==3 && debug){
                //     cout<<"depth: "<<depth<<' '<<moves[i].first<<' '<<moves[i].second<<' '<<score<<endl;
                // }
            }
            best_score=max(score,best_score);
            if (score>alpha){
                alpha=score;
                if (alpha>=beta){
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
                score=get_board_score_for_both(board);
            }
            else{
                score=local_minimax(board,true,depth-1,alpha,beta,false);
            }
            // if (depth==2 && debug){
            //         cout<<"depth: "<<depth<<' '<<moves[i].first<<' '<<moves[i].second<<' '<<score<<endl;
            //     }
            best_score=min(score,best_score);
            if (score<beta){
                beta=score;
                if (alpha>=beta){
                    board[moves[i].first][moves[i].second]=-1;
                    break;
                }
            }
            board[moves[i].first][moves[i].second]=-1;
        }
        return best_score;
    }
}
int PVS(vector<vector<int> >& board, bool maxplayer, int depth, int alpha, int beta){
    vector<pair<int,int>> moves;
    get_moves(board,moves);
    sort_moves(board,moves,maxplayer);
    int global_best_score;
    if (maxplayer){
        board[moves[0].first][moves[0].second]=1;
        if (depth==0){
            global_best_score=get_board_score_for_both(board);
        }
        else{
            global_best_score=PVS(board,false,depth-1,alpha,beta);
        }
        if (global_best_score>beta){
            board[moves[0].first][moves[0].second]=-1;
            // return beta;
            return global_best_score;
        }
        if (global_best_score>alpha){
            alpha=global_best_score;
        }
        board[moves[0].first][moves[0].second]=-1;
        #pragma omp parallel
        {
            int local_best_score=global_best_score;
            int local_alpha=alpha;
            int local_beta=beta;
            vector<vector<int> > local_board=board;
            #pragma omp for schedule(static)
            for (int i=1;i<moves.size();i++){
                local_board[moves[i].first][moves[i].second]=1;
                int score=0;
                if (depth==0){
                    score=get_board_score_for_both(local_board);
                }
                else{
                    score=local_minimax(local_board,false,depth-1,local_alpha,local_beta,false);
                }
                if (score>local_best_score){
                    local_best_score=score;
                    // local_alpha=local_best_score;
                }
                if (score>local_alpha){
                    local_alpha=score;
                }
                local_board[moves[i].first][moves[i].second]=-1;
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
            global_best_score=get_board_score_for_both(board);
        }
        else{
            global_best_score=PVS(board,true,depth-1,alpha,beta);
        }
        // if (global_best_score>beta){
        //     board[moves[0].first][moves[0].second]=-1;
        //     return beta;
        // }
        // if (global_best_score>alpha){
        //     alpha=global_best_score;
        // }
        if (global_best_score<=alpha){
            board[moves[0].first][moves[0].second]=-1;
            return global_best_score;
        }
        if (global_best_score<beta){
            beta=global_best_score;
        }
        board[moves[0].first][moves[0].second]=-1;
        #pragma omp parallel
        {
            int local_best_score=global_best_score;
            int local_alpha=alpha;
            int local_beta=beta;
            vector<vector<int> > local_board=board;
            #pragma omp for schedule(static)
            for (int i=1;i<moves.size();i++){
                local_board[moves[i].first][moves[i].second]=0;
                int score=0;
                if (depth==0){
                    score=get_board_score_for_both(local_board);
                }
                else{
                    // if (moves[i].first==7 && moves[i].second==9){
                    //     score=local_minimax(local_board,true,depth-1,local_alpha,local_beta,true);

                    // }
                    // else{
                        score=local_minimax(local_board,true,depth-1,local_alpha,local_beta,false);

                    // }
                }
                // if (depth==4){
                //     cout<<moves[i].first<<' '<<moves[i].second<<' '<<score<<endl;
                // }
                if (score<local_best_score){
                    local_best_score=score;
                    local_beta=local_best_score;
                }
                // if (score<local_beta){
                //     local_beta=score;
                // }
                local_board[moves[i].first][moves[i].second]=-1;
            }
            #pragma omp critical
            {
                global_best_score=min(local_best_score,global_best_score);
            }
        }
    }
    return global_best_score;
}
pair<int,int> search_next_move(vector<vector<int> >& board, int depth){
    vector<pair<int,int>> moves;
    get_moves(board,moves);
    sort_moves(board,moves,true);
    pair<int,int> global_best_move=moves[0];
    cout<<global_best_move.first<<' '<<global_best_move.second<<endl;
    board[moves[0].first][moves[0].second]=1;
    int alpha=INT_MIN;
    int beta=INT_MAX;
    int global_best_score=PVS(board,false,depth-1,INT_MIN,INT_MAX);
    alpha=global_best_score;
    cout<<alpha<<endl;
    board[moves[0].first][moves[0].second]=-1;
    #pragma omp parallel
    {
        int local_best_score=INT_MIN;
        pair<int,int> local_best_move=moves[1];
        int local_alpha=alpha;
        int local_beta=beta;
        vector<vector<int> > local_board=board;
        #pragma omp for schedule(static)
        for (int i=1;i<moves.size();i++){
            local_board[moves[i].first][moves[i].second]=1;
            int score=local_minimax(local_board,false,depth-1,local_alpha,local_beta,false);
            cout<<moves[i].first<<' '<<moves[i].second<<' '<<score<<endl;
            if (score>local_best_score){
                local_best_score=score;
                local_best_move=moves[i];
                local_alpha=local_best_score;
            }
            local_board[moves[i].first][moves[i].second]=-1;
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
    // board[6][10]=board[7][11]=board[8][9]=board[9][6]=board[9][10]=board[10][7]=board[10][8]=board[10][10]=board[11][5]=0;
    // board[7][9]=board[8][8]=board[8][10]=board[9][7]=board[9][8]=board[9][9]=board[9][11]=board[10][6]=board[11][7]=1;
    board[8][8]=board[8][9]=board[9][9]=board[10][8]=board[10][9]=board[10][10]=board[11][8]=board[12][7]=board[12][8]=board[12][9]=1;
    board[9][7]=board[9][8]=board[9][10]=board[10][7]=board[10][11]=board[11][7]=board[11][9]=board[11][11]=board[12][6]=board[13][8]=0;
    double start=omp_get_wtime();
    pair<int,int> move=search_next_move(board,4);
    double end=omp_get_wtime();
    cout<<move.first<<' '<<move.second<<endl;
    board[move.first][move.second]=1;
    print_board(board);
    cout<<end-start<<endl;
    return 0;
}