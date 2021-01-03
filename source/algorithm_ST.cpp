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
    int val= 0;
};

class Enemy{
    private:
        Pos *EnemyBurst= NULL;
        int EnemySize= 1;
        Pos *table= NULL;
        int TableSize= 1;
        int color= BLUE;
    public:
        bool check_enemy_burst(Board);
        //table
        void push_table(int, int);
        void print_table();
        //burst
        void push_enemy_burst(int, int);
        void print_enemy_burst();
        Pos enemy_burst_top();
        bool enemy_burst_pop();
        void sort_enemy_burst(Board);
};

//board
bool check_board_empty(Board);
bool check_corner(Board, int[]);
bool check_edge(Board);
bool check_center(Board);
bool check_point_center(int, int, Board);

//self
Pos **AroundPoint= NULL;
Pos *AddPoint= NULL;
int AddPointSize= 1;
Pos *Burst= NULL;
int BurstSize= 1;
int AroundPointSize= 1;

bool check_point_add();
bool protect(Enemy, Board, Player, int[]); //turn the bursted one to mine
bool attack(Board, Player, int[]); //find the harmful to burst
void priority_add(Board board, int[]);
bool check_point_around(int, Board);
Pos choose_around();
bool check_around(int, int, Board, int);
bool check_burst(Board);
void push_burst(int, int);
void print_burst();
int count_board(Board, int);
void push_center_point(int, int);
void push_around_point(int, int);
void print_around_point();
void push_add_point(int, int);
void print_add_point();

void algorithm_A(Board board, Player player, int index[]){

    int row, col;
    int color = player.get_color();
    Enemy enemy;
    
    // if(!check_board_empty(board)){
    //     if(enemy.check_enemy_burst(board)){
    //         enemy.sort_enemy_burst(board);
    //         cout<<"enemy burst."<<endl;
    //         if(protect(enemy, board, player, index)){
    //             cout<<"protect, return the point"<<index[0]<<index[1]<<endl;
    //             return;
    //         }
    //         else{
    //             cout<<"connot protect"<<endl;
    //             if(attack(board, player, index)){
    //                 cout<<"attack, return the point"<<index[0]<<index[1]<<endl;
    //                 return;
    //             }
    //         }
    //     }
    //     //if no enemy burst or cannot attack and protect
    //     if(check_point_around(color, board)){
    //         Pos choosen= choose_around();
    //         index[0]= choosen.x;
    //         index[1]= choosen.y;
    //         cout<<"point around, return the point"<<endl;
    //         return;
    //     }
    // }
    priority_add(board, index);
}

bool check_board_empty(Board board){
    for(int i= 0; i<ROW; i++)
        for(int j= 0; j<COL; j++){
            if(board.get_orbs_num(i, j)!= 0)
                return false;
        }
    return true;
}

bool protect(Enemy enemy, Board board, Player redPlayer, int index[]){
    bool pro= false;
    if(check_burst(board)){
        Player bluePlayer(BLUE);
        Pos tmp= enemy.enemy_burst_top();
        int count= 0;
        while(enemy.enemy_burst_pop()){
            for(int i= 0; i<BurstSize; i++){
                Board boardCopy= board;
                boardCopy.place_orb(Burst[i].x, Burst[i].y, &redPlayer);
                if(boardCopy.get_cell_color(tmp.x, tmp.y)!= BLUE){
                    int newCount= count_board(boardCopy, RED);
                    if(count<newCount){
                        index[0]= Burst[i].x;
                        index[1]= Burst[i].y;
                        count= newCount;
                    }
                }
            }
            if(count!= 0){
                pro= true;
                break;
            }
            else
                tmp= enemy.enemy_burst_top();
        }
    }
    return pro;
}

int count_board(Board board, int color){
    int count= 0;
    for(int i= 0; i<ROW; i++)
        for(int j= 0; j<COL; j++)
            if(board.get_cell_color(i, j)== color)
                count++;
    return count;
}

