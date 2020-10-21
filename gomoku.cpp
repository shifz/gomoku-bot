#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
using namespace std;

int BOARD_WIDTH=19;
void print_board(const vector<uint32_t>& board, const vector<uint32_t>& occupied){
    for (int i=0;i<BOARD_WIDTH;i++){
        uint32_t cur_row=board[i];
        uint32_t cur_row_occupancy=occupied[i];
        for (int j=0;j<BOARD_WIDTH;j++){
            if (cur_row_occupancy%2==0){
                cout<<' ';
            }
            else{
                if (cur_row%2==1){
                    cout<<'x';
                }
                else{
                    cout<<'o';
                }
            }
            cur_row>>=1;
            cur_row_occupancy>>=1;
        }
        cout<<endl;
    }
}
int main(){
    // vector<vector<int> > board(19,vector<int>(19,-1));
    vector<uint32_t> board(BOARD_WIDTH,0);
    vector<uint32_t> occupied(BOARD_WIDTH,0);
    board[BOARD_WIDTH/2+1]+=(1<<(BOARD_WIDTH/2+1));
    occupied[BOARD_WIDTH/2+1]+=(1<<(BOARD_WIDTH/2+1));
    while(true){
        string cmd;
        cout<<"> ";
        getline(cin,cmd);
        if (cmd=="print"){
            print_board(board,occupied);
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
            if ((occupied[x]<<y)%2==1){
                cout<<"The spot is taken"<<endl;
            }
            else{
                occupied[x]+=(1<<y);

            }
        }
    }
    return 0;
}