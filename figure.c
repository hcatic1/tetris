#include "figure.h"
#include <stdio.h>  
#include <stdlib.h>


const char figures[7][4][4] = 
{   
    {{0b00000000, 0b00001000, 0b00001000, 0b00011000}, {0b00000000, 0b00000000, 0b00010000, 0b00011100}, {0b00000000, 0b00011000, 0b00010000, 0b00010000}, {0b00000000, 0b00000000, 0b00011100, 0b00010000}},
    {{0b00000000, 0b00010000, 0b00010000, 0b00011000}, {0b00000000, 0b00000000, 0b00111000, 0b00100000}, {0b00000000, 0b00011000, 0b00001000, 0b00001000}, {0b00000000, 0b00000000, 0b00001000, 0b00111000}},
    {{0b00010000, 0b00010000, 0b00010000, 0b00010000}, {0b00000000, 0b00000000, 0b00000000, 0b00111100}, {0b00001000, 0b00001000, 0b00001000, 0b00001000}, {0b00000000, 0b00000000, 0b00000000, 0b00111100}},
    {{0b00000000, 0b00000000, 0b00011000, 0b00011000}, {0b00000000, 0b00000000, 0b00011000, 0b00011000}, {0b00000000, 0b00000000, 0b00011000, 0b00011000}, {0b00000000, 0b00000000, 0b00011000, 0b00011000}},
    {{0b00000000, 0b00010000, 0b00011000, 0b00001000}, {0b00000000, 0b00000000, 0b00011000, 0b00110000}, {0b00000000, 0b00010000, 0b00011000, 0b00001000}, {0b00000000, 0b00000000, 0b00011000, 0b00110000}},
    {{0b00000000, 0b00001000, 0b00011000, 0b00010000}, {0b00000000, 0b00000000, 0b00011000, 0b00001100}, {0b00000000, 0b00001000, 0b00011000, 0b00010000}, {0b00000000, 0b00000000, 0b00011000, 0b00001100}},
    {{0b00000000, 0b00000000, 0b00001000, 0b00011100}, {0b00000000, 0b00001000, 0b00001100, 0b00001000}, {0b00000000, 0b00000000, 0b00011100, 0b00001000}, {0b00000000, 0b00001000, 0b00011000, 0b00001000}}
    
    
    
    
    
    
    
    /*
    {0b00000000, 0b00001000, 0b00001000, 0b00011000}, //J
    {0b00000000, 0b00010000, 0b00010000, 0b00011000}, //L
    {0b00010000, 0b00010000, 0b00010000, 0b00010000}, //I
    {0b00000000, 0b00000000, 0b00011000, 0b00011000}, //cube
    {0b00000000, 0b00010000, 0b00011000, 0b00001000}, //4
    {0b00000000, 0b00001000, 0b00011000, 0b00010000}, //z
    {0b00000000, 0b00000000, 0b00010000, 0b00111000}, //inverted T
     */
};
 
char a = 3;
char temp[4] = {0,0,0,0 }, pow;
char fig = 0;
char rotation = 0;
char position = 0;
char random_number(char max_number){
        srand((a)); 
        a+=5;
        a *= rand();
        return (a)%max_number;    
}


void choose_new_figure(char* fig_bin_array){
    fig = random_number(7);
    rotation = random_number(4);
    position = 0;
    const char *pom = figures[fig][rotation];
    for(char i=0; i<4; i++){
        fig_bin_array[i] = pom[i];
    }
    
}

void prepare_new_figure(char* matrix, char* fig_bin_array){
    choose_new_figure(fig_bin_array);
    for(char i=0; i<4; i++)
        matrix[i] = fig_bin_array[i];
}


__bit can_go_further(char* matrix, char* matrix_row, char* fig_bin_array){
    while(!matrix[(*matrix_row)]) (*matrix_row)--;
    for(int i = 1; i < 4; i++){
        if(fig_bin_array[3 - i] & (matrix[(*matrix_row)+ 1 - i] ^ fig_bin_array[3 - i + 1]))return 0;
    }
    return (*matrix_row) < 19 && !(fig_bin_array[3] & (matrix[(*matrix_row)+ 1])); 
}