bool attack(Board board, Player redPlayer, int index[]){
    bool atta= false;
    int count= count_board(board, BLUE);
    if(check_burst(board)){
        for(int i= 0; i<BurstSize; i++){
            Board boardCopy= board;
            boardCopy.place_orb(Burst[i].x, Burst[i].y, &redPlayer);
            int newCount= count_board(boardCopy, BLUE);
            if(count>newCount){
                count= newCount;
                atta= true;
                index[0]= Burst[i].x;
                index[1]= Burst[i].y;
            }
        }
    }
    return atta;
}

bool check_point_around(int color, Board board){
    bool around= false;
    for(int i= 0; i<ROW; i++)
        for(int j= 0; j< COL; j++){
            if(board.get_cell_color(i, j)== color&& (board.get_orbs_num(i, j)+ 1== board.get_capacity(i, j))){
                push_center_point(i, j);
                around= check_around(i, j, board, color)|around;
            }
        }
    return around;
}

bool check_around(int i, int j, Board board, int color){
    bool around= false;
    if(i>0){
        if(board.get_cell_color(i- 1, j)== 'w'){
            push_around_point(i- 1, j);
            around= true;
        }
    }
    //down
    if(i<ROW- 1){
        if(board.get_cell_color(i+ 1, j)== 'w'){
            push_around_point(i+ 1, j);
            around= true;
        }
    }
    //left
    if(j>0){
        if(board.get_cell_color(i, j- 1)== 'w'){
            push_around_point(i, j- 1);
            around= true;
        }
    }
    //right
    if(j<COL- 1){
        if(board.get_cell_color(i, j+ 1)== 'w'){
            push_around_point(i, j+ 1);
            around= true;
        }
    }
    //up-left
    if(j>0&&i>0){
        if(board.get_cell_color(i- 1, j- 1)== 'w')
            push_around_point(i- 1, j- 1);
            around= true;
    }
    //up-right
    if(j<COL- 1&&i>0){
        if(board.get_cell_color(i- 1, j+ 1)== 'w'){
            push_around_point(i- 1, j+ 1);
            around= true;
        }
    }
    //down-left
    if(j>0&& i<ROW- 1){
        if(board.get_cell_color(i+ 1, j- 1)== 'w'){
            push_around_point(i+ 1, j- 1);
            around= true;
        }
    }
    //down-right
    if(j<COL- 1&& i<ROW- 1){
        if(board.get_cell_color(i+ 1, j+ 1)== 'w'){
            push_around_point(i+ 1, j+ 1);
            around= true;
        }
    }
    return around;
}

Pos choose_around(){
    int size= AroundPoint[0][0].val;
    Pos choosen= AroundPoint[0][0];
    for(int i= 0; i< AroundPointSize; i++){
        if(size>AroundPoint[i][0].val){
            size= AroundPoint[i][0].val;
            choosen= AroundPoint[i][0];
        }
    }
    return choosen;
}

void print_around_point(){
    for(int i= 0; i<AroundPointSize; i++){
        cout<<AroundPoint[i][0].x<<AroundPoint[i][0].y<<endl;
        for(int j= 1; j<AroundPoint[i][0].val; j++)
            cout<<AroundPoint[i][j].x<<AroundPoint[i][j].y<<" ";
        cout<<endl;
    }
}

void priority_add(Board board, int index[]){
    delete [] AddPoint;
    AddPoint= NULL;
    if(check_corner(board, index)){
        index[0]= AddPoint[AddPointSize- 1].x;
        index[1]= AddPoint[AddPointSize- 1].y;
        cout<<"corner, return the point"<<AddPoint[AddPointSize- 1].x<<AddPoint[AddPointSize- 1].y<<endl;
        return;
    }
    else if(check_edge(board)){
        index[0]= AddPoint[AddPointSize- 1].x;
        index[1]= AddPoint[AddPointSize- 1].y;
        cout<<"edge, return the point"<<AddPoint[AddPointSize- 1].x<<AddPoint[AddPointSize- 1].y<<endl;
        return;
    }
    else if(check_center(board))
        cout<<"center, return the point"<<endl;
    else{
        cout<<"random"<<endl;
        // random
        // while(1){
        //     row = rand() % 5;
        //     col = rand() % 6;
        //     if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
        // }

        // index[0] = row;
        // index[1] = col;
    }
    cout<<"priority add the point"<<endl;
}

