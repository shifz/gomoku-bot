//
// Created by Jiaxin Ye on 11/15/20.
//

#ifndef GOMOKU_BOT_GOMOKU_UTILS_H
#define GOMOKU_BOT_GOMOKU_UTILS_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>
using namespace std;

extern int BOARD_WIDTH;
extern int W[11];

int get_count_index(int count, int block, bool broken);
int check_broken(const vector<vector<int> >& board, int count, int block, pair<int,int> pos, pair<int,int> dir, bool one);
int evaluate_horizontally(const vector<vector<int> >& board, bool one);
int evaluate_vertically(const vector<vector<int> >& board, bool one);
int evaluate_diagnally(const vector<vector<int> >& board, bool one);
int get_board_score(vector<vector<int> >& board, bool one);
bool moveComp(const pair<pair<int, int>, int>& move_1, const pair<pair<int, int>, int>& move_2);
void sort_moves(vector<vector<int> >& board, vector<pair<int,int> >& moves, bool one);
void print_board(const vector<vector<int> >& board);
void get_moves(const vector<vector<int> >& board, vector<vector<pair<int,int> > >& moves);

#endif //GOMOKU_BOT_GOMOKU_UTILS_H
