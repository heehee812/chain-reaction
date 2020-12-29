#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
struct Pos{
    int x, y;
};

class Enemy{
    private:
        Pos *table= NULL;
        Pos **EnemyBurst;
    public:
        int size= 1;
        bool check_enemy_burst();
        void add_enemy();
        //table
        void push(int i, int j);
        void print_table();
};

//board
bool check_board_empty(Board board);

//self
Pos *AroundPoint;
Pos *AddPoint;

bool check_point_around();
bool check_point_add();
void attack(); //turn the bursted one to mine
void point_burst(); //find the most number to burst
void priority_add();

void algorithm_A(Board board, Player player, int index[]){

    int row, col;
    int color = player.get_color();

    Enemy enemy;
    enemy.push(1, 1);
    enemy.print_table();
    // if(check_board_empty(board))
    //     cout<<"yep, is empty!"<<endl;
    // else
    //     cout<<"no, is not empty! "<<endl;
    
    
    // while(1){
    //     row = rand() % 5;
    //     col = rand() % 6;
    //     if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    // }

    // index[0] = row;
    // index[1] = col;
    
}

bool check_board_empty(Board board){
    for(int i= 0; i<ROW; i++)
        for(int j= 0; j<COL; j++){
            if(board.get_orbs_num(i, j)!= 0)
                return false;
        }
    return true;
}

//Enemy
void Enemy::push(int i, int j){
    if(table!= NULL){
        Pos *tmp= table;
        delete [] table;
        Pos *table;
        table= new Pos[size++];
        for(int i= 0; i<size-1; i++)
            table[i]= tmp[i];
    }
    else
        table= new Pos[size];
    
    table[size-1].x= i;
    table[size-1].y =j;
}

void Enemy::print_table(){
    if(table!=NULL){
        for(int i= 0; i<size; i++)
            cout<<table[i].x<<table[i].y<<" ";
        cout<<endl;
    }
    else
        cout<<"empty..."<<endl;
    
}