void push_add_point(int i, int j){
    if(AddPoint!= NULL){
        Pos *tmp= AddPoint;
        AddPoint= new Pos[++AddPointSize];
        for(int x= 0; x<AddPointSize-1; x++){
            AddPoint[x]= tmp[x];
        }
        AddPoint[AddPointSize-1].x= i;
        AddPoint[AddPointSize-1].y =j;
    }
    else{
        AddPoint= new Pos[AddPointSize];
        AddPoint[0].x= i;
        AddPoint[0].y =j;
    }
}

void print_add_point(){
    for(int i= 0; i<AddPointSize; i++)
        cout<<AddPoint[i].x<<AddPoint[i].y<<" ";
    cout<<endl;
}

bool check_corner(Board board, int index[]){
    bool check= false;
    if(board.get_cell_color(0, 0)== RED|| board.get_cell_color(0, 0)== 'w'){
        if(board.get_orbs_num(0, 0)< board.get_capacity(0, 0)- 1){
            push_add_point(0, 0);
            check= true;
        }
    }
    if(board.get_cell_color(0, 5)== RED||board.get_cell_color(0, 5)== 'w'){
        if(board.get_orbs_num(0, 5)<board.get_capacity(0, 5)- 1){
            push_add_point(0, 5);
            check= true;
        }
    }
    if(board.get_cell_color(4, 0)== RED|| board.get_cell_color(4, 0)== 'w'){
        if(board.get_orbs_num(4, 0)<board.get_capacity(4, 0)- 1){
            push_add_point(4, 0);
            check= true;
        }
    }
    if(board.get_cell_color(4, 5)== RED|| board.get_cell_color(4, 5)== 'w'){
        if(board.get_orbs_num(4, 5)<board.get_capacity(4, 5)- 1){
            push_add_point(4, 5);
            check= true;
        }
    }
    if(AddPoint!= NULL)
        print_add_point();
    return check;
}

bool check_edge(Board board){
    bool check= false;
    for(int i= 0; i< COL; i++){
        if(board.get_cell_color(0, i)== RED|| board.get_cell_color(0, i)== 'w'){
            if(board.get_orbs_num(0, i)< board.get_capacity(0, i)- 1){
                push_add_point(0, i);
                check= true;
            }
        }
    }
    for(int i= 0; i< COL; i++){
        if(board.get_cell_color(ROW- 1, i)== RED||board.get_cell_color(ROW- 1, i)== 'w'){
            if(board.get_orbs_num(ROW- 1, i)< board.get_capacity(ROW- 1, i)- 1){
                push_add_point(ROW- 1, i);
                check= true;
            }
        }
    }
    for(int j= 0; j< ROW; j++){
        if(board.get_cell_color(j, 0)== RED|| board.get_cell_color(j, 0)== 'w'){
            if(board.get_orbs_num(j, 0)< board.get_capacity(j, 0)- 1){
                push_add_point(j, 0);
                check= true;
            }
        }
    }
    for(int j= 0; j< ROW; j++){
        if(board.get_cell_color(j, COL- 1)== RED|| board.get_cell_color(j, COL- 1)== 'w'){
            if(board.get_orbs_num(j, COL- 1)< board.get_capacity(j, COL- 1)- 1){
                push_add_point(j, COL- 1);
                check= true;
            }
        }
    }
    return check;
}

bool check_center(Board board){
    bool check= false;
    cout<<"check center"<<endl;
    for(int i= 0; i<ROW; i++)
        for(int j= 0; j<COL; j++){
            if(board.get_cell_color(i, j)== RED){
                check= check_point_center(i, j, board)|check;
            }
        }
    return check;
}

