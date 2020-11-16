#include "gomoku_utils.h"
#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

int minimax(vector<vector<int> >& board, int depth, bool maxplayer, int alpha, int beta, bool debug){
    if (depth == 0){
        int temp=get_board_score_for_both(board);
        if (debug&&temp<0) cout<<temp<<endl;
        return temp;
    }
    if (maxplayer){
        int value=INT_MIN;
        vector<pair<int,int> > moves;
        get_moves(board,moves);
        for (int i=0;i<moves.size();i++){
            board[moves[i].first][moves[i].second]=1;
            value=max(minimax(board,depth-1,false,alpha,beta,debug),value);
            board[moves[i].first][moves[i].second]=-1;
            alpha = max(alpha,value);
            if (alpha>=beta){
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
            value=min(minimax(board,depth-1,true,alpha,beta,debug),value);
            board[moves[i].first][moves[i].second]=-1;
            beta = min(beta,value);
            if (alpha>=beta){
                break;
            }
        }
        return value;
    }
}
pair<int,int> search_next_move(vector<vector<int> >& board){
    vector<pair<int,int> > moves;
    get_moves(board,moves);
    int max_value=INT_MIN;
    pair<int,int> best_move;
    for (int i=0;i<moves.size();i++){
        board[moves[i].first][moves[i].second]=1;
        int value;
        value=minimax(board,3,false,INT_MIN,INT_MAX,false);
        if (value>max_value){
            max_value=value;
            best_move=moves[i];
        }
        board[moves[i].first][moves[i].second]=-1;
    }
    return best_move;
}
int main(){
    vector<vector<int> > board(19,vector<int>(19,-1));
    // vector<int> test_column = {-1,-1,-1,-1,-1,0,1,1,-1,1,1,-1,-1,-1,-1,-1,-1,-1,-1};
    // for (size_t i = 0; i < BOARD_WIDTH; i++){
    //     board[0][i] = test_column[i];
    // }
    // cout << evaluate_horizontally(board,true)<<endl;
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
    board[6][10]=board[7][11]=board[8][9]=board[9][6]=board[9][10]=board[10][7]=board[10][8]=board[10][10]=board[11][5]=0;
    board[7][9]=board[8][8]=board[8][10]=board[9][7]=board[9][8]=board[9][9]=board[9][11]=board[10][6]=board[11][7]=1;
    cout<<get_board_score(board,true);
    print_board(board);
    auto start = high_resolution_clock::now(); 
    pair<int,int> p=search_next_move(board);
    auto stop = high_resolution_clock::now(); 
    cout<<p.first<<' '<<p.second<<endl;
    cout<<(duration_cast<milliseconds>(stop - start)).count()<<endl;
    // board[9][9]=1;
    // while(true){
    //     string cmd;
    //     cout<<"> ";
    //     getline(cin,cmd);
    //     if (cmd=="print"){
    //         print_board(board);
    //     }
    //     else if (cmd=="end"){
    //         break;
    //     }
    //     else{
    //         stringstream ss;
    //         ss.str(cmd);
    //         int x;
    //         int y;
    //         ss>>x>>y;
    //         if (board[x][y]!=-1){
    //             cout<<"The spot is taken"<<endl;
    //         }
    //         else{
    //             board[x][y]=0;
    //             print_board(board);
    //             pair<int,int> p=search_next_move(board);
    //             cout<<p.first<<' '<<p.second<<endl;
    //             board[p.first][p.second]=1;
    //         }
    //     }
    // }
    return 0;
}