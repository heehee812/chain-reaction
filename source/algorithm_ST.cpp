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

bool check_point_add();
bool protect(); //turn the bursted one to mine
bool attack(); //find the harmful to burst
void priority_add();
bool check_point_around(int, Board);
bool check_around(int, int, Board, int);

void algorithm_A(Board board, Player player, int index[]){

    int row, col;
    int color = player.get_color();
    Enemy enemy(player);
    
    if(!check_board_empty(board)){
        if(enemy.check_enemy_burst(board)){
            cout<<"enemy burst"<<endl;
            if(protect())
                cout<<"protect, return the point"<<endl;
            else{
                cout<<"connot protect"<<endl;
                if(attack())
                    cout<<"attack, return the point"<<endl;
                else
                    cout<<"cannot attack"<<endl;
            }
        }
        //if no enemy burst or cannot attack and protect
        else
            cout<<"No, no one burst..."<<endl;
        if(check_point_around(color, board)){
            cout<<"point around, return the point"<<endl;
        }
        else
            cout<<"no one need around"<<endl;
    }
    else
        cout<<"board is empty!"<<endl;
    priority_add();
       
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

bool protect(){
    bool pro= true;
    return !pro;
}

bool attack(){
    bool atta= true;
    return !atta;
}

bool check_point_around(int color, Board board){
    bool around= false;
    for(int i= 0; i<ROW; i++)
        for(int j= 0; j< COL; j++){
            if(board.get_cell_color(i, j)== color&& (board.get_orbs_num(i, j)+ 1== board.get_capacity(i, j))){
                around= check_around(i, j, board, color)&around;
            }
        }
    return around;
}

bool check_around(int i, int j, Board board, int color){
    bool around= false;
    if(i>0){
        if(board.get_cell_color(i- 1, j)== 'w'){
            cout<<"store"<<i- 1<<j<<"to the AddPoint queue"<<endl;
            around= true;
        }
    }
    //down
    if(i<ROW){
        if(board.get_cell_color(i+ 1, j)== 'w'){
            cout<<"store"<<i+ 1<<j<<"to the AddPoint queue"<<endl;
            around= true;
        }
    }
    //left
    if(j>0){
        if(board.get_cell_color(i, j- 1)== 'w'){
            cout<<"store"<<i<<j- 1<<"to the AddPoint queue"<<endl;
            around= true;
        }
    }
    //right
    if(j<COL){
        if(board.get_cell_color(i, j+ 1)== 'w'){
            cout<<"store"<<i<<j+ 1<<"to the AddPoint queue"<<endl;
            around= true;
        }
    }
    //up-left
    if(j>0&&i>0){
        if(board.get_cell_color(i- 1, j- 1)== 'w')
            cout<<"store"<<i- 1<<j- 1<<"to the AddPoint queue"<<endl;
            around= true;
    }
    //up-right
    if(j<COL&&i>0){
        if(board.get_cell_color(i- 1, j+ 1)== 'w'){
            cout<<"store"<<i- 1<<j+ 1<<"to the AddPoint queue"<<endl;
            around= true;
        }
    }
    //down-left
    if(j>0&& i<ROW){
        if(board.get_cell_color(i+ 1, j- 1)== 'w'){
            cout<<"store"<<i+ 1<<j- 1<<"to the AddPoint queue"<<endl;
            around= true;
        }
    }
    //down-right
    if(j<COL&& i<ROW){
        if(board.get_cell_color(i+ 1, j+ 1)== 'w'){
            cout<<"store"<<i+ 1<<j+ 1<<"to the AddPoint queue"<<endl;
            around= true;
        }
    }
    return around;
}

void priority_add(){
    cout<<"priority add the point"<<endl;
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