bool check_point_center(int i, int j, Board board){
    bool check= false;
    if(i> 0&& i<ROW- 1&&j> 0&& j< COL- 1){
        if(board.get_cell_color(i-1, j)== RED&& board.get_orbs_num(i-1, j)<board.get_capacity(i-1, j)- 1)//up
            if(board.get_cell_color(i+ 1, j)== RED&& board.get_orbs_num(i+1, j)<board.get_capacity(i+1, j)- 1)//down
                if(board.get_cell_color(i, j- 1)== RED&& board.get_orbs_num(i, j- 1)<board.get_capacity(i, j- 1)- 1)//left
                    if(board.get_cell_color(i, j+ 1)== RED &&board.get_orbs_num(i, j+ 1)<board.get_capacity(i, j+ 1)- 1)//right
                        if(board.get_cell_color(i- 1, j- 1)== RED&& board.get_orbs_num(i- 1, j- 1)<board.get_capacity(i- 1, j- 1)- 1)//up-left
                            if(board.get_cell_color(i- 1, j+ 1)== RED&& board.get_orbs_num(i- 1, j+ 1)<board.get_capacity(i- 1, j+ 1)- 1)//up-right
                                if(board.get_cell_color(i+ 1, j- 1)== RED&& board.get_orbs_num(i+1, j- 1)<board.get_capacity(i+1, j- 1)- 1)//down-left
                                    if(board.get_cell_color(i+ 1, j+ 1)== RED&& board.get_orbs_num(i+ 1, j+ 1)<board.get_capacity(i+ 1, j+ 1)- 1)//down-right
                                        check= true;
    }
    else if(i<ROW- 1&&j> 0&& j< COL- 1){
        if(board.get_cell_color(i+ 1, j)== RED&& board.get_orbs_num(i+1, j)<board.get_capacity(i+1, j)- 1)//down
            if(board.get_cell_color(i, j- 1)== RED&& board.get_orbs_num(i, j- 1)<board.get_capacity(i, j- 1)- 1)//left
                if(board.get_cell_color(i, j+ 1)== RED &&board.get_orbs_num(i, j+ 1)<board.get_capacity(i, j+ 1)- 1)//right
                    if(board.get_cell_color(i+ 1, j- 1)== RED&& board.get_orbs_num(i+1, j- 1)<board.get_capacity(i+1, j- 1)- 1)//down-left
                        if(board.get_cell_color(i+ 1, j+ 1)== RED&& board.get_orbs_num(i+ 1, j+ 1)<board.get_capacity(i+ 1, j+ 1)- 1)//down-right
                            check= true;
    }
    else if(i> 0&&j> 0&& j< COL- 1){
        if(board.get_cell_color(i-1, j)== RED&& board.get_orbs_num(i-1, j)<board.get_capacity(i-1, j)- 1)//up
            if(board.get_cell_color(i, j- 1)== RED&& board.get_orbs_num(i, j- 1)<board.get_capacity(i, j- 1)- 1)//left
                if(board.get_cell_color(i, j+ 1)== RED &&board.get_orbs_num(i, j+ 1)<board.get_capacity(i, j+ 1)- 1)//right
                    if(board.get_cell_color(i- 1, j- 1)== RED&& board.get_orbs_num(i- 1, j- 1)<board.get_capacity(i- 1, j- 1)- 1)//up-left
                        if(board.get_cell_color(i- 1, j+ 1)== RED&& board.get_orbs_num(i- 1, j+ 1)<board.get_capacity(i- 1, j+ 1)- 1)//up-right
                            check= true;
    }
    else if(i> 0&& i<ROW- 1&& j< COL- 1){
        if(board.get_cell_color(i-1, j)== RED&& board.get_orbs_num(i-1, j)<board.get_capacity(i-1, j)- 1)//up
            if(board.get_cell_color(i+ 1, j)== RED&& board.get_orbs_num(i+1, j)<board.get_capacity(i+1, j)- 1)//down
                if(board.get_cell_color(i, j+ 1)== RED &&board.get_orbs_num(i, j+ 1)<board.get_capacity(i, j+ 1)- 1)//right
                    if(board.get_cell_color(i- 1, j+ 1)== RED&& board.get_orbs_num(i- 1, j+ 1)<board.get_capacity(i- 1, j+ 1)- 1)//up-right
                        if(board.get_cell_color(i+ 1, j+ 1)== RED&& board.get_orbs_num(i+ 1, j+ 1)<board.get_capacity(i+ 1, j+ 1)- 1)//down-right
                            check= true;
    }
    else if(i> 0&& i<ROW- 1&&j> 0){
        if(board.get_cell_color(i-1, j)== RED&& board.get_orbs_num(i-1, j)<board.get_capacity(i-1, j)- 1)//up
            if(board.get_cell_color(i+ 1, j)== RED&& board.get_orbs_num(i+1, j)<board.get_capacity(i+1, j)- 1)//down
                if(board.get_cell_color(i, j- 1)== RED&& board.get_orbs_num(i, j- 1)<board.get_capacity(i, j- 1)- 1)//left
                    if(board.get_cell_color(i- 1, j- 1)== RED&& board.get_orbs_num(i- 1, j- 1)<board.get_capacity(i- 1, j- 1)- 1)//up-left
                        if(board.get_cell_color(i+ 1, j- 1)== RED&& board.get_orbs_num(i+1, j- 1)<board.get_capacity(i+1, j- 1)- 1)//down-left
                            check= true;
    }
    else if(i<ROW- 1&&j< COL- 1){
        if(board.get_cell_color(i+ 1, j)== RED&& board.get_orbs_num(i+1, j)<board.get_capacity(i+1, j)- 1)//down
            if(board.get_cell_color(i, j+ 1)== RED &&board.get_orbs_num(i, j+ 1)<board.get_capacity(i, j+ 1)- 1)//right
                if(board.get_cell_color(i+ 1, j+ 1)== RED&& board.get_orbs_num(i+ 1, j+ 1)<board.get_capacity(i+ 1, j+ 1)- 1)//down-right
                    check= true;
    }
    else if(i<ROW- 1&&j> 0){
        if(board.get_cell_color(i+ 1, j)== RED&& board.get_orbs_num(i+1, j)<board.get_capacity(i+1, j)- 1)//down
            if(board.get_cell_color(i, j- 1)== RED&& board.get_orbs_num(i, j- 1)<board.get_capacity(i, j- 1)- 1)//left
                if(board.get_cell_color(i+ 1, j- 1)== RED&& board.get_orbs_num(i+1, j- 1)<board.get_capacity(i+1, j- 1)- 1)//down-left
                    check= true;
    }
    else if(i> 0&& j< COL- 1){
        if(board.get_cell_color(i-1, j)== RED&& board.get_orbs_num(i-1, j)<board.get_capacity(i-1, j)- 1)//up
            if(board.get_cell_color(i, j+ 1)== RED &&board.get_orbs_num(i, j+ 1)<board.get_capacity(i, j+ 1)- 1)//right
                if(board.get_cell_color(i- 1, j+ 1)== RED&& board.get_orbs_num(i- 1, j+ 1)<board.get_capacity(i- 1, j+ 1)- 1)//up-right
                    check= true;
    }
    else if(i<ROW- 1&& j< COL- 1){
        if(board.get_cell_color(i-1, j)== RED&& board.get_orbs_num(i-1, j)<board.get_capacity(i-1, j)- 1)//up
            if(board.get_cell_color(i, j- 1)== RED&& board.get_orbs_num(i, j- 1)<board.get_capacity(i, j- 1)- 1)//left
                if(board.get_cell_color(i- 1, j- 1)== RED&& board.get_orbs_num(i- 1, j- 1)<board.get_capacity(i- 1, j- 1)- 1)//up-left
                    check= true;
    }
    if(check)
        cout<<"Add "<<i<<j<<"into AddPoint"<<endl;
    return check;
}

