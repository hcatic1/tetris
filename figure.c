#include "figure.h"
#include <stdio.h>  
#include <stdlib.h>


const char figures[7][4][4] = 
{   
    {{0b00000000, 0b00001000, 0b00001000, 0b00011000}, {0b00000000, 0b00000000, 0b00010000, 0b00011100}, {0b00000000, 0b00011000, 0b00010000, 0b00010000}, {0b00000000, 0b00000000, 0b00011100, 0b00000100}},
    {{0b00000000, 0b00010000, 0b00010000, 0b00011000}, {0b00000000, 0b00000000, 0b00111000, 0b00100000}, {0b00000000, 0b00011000, 0b00001000, 0b00001000}, {0b00000000, 0b00000000, 0b00001000, 0b00111000}},
    {{0b00010000, 0b00010000, 0b00010000, 0b00010000}, {0b00000000, 0b00000000, 0b00000000, 0b00111100}, {0b00001000, 0b00001000, 0b00001000, 0b00001000}, {0b00000000, 0b00000000, 0b00000000, 0b00111100}},
    {{0b00000000, 0b00000000, 0b00011000, 0b00011000}, {0b00000000, 0b00000000, 0b00011000, 0b00011000}, {0b00000000, 0b00000000, 0b00011000, 0b00011000}, {0b00000000, 0b00000000, 0b00011000, 0b00011000}},
    {{0b00000000, 0b00010000, 0b00011000, 0b00001000}, {0b00000000, 0b00000000, 0b00011000, 0b00110000}, {0b00000000, 0b00010000, 0b00011000, 0b00001000}, {0b00000000, 0b00000000, 0b00011000, 0b00110000}},
    {{0b00000000, 0b00001000, 0b00011000, 0b00010000}, {0b00000000, 0b00000000, 0b00011000, 0b00001100}, {0b00000000, 0b00001000, 0b00011000, 0b00010000}, {0b00000000, 0b00000000, 0b00011000, 0b00001100}},
    {{0b00000000, 0b00000000, 0b00001000, 0b00011100}, {0b00000000, 0b00001000, 0b00001100, 0b00001000}, {0b00000000, 0b00000000, 0b00011100, 0b00001000}, {0b00000000, 0b00001000, 0b00011000, 0b00001000}}
    
 
};
 
char temp[4] = {0,0,0,0}; //temporary variable
char figure = 0; //variable that tracks which of the seven figures is the current figure
char rotation = 0; //variable that tracks which rotation of the figure is currently displayed or rather in which rotation is the figure
signed char position = 0; //variable that tracks how many times figure was moved from the center line, left or right - fourth column from the right

//implementation of the function copy4 whose prototype is given in the figure.h header
void copy4 (char* array1, char* array2){
    for(int i=0; i<4; i++)
        array1[i] = array2[i];
}

//implementation of the function move whose prototype is given in the figure.h header
__bit move(char* temp, char* rotatedFigure, signed char* position){
    double x = 1;
    signed char position2 = (*position);  //temporary variable
    copy4(temp, rotatedFigure); 
    
    //position greater than zero indicates that figure had been moved to the left
    while(position2 > 0){
        for(int i=0 ; i<4; i++){
            if((temp[i] & (1<<7))) return 0; //tests if moving figure left is possible, if not function immediately stops and returns false, move not possible, move illegal
            temp[i] = temp[i]*2; //move left, in binary representation this means multiply with 2
        }
        position2--;
    }
    while(position2 < 0){
        for(int i=0 ; i<4; i++){
            if((temp[i] & (1<<0))) return 0; //tests if moving figure right is possible, if not function immediately stops and returns false, move not possible, move illegal
            temp[i] = temp[i]/2; //move right, in binary representation this means divide by 2
        }
        position2++;
    }
    return 1; //if test didn't fail by now move is possible
    
}

//implementation of the function choose_new_figure whose prototype is given in the figure.h header
char random_number(char max_number, char x){
        srand((x)); //using x as a random seed
        x+=5;
        x *= rand();
        return (x)%max_number;   //returns pseudo-random number between 0 and max_number
}

//implementation of the function choose_new_figure whose prototype is given in the figure.h header
void choose_new_figure(char* fig_bin_array, char x){
    figure = random_number(7,x); //takes random number between 0 and 6
    rotation = random_number(4,x); //takes random number between 0 and 3
    position = 0;
    const char *pom = figures[figure][rotation]; //chooses one random figure from figures
    for(char i=0; i<4; i++) fig_bin_array[i] = pom[i];  //copy random figure to fig_bin_array
}

//implementation of the function prepare_new_figure whose prototype is given in the figure.h header
void prepare_new_figure(char* matrix, char* fig_bin_array, char x){
    choose_new_figure(fig_bin_array, x);
    for(char i=0; i<4; i++)
        matrix[i] = fig_bin_array[i]; //place choosen figure into first 4 rows of matrix
}

