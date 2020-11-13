#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
using namespace std;

int BOARD_WIDTH=19;
int get_count_index(int count, int block, bool broken){
    cout<<count<<','<<block<<','<<(broken? "true":"false")<<endl;
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
int evaluate_horizontally(const vector<uint32_t>& board, const vector<uint32_t>& occupied, bool one){
    int counts[9]={0};
    int count=0;
    for (int i=0;i<BOARD_WIDTH;i++){
        uint32_t cur_row=board[i];
        uint32_t cur_row_occupancy=occupied[i];
        // used for broken threes and broken fours. for example OXX_XX will have 
        // prev_count = 2 and prev_block = 1 when we reached the space in the middle.
        int prev_count=0; 
        int prev_blocks=-1;
        int blocks=2;
        int index = -1;
        for (int j=0;j<BOARD_WIDTH;j++){
            if (i==0) cout<<"column: "<<j<<endl;
            if (cur_row_occupancy%2 != 0 && (cur_row%2 == (one? 1:0))){
                count++;
            }
            else if (cur_row_occupancy%2 == 0){
                if (prev_count > 0 && count==0){
                    index = get_count_index(prev_count, prev_blocks, false);
                    prev_count = 0;
                    prev_blocks = -1;
                }
                else if (count > 0){
                    blocks--;
                    if (prev_count > 0){
                        assert(blocks + prev_blocks<=1);
                        index = get_count_index(count + prev_count, blocks + prev_blocks, true);
                        prev_count=0;
                        prev_blocks=-1;
                    }
                    else{
                        prev_count = count;
                        prev_blocks = blocks;
                    }
                    count = 0;
                }
                blocks = 1;
            }
            else if (count > 0){
                if (prev_count > 0){
                    assert(blocks + prev_blocks<=2 && blocks + prev_blocks>=1);
                    index = get_count_index(count + prev_count, blocks + prev_blocks, true);
                }
                else{
                    index = get_count_index(count, blocks, false);
                }
                prev_count = 0;
                prev_blocks = -1;
                count = 0;
                blocks = 2;
            }
            else{
                blocks = 2;
                if (prev_count>0)
                    index = get_count_index(prev_count, prev_blocks, false);
                prev_count = 0;
                prev_blocks = -1;
            }
            if (index >= 0){
                counts[index]++;
                cout<< index<< ","<<i<<','<<j<<endl;
                index = -1;
            }
            cur_row>>=1;
            cur_row_occupancy>>=1;
        }
        if (count > 0){
            if (prev_count > 0){
                assert(blocks + prev_blocks<=2 && blocks + prev_blocks>=1);
                index = get_count_index(count + prev_count, blocks + prev_blocks, true);
            }
            else{
                index = get_count_index(count, blocks, false);
            }
            if (index >= 0){
                counts[index]++;
            }
        }
    }
    for (int i=0;i<9;i++){
        cout<<counts[i]<<',';
    }
    cout<<endl;
    return 0;
}

int evaluate_vertically(const vector<uint32_t>& board, const vector<uint32_t>& occupied, bool one){
    return 0;
}

int evaluate_diagnally(const vector<uint32_t>& board, const vector<uint32_t>& occupied, bool one){
    return 0;
}
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
    board[0]=0b0000000000000000000;
    occupied[0]=0b1101010101010101010;
    cout<<evaluate_horizontally(board,occupied,false);
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