bool check_burst(Board board){
    bool burst= false;
    delete [] Burst;
    Burst= NULL;
    BurstSize= 1;
    for(int i= 0; i< ROW; i++){
        for(int j= 0; j<COL; j++){
            if(board.get_cell_color(i, j)== RED&& (board.get_orbs_num(i, j)+ 1== board.get_capacity(i, j))){
                push_burst(i, j);
                burst= true;
            }
        }
    }
    if(burst){
        cout<<"check burst."<<endl;
    }
    else
        cout<<"Cannot burst..."<<endl;
    return burst;
}

void push_burst(int i, int j){
    if(Burst!= NULL){
        Pos *tmp= Burst;
        Burst= new Pos[++BurstSize];
        for(int x= 0; x<BurstSize-1; x++){
            Burst[x]= tmp[x];
        }
        Burst[BurstSize-1].x= i;
        Burst[BurstSize-1].y =j;
    }
    else{
        Burst= new Pos[BurstSize];
        Burst[0].x= i;
        Burst[0].y =j;
    }
}

void print_burst(){
    if(Burst!=NULL){
        for(int i= 0; i<BurstSize; i++){
            cout<<Burst[i].x<<Burst[i].y<<" ";
        }
        cout<<endl;
    }
    else
        cout<<"burst empty..."<<endl;
}

