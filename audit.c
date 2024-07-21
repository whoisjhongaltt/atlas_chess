
#include "main.h"

/*
	https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
*/

Coord cm_pos_to_coord(Pos p){
	Coord ret;
	ret.file = 97 + p.x;
	ret.rank = 8  - p.y;
	return ret;
}

void cm_generate_move_notation(CMBoardState* board, 
	Pos start, Pos end, char* str
	) {

	CMPieces start_piece = cm_get_piece_from_pos(board, start);
	CMPieces   end_piece = cm_get_piece_from_pos(board, end);

	Coord start_cord = cm_pos_to_coord(start);
	Coord end_cord   = cm_pos_to_coord(end);

	char piece = cm_get_fen_char(start_piece);

	sprintf(str, "%c%c%d", piece, end_cord.file, end_cord.rank);

}

//what do we need to undo a move

/* 
	by knowing just the move we can know which piece was moved
	we need to know if a piece was taken

	in this function call the move hasn't actually been made in the board
*/

void cm_append_audit(CMBoardState* board, MoveAudit** head_ptr, Pos start, Pos end)
{
  if(!cm_pos_in_bounds(end)) return;
  assert(head_ptr != NULL);
  MoveAudit* new  = malloc(sizeof(Move));
  new->start = start;
  new->end   = end;
  new->next  = NULL;

  cm_generate_move_notation(board, start, end, new->str);

  if(*head_ptr == NULL){
    *head_ptr = new;
  }else{
    MoveAudit* curr  = *head_ptr;

    while(curr->next != NULL){
      curr = curr->next;
    }

    curr->next = new;
  }
}