__bit can_go_right(char* matrix, char* matrix_row, char *matrix_col, char* fig_bin_array ){
    for(int i = 0; i < 4; i++){
        if(((matrix[*matrix_row - 3 + i] & (1<<7))) || ((matrix[*matrix_row - i] ^ fig_bin_array[3-i])&fig_bin_array[3-i]*2)) return 0;
    }
    return 1;
}
__bit can_go_right_rot(char* matrix, char* matrix_row, char* fig_bin_array){
    for(int i = 0; i < 4; i++){
        if(((matrix[*matrix_row - 3 + i] & (1<<7))) || ((matrix[*matrix_row - i] ^ fig_bin_array[3-i])&temp[3-i]*2)) return 0;
    }
    return 1;
}
__bit can_go_left(char* matrix, char* matrix_row, char *matrix_col, char* fig_bin_array){
    for(int i = 0; i < 4; i++){
        if((matrix[*matrix_row - 3 + i] & (1<<0)) || ((matrix[*matrix_row - i] ^ fig_bin_array[3-i])&fig_bin_array[3-i]/2)) return 0;
    }
    return 1;
}
__bit can_go_left_rot(char* matrix, char* matrix_row, char* fig_bin_array){
    for(int i = 0; i < 4; i++){
        if((matrix[*matrix_row - 3 + i] & (1<<0)) || ((matrix[*matrix_row - i] ^ fig_bin_array[3-i])&temp[3-i]/2)) return 0;
    }
    return 1;
}
void go_down_1place(char* matrix, char* matrix_row, char* fig_bin_array){
    for(char i=0; i<4; i++){
        matrix[(*matrix_row)+1-i] = matrix[(*matrix_row)+1-i] | (fig_bin_array[3-i]);
        matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]);
    }
    (*matrix_row)++;
}

void go_right(char* matrix, char* matrix_row, char *matrix_col, char* fig_bin_array){
    if(can_go_right(matrix, matrix_row, matrix_col, fig_bin_array)){
        for(char i=0; i<4; i++){
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]);
            fig_bin_array[3-i] = fig_bin_array[3-i]*2;
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] | (fig_bin_array[3-i]);
        }
        position++;
    }
}
void go_right_rot(char* matrix, char* matrix_row, char* fig_bin_array){
    if(can_go_right_rot(matrix, matrix_row, fig_bin_array)){
        for(char i=0; i<4; i++){
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]);
            temp[3-i] = temp[3-i]*2;
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] | (temp[3-i]);
        }
    }
}
void go_left(char* matrix, char* matrix_row, char *matrix_col, char* fig_bin_array){
    if(can_go_left(matrix, matrix_row, matrix_col, fig_bin_array)){
        for(char i=0; i<4; i++){
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]);
            fig_bin_array[3-i] = fig_bin_array[3-i]/2;
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] | (fig_bin_array[3-i]);
        }
        position--;
    }
}
void go_left_rot(char* matrix, char* matrix_row, char* fig_bin_array){
    if(can_go_left_rot(matrix, matrix_row, fig_bin_array)){
        for(char i=0; i<4; i++){
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]);
            temp[3-i] = temp[3-i]/2;
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] | (temp[3-i]);
        }
    }
}
__bit can_rotate(char* matrix, char* matrix_row, char *matrix_col, char* fig_bin_array){
    if(!*matrix_col && fig_bin_array[1]) return 0;
    rotation++;
    for(char i=0; i < 4; i++){
            temp[i] = figures[fig][rotation%4][i];
    }
    char pos = position;
    while(pos > 0){
        if(pos == position) go_right_rot(matrix, matrix_row, fig_bin_array);
        else go_right_rot(matrix, matrix_row, temp);
        pos--;
    }
    while(pos < 0){
        if(pos == position) go_left_rot(matrix, matrix_row, fig_bin_array);
        else go_left_rot(matrix, matrix_row, temp);
        pos++;
    }
    for(char i = 0; i < 4; i++){
        if((matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i])) & temp[3-i]) return 0;
    }
    return 1;
}
void rotate(char* matrix, char* matrix_row, char *matrix_col, char* fig_bin_array){
    if(can_rotate(matrix, matrix_row, matrix_col, fig_bin_array)){
        for(char i=0; i < 4; i++){
            matrix[(*matrix_row)-i] = (matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i])) | temp[3-i];
            fig_bin_array[3-i] = temp[3-i];
        }
    }
}
void remove_full_rows(char* matrix){
    for(char i = 0; i<20; i++){
        if(matrix[19-i]==255){
            char j=19-i;
            while(j != 0){
                matrix[j]=matrix[j-1];
                j--;
            }i--;
        }
    }
}
