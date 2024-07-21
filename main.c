#include <stdio.h>
#define GLT_CHESS_IMPLEMENTATION 1 
#include "glt_chess.h"

static void print_board(glt_chess_board board)
{
        printf("\n");
        for (int i = 63; i >= 0; i)
        {
                for (size_t j = 0; j < 8; j++)
                {
                        printf("%c ", glt_get_fen_char(board.pieces[(i-7) + j]));
                }
                i -= 8;
                printf("\n");
        }
        printf("\n");
}

static void print_moves(glt_move* moves){
        glt_move* curr = moves;
        while(curr){
                glt_coord cord = glt_pos_to_coord(curr->end);
                printf("{%c, %d}", cord.file,  cord.rank);
                curr = curr->next;
        }
}

int main(int argc, char const *argv[])
{
        glt_chess_board board;
        glt_initilize_board(&board);
        print_board(board);

        /*
        glt_coord cord = {'A', 2};
        glt_move* moves =   glt_generate_moves(&board, glt_coord_to_pos(cord));
        print_moves(moves);
        glt_make_move(&board, *moves);
        print_board(board);
        */

        glt_coord cord = {'B', 2};
        glt_move* moves = glt_generate_moves(&board, glt_coord_to_pos(cord));
        print_moves(moves);
        glt_make_move(&board, *moves);
        print_board(board);

        cord.file = 'C';
        cord.rank = 1;
        moves = glt_generate_moves(&board, glt_coord_to_pos(cord));
        print_moves(moves);
        glt_make_move(&board, *moves);
        print_board(board);
        return 0;
}