void push_center_point(int i, int j){
    if(AroundPoint!= NULL){
        Pos **tmp= AroundPoint;
        AroundPoint= new Pos*[++AroundPointSize];
        for(int x= 0; x<AroundPointSize-1; x++){
            AroundPoint[x]= tmp[x];
        }
    }
    else{
        AroundPoint= new Pos*[AroundPointSize];
    }
    AroundPoint[AroundPointSize-1]= new Pos[1];
    AroundPoint[AroundPointSize-1][0].val= 1;
    AroundPoint[AroundPointSize-1][0].x= i;
    AroundPoint[AroundPointSize-1][0].y =j;
}

void push_around_point(int i, int j){
    Pos *tmp= AroundPoint[AroundPointSize- 1];
    int index= ++AroundPoint[AroundPointSize- 1][0].val;
    delete [] AroundPoint[AroundPointSize- 1];
    AroundPoint[AroundPointSize- 1]= new Pos[index];
    AroundPoint[AroundPointSize- 1][0].val= index;
    for(int x= 0; x<index-1; x++)
        AroundPoint[AroundPointSize- 1][x]= tmp[x];
    AroundPoint[AroundPointSize- 1][index- 1].x= i;
    AroundPoint[AroundPointSize- 1][index- 1].y =j;
}

bool check_point_add(){
    bool add= false;
    return add;
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
    delete [] EnemyBurst;
    EnemyBurst= NULL;
    EnemySize= 1;
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
            cout<<EnemyBurst[i].x<<EnemyBurst[i].y<<" ";
        }
        cout<<endl;
    }
    else
        cout<<"enemy burst empty..."<<endl;
}

void Enemy::sort_enemy_burst(Board board){
    Board boardCopy= board;
    Player enemy(BLUE);
    for(int i= 0; i<EnemySize; i++){
        int val= 0;
        boardCopy.place_orb(EnemyBurst[i].x, EnemyBurst[i].y, &enemy);
        for(int r= 0; r< ROW; r++)
            for(int c= 0; c<COL; c++)
                if(boardCopy.get_cell_color(r, c)== color)
                    val++;
        EnemyBurst[i].val= val;
    }
    
}

Pos Enemy::enemy_burst_top(){
    return this->EnemyBurst[EnemySize-1];
}

bool Enemy::enemy_burst_pop(){
    if(EnemySize- 1>= 0){
        EnemySize--;
        Pos *tmp;
        tmp= EnemyBurst;
        EnemyBurst= new Pos[EnemySize];
        if(EnemySize!= 0){
            for(int i= 0; i< EnemySize; i++)
                EnemyBurst[i]= tmp[i];
        }
        return true;
    }
    else{
        delete [] EnemyBurst;
        return false;
    }
}