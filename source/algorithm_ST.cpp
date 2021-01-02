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
        Pos *EnemyBurst= NULL;
        int EnemySize= 1;
        Pos *table= NULL;
        int TableSize= 1;
        char color;
    public:
        Enemy(Player player){
            if(player.get_color()== RED)
                color= BLUE;
            else
                color= RED;
            
        }
        bool check_enemy_burst(Board);
        //table
        void push_table(int, int);
        void print_table();
        //burst
        void push_enemy_burst(int, int);
        void print_enemy_burst();
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
    Enemy enemy(player);
    
    if(check_board_empty(board))
        cout<<"yep, is empty!"<<endl;
    else{
        enemy.check_enemy_burst(board);
    }
    
    
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
void Enemy::push_table(int i, int j){
    if(table!= NULL){
        Pos *tmp= table;
        table= new Pos[++TableSize];
        for(int x= 0; x<TableSize-1; x++){
            table[x]= tmp[x];
        }
    }
    else
        table= new Pos[TableSize];
    table[TableSize-1].x= i;
    table[TableSize-1].y =j;
}

void Enemy::print_table(){
    if(table!=NULL){
        for(int i= 0; i<TableSize; i++){
            cout<<table[i].x<<table[i].y<<" ";;
        }
        cout<<endl;
    }
    else
        cout<<"table empty..."<<endl;
    
}

bool Enemy::check_enemy_burst(Board board){
    bool burst= false;
    EnemyBurst= NULL;
    for(int i= 0; i< ROW; i++){
        for(int j= 0; j<COL; j++){
            if(board.get_cell_color(i, j)== color&& (board.get_orbs_num(i, j)+ 1== board.get_capacity(i, j))){
                push_enemy_burst(i, j);
                burst= true;
            }
        }
    }
    print_enemy_burst();
    return burst;
}

void Enemy::push_enemy_burst(int i, int j){
    if(EnemyBurst!= NULL){
        Pos *tmp= EnemyBurst;
        EnemyBurst= new Pos[++EnemySize];
        for(int x= 0; x<EnemySize-1; x++){
            EnemyBurst[x]= tmp[x];
        }
    }
    else
        EnemyBurst= new Pos[EnemySize];
    EnemyBurst[EnemySize-1].x= i;
    EnemyBurst[EnemySize-1].y =j;
}
void Enemy::print_enemy_burst(){
    if(EnemyBurst!=NULL){
        for(int i= 0; i<EnemySize; i++){
            cout<<EnemyBurst[i].x<<EnemyBurst[i].y<<" ";;
        }
        cout<<endl;
    }
    else
        cout<<"enemy burst empty..."<<endl;
}