//implementation of the function can_go_further whose prototype is given in the figure.h header
__bit can_go_further(char* matrix, char* matrix_row, char* fig_bin_array){
    while(!matrix[(*matrix_row)]) (*matrix_row)--;
    for(int i = 1; i < 4; i++){
        if(fig_bin_array[3 - i] & (matrix[(*matrix_row)+ 1 - i] ^ fig_bin_array[3 - i + 1]))return 0; //checks for collision between figure and current state of matrix
    }
    return (*matrix_row) < 19 && !(fig_bin_array[3] & (matrix[(*matrix_row)+ 1]));  //checks if figure is on bottom
}

//implementation of the function can_go_right whose prototype is given in the figure.h header
__bit can_go_right(char* matrix, char* matrix_row, char* fig_bin_array ){
    for(int i = 0; i < 4; i++){ 
        if(((matrix[*matrix_row - 3 + i] & (1<<7))) || ((matrix[*matrix_row - i] ^ fig_bin_array[3-i])&fig_bin_array[3-i]*2)) return 0; //firstly checks if the figure has reached edge if so further moving it is an illegal move, secondly checks if there is collisions between moved figure and matrix (matrix without the original figure on it) - moving figure right in binary representation achieved by multiplying with 2 
    }
    return 1;
}

//implementation of the function can_go_left whose prototype is given in the figure.h header
__bit can_go_left(char* matrix, char* matrix_row,char* fig_bin_array){
    for(int i = 0; i < 4; i++){
        if((matrix[*matrix_row - 3 + i] & (1<<0)) || ((matrix[*matrix_row - i] ^ fig_bin_array[3-i])&fig_bin_array[3-i]/2)) return 0; //firstly checks if the figure has reached edge if so further moving it is an illegal move, secondly checks if there is collisions between moved figure and matrix (matrix without the original figure on it) - moving figure left in binary representation achieved by dividing by 2 
    }
    return 1;
}

//implementation of the function can_rotate whose prototype is given in the figure.h header
__bit can_rotate(char* matrix, char* matrix_row, char* fig_bin_array, char* temp){
    for(int i=0; i<4; i++)
        if(temp[3 - i] & (matrix[(*matrix_row) - i] ^ fig_bin_array[3 - i]))return 0; //checks if there are collision between figures already on matrix (matrix when original figure is removed) and the rotated figure
}

//implementation of the function go_down_1place whose prototype is given in the figure.h header
void go_down_1place(char* matrix, char* matrix_row, char* fig_bin_array){
    for(char i=0; i<4; i++){
        matrix[(*matrix_row)+1-i] = matrix[(*matrix_row)+1-i] | (fig_bin_array[3-i]); //place current figure row into row below 
        matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]); //remove current figure row from matrix
    }
    (*matrix_row)++;
}

//implementation of the function go_right whose prototype is given in the figure.h header
void go_right(char* matrix, char* matrix_row, char* fig_bin_array){
    if(can_go_right(matrix, matrix_row, fig_bin_array)){ //checks if the move is possible/legal
        for(char i=0; i<4; i++){
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]); //removing current figure from matrix
            fig_bin_array[3-i] = fig_bin_array[3-i]*2; //moving figure right, in binary representation, multiplying by 2
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] | (fig_bin_array[3-i]); //putting the figure on matrix, binary, bitwise
        }
        position++; //updating position in case of rotation
    }
}

//implementation of the function go_left whose prototype is given in the figure.h header
void go_left(char* matrix, char* matrix_row, char* fig_bin_array){
    if(can_go_left(matrix, matrix_row, fig_bin_array)){ //checks if move is possible/legal
        for(char i=0; i<4; i++){
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]); //removing current figure from matrix
            fig_bin_array[3-i] = fig_bin_array[3-i]/2; //moving figure left, in binary representation, dividing by 2
            matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] | (fig_bin_array[3-i]); //putting the figure on matrix, binary, bitwise
        }
        position--; //updating position in case of rotation
    }
}

//implementation of the function rotate whose prototype is given in the figure.h header
void rotate(char* matrix, char* matrix_row, char* fig_bin_array){
    char rotation2 = (rotation+1)%4; //updates which rotation of figure is to be considered
    if(move(temp, figures[figure][rotation2], &position)) //moves rotation of figure to temporary variable in respect to position - how many times the figure had been moved left or right if possisble
        if(can_rotate(matrix, matrix_row, fig_bin_array, temp)){ //checks if move legal/possible
            for(int i=0; i<4; i++){
                matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] ^ (fig_bin_array[3-i]); //removing current figure from matrix
                fig_bin_array[3-i] = temp[3-i]; //updating figure
                matrix[(*matrix_row)-i] = matrix[(*matrix_row)-i] | (fig_bin_array[3-i]); //putting the figure on matrix, binary, bitwise
            }
            rotation = rotation2; //updated rotation variable
        }
}

//implementation of the function remove_full_rows whose prototype is given in the figure.h header
void remove_full_rows(char* matrix, char* time){
    for(char i = 0; i<20; i++){ //goes through every row of matrix bottom to top
        if(matrix[19-i]==255){ //if row is full
            (*time) = ((*time)+2)%256; // increases game speed
            char j=19-i; //row j is full
            while(j != 0){ //move all rows above j one down one by one
                matrix[j]=matrix[j-1];
                j--; //going a row up and then moving it down
            }i--; //check the rest of the matrix
        }
    }
}
