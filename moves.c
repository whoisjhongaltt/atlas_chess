#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"

inline bool is_pos_equal(Pos a, Pos b) { return (a.x == b.x && a.y == b.y); }

inline bool cm_is_piece_black(CMPieces piece) { return (int)piece > 6; }

inline bool cm_is_piece_active_color(CMBoardState* board, CMPieces piece) {
  if (piece == NONE)
    return false;
  else if (cm_is_piece_black(piece) && board->active_color == 'b') {
    return true;
  } else if (!cm_is_piece_black(piece) && board->active_color == 'w') {
    return true;
  }

  return false;
}

inline bool cm_is_same_color(CMPieces a, CMPieces b) {
  if (b == NONE || a == NONE)
    return false;

  else if (cm_is_piece_black(a) && cm_is_piece_black(b)) {
    return true;
  } else if (!cm_is_piece_black(a) && !cm_is_piece_black(b)) {
    return true;
  }

  return false;
}
// check if the position is outside the board
inline bool cm_pos_in_bounds(Pos pos) {
  return !((pos.x > 7 || pos.x < 0) || (pos.y > 7 || pos.y < 0));
}

void cm_list_append(Move** head_ptr, Pos start, Pos end) {
  if (!cm_pos_in_bounds(end)) return;

  assert(head_ptr != NULL);
  Move* new_move = (Move*)malloc(sizeof(Move));
  new_move->start = start;
  new_move->end = end;
  new_move->next = NULL;

  if (*head_ptr == NULL) {
    *head_ptr = new_move;
  } else {
    Move* curr = *head_ptr;

    while (curr->next != NULL) {
      curr = curr->next;
    }

    curr->next = new_move;
  }
}

void cm_list_print(Move* head) {
  Move* curr = head;
  while (curr) curr = curr->next;
}

inline void cm_print_pos(Pos pos) { printf("Pos = (%d, %d) \n", pos.x, pos.y); }

/*
  Given a pawn position generate moves

  RULES

  Check if there is a piece ahead



*/
Move* cm_generate_white_pawn_moves(CMBoardState* board, Pos p) {
  Move* head = NULL;

  Pos move_forward;
  move_forward.x = p.x;
  move_forward.y = p.y - 1;

  if (cm_get_piece_from_pos(board, move_forward) == NONE) {
    cm_list_append(&head, p, move_forward);

    // if it can't move forware it can't move two moves forward
    if (p.y == 6) {
      move_forward.y -= 1;
      if (cm_get_piece_from_pos(board, move_forward) == NONE) {
        cm_list_append(&head, p, move_forward);
      }
    }
  }

  // Check if it can take

  Pos diag;
  diag.x = p.x + 1;
  diag.y = p.y - 1;

  if (cm_get_piece_from_pos(board, diag) != NONE &&
      cm_is_piece_black(cm_get_piece_from_pos(board, diag))) {
    cm_list_append(&head, p, diag);
  }

  diag.x = p.x - 1;
  diag.y = p.y - 1;

  if (cm_get_piece_from_pos(board, diag) != NONE &&
      cm_is_piece_black(cm_get_piece_from_pos(board, diag))) {
    cm_list_append(&head, p, diag);
  }

  return head;
}

/*
  GO thru 4 direction
  Stop if stumbled on a piece or out of the board
*/
Move* cm_generate_rook_moves(CMBoardState* board, Pos p) {
  Move* head = NULL;
  CMPieces base_piece = cm_get_piece_from_pos(board, p);

  // top
  {
    Pos new_pos = p;
    new_pos.y -= 1;
    while (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if (new_piece == NONE)
        cm_list_append(&head, p, new_pos);
      else if (cm_is_same_color(base_piece, new_piece))
        break;
      else {
        cm_list_append(&head, p, new_pos);
        break;
      }
      new_pos.y -= 1;
    }
  }

  // bottom
  {
    Pos new_pos = p;
    new_pos.y += 1;
    while (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if (new_piece == NONE)
        cm_list_append(&head, p, new_pos);
      else if (cm_is_same_color(base_piece, new_piece))
        break;
      else {
        cm_list_append(&head, p, new_pos);
        break;
      }
      new_pos.y += 1;
    }
  }

  // left
  {
    Pos new_pos = p;
    new_pos.x -= 1;
    while (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if (new_piece == NONE)
        cm_list_append(&head, p, new_pos);
      else if (cm_is_same_color(base_piece, new_piece))
        break;
      else {
        cm_list_append(&head, p, new_pos);
        break;
      }
      new_pos.x -= 1;
    }
  }

  // right
  {
    Pos new_pos = p;
    new_pos.x += 1;
    while (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if (new_piece == NONE)
        cm_list_append(&head, p, new_pos);
      else if (cm_is_same_color(base_piece, new_piece))
        break;
      else {
        cm_list_append(&head, p, new_pos);
        break;
      }
      new_pos.x += 1;
    }
  }
  return head;
}

/*
  Go thru the 4 direction in a while loop
  Stop if stumbled on a piece or out of the board

*/
Move* cm_generate_bishop_moves(CMBoardState* board, Pos p) {
  Move* head = NULL;

  CMPieces base_piece = cm_get_piece_from_pos(board, p);

  // bottom left
  {
    Pos bottom_left = p;
    bottom_left.x += 1;
    bottom_left.y += 1;

    while (cm_pos_in_bounds(bottom_left)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, bottom_left);

      if (new_piece == NONE) {
        cm_list_append(&head, p, bottom_left);
      }

      // same color exit
      else if (cm_is_same_color(base_piece, new_piece)) {
        break;
      } else {
        cm_list_append(&head, p, bottom_left);
        break;
      }

      bottom_left.x += 1;
      bottom_left.y += 1;
    }
  }

  // top right
  {
    Pos top_right = p;
    top_right.x -= 1;
    top_right.y -= 1;

    while (cm_pos_in_bounds(top_right)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, top_right);

      if (new_piece == NONE)
        cm_list_append(&head, p, top_right);
      else if (cm_is_same_color(base_piece, new_piece))
        break;

      else {
        cm_list_append(&head, p, top_right);
        break;
      }

      top_right.x -= 1;
      top_right.y -= 1;
    }
  }

  // bottom right
  {
    Pos top_right = p;
    top_right.x -= 1;
    top_right.y += 1;

    while (cm_pos_in_bounds(top_right)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, top_right);

      if (new_piece == NONE)
        cm_list_append(&head, p, top_right);
      else if (cm_is_same_color(base_piece, new_piece))
        break;

      else {
        cm_list_append(&head, p, top_right);
        break;
      }

      top_right.x -= 1;
      top_right.y += 1;
    }
  }
  // top left
  {
    Pos top_right = p;
    top_right.x += 1;
    top_right.y -= 1;

    while (cm_pos_in_bounds(top_right)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, top_right);

      if (new_piece == NONE)
        cm_list_append(&head, p, top_right);
      else if (cm_is_same_color(base_piece, new_piece))
        break;

      else {
        cm_list_append(&head, p, top_right);
        break;
      }

      top_right.x += 1;
      top_right.y -= 1;
    }
  }

  return head;
}

// Just combine the moves of rook and bishop
Move* cm_generate_queen_moves(CMBoardState* board, Pos p) {
  Move* rook_moves = cm_generate_rook_moves(board, p);
  Move* bishop_moves = cm_generate_bishop_moves(board, p);

  if (!rook_moves) {
    return bishop_moves;
  }

  Move* curr = rook_moves;
  while (curr->next) {
    curr = curr->next;
  }
  curr->next = bishop_moves;

  return rook_moves;
}

// later check if it's in check
Move* cm_generate_king_moves(CMBoardState* board, Pos p) {
  Move* head = NULL;
  CMPieces base_piece = cm_get_piece_from_pos(board, p);

  // up
  {
    Pos new_pos = p;
    new_pos.y -= 1;
    if (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if ((new_piece == NONE) || !cm_is_same_color(base_piece, new_piece)) {
        cm_list_append(&head, p, new_pos);
      }
    }
  }

  // down
  {
    Pos new_pos = p;
    new_pos.y += 1;
    if (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if ((new_piece == NONE) || !cm_is_same_color(base_piece, new_piece)) {
        cm_list_append(&head, p, new_pos);
      }
    }
  }

  // left
  {
    Pos new_pos = p;
    new_pos.x -= 1;
    if (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if ((new_piece == NONE) || !cm_is_same_color(base_piece, new_piece)) {
        cm_list_append(&head, p, new_pos);
      }
    }
  }

  // right
  {
    Pos new_pos = p;
    new_pos.x += 1;
    if (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if ((new_piece == NONE) || !cm_is_same_color(base_piece, new_piece)) {
        cm_list_append(&head, p, new_pos);
      }
    }
  }

  // top left
  {
    Pos new_pos = p;
    new_pos.y -= 1;
    new_pos.x -= 1;
    if (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if ((new_piece == NONE) || !cm_is_same_color(base_piece, new_piece)) {
        cm_list_append(&head, p, new_pos);
      }
    }
  }

  // bottom left
  {
    Pos new_pos = p;
    new_pos.y += 1;
    new_pos.x -= 1;
    if (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if ((new_piece == NONE) || !cm_is_same_color(base_piece, new_piece)) {
        cm_list_append(&head, p, new_pos);
      }
    }
  }

  // top right
  {
    Pos new_pos = p;
    new_pos.y -= 1;
    new_pos.x += 1;
    if (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if ((new_piece == NONE) || !cm_is_same_color(base_piece, new_piece)) {
        cm_list_append(&head, p, new_pos);
      }
    }
  }

  // bottom right
  {
    Pos new_pos = p;
    new_pos.y += 1;
    new_pos.x += 1;
    if (cm_pos_in_bounds(new_pos)) {
      CMPieces new_piece = cm_get_piece_from_pos(board, new_pos);

      if ((new_piece == NONE) || !cm_is_same_color(base_piece, new_piece)) {
        cm_list_append(&head, p, new_pos);
      }
    }
  }

  return head;
}

Move* cm_generate_black_pawn_moves(CMBoardState* board, Pos p) {
  Move* head = NULL;

  Pos move_forward;
  move_forward.x = p.x;
  move_forward.y = p.y + 1;

  if (cm_get_piece_from_pos(board, move_forward) == NONE) {
    cm_list_append(&head, p, move_forward);

    // if it can't move forware it can't move two moves forward
    if (p.y == 1) {
      move_forward.y += 1;
      if (cm_get_piece_from_pos(board, move_forward) == NONE) {
        cm_list_append(&head, p, move_forward);
      }
    }
  }

  // Check if it can take

  Pos diag;
  diag.x = p.x + 1;
  diag.y = p.y + 1;

  if (cm_get_piece_from_pos(board, diag) != NONE &&
      !cm_is_piece_black(cm_get_piece_from_pos(board, diag))) {
    cm_list_append(&head, p, diag);
  }

  diag.x = p.x + 1;
  diag.y = p.y - 1;

  if (cm_get_piece_from_pos(board, diag) != NONE &&
      !cm_is_piece_black(cm_get_piece_from_pos(board, diag))) {
    cm_list_append(&head, p, diag);
  }

  return head;
}

/*
  Given a knight position generate the move
   position where the knight can move
   This should also check if the postion is empty or not
   Empty means position without the current color of night



    First get all 8 of the knight moves

    1. Calculate the position the 8 moves
    2. Don't add the ones that are outside the board
    2. Don't add the ones that are at the same collered board

 */
Move* cm_generate_knight_moves(CMBoardState* board, Pos p) {
  Move* head = NULL;

  Pos moves[8] = {{-2, -1}, {-2, +1}, {-1, +2}, {-1, -2},
                  {+2, -1}, {+2, +1}, {+1, -2}, {+1, +2}};

  for (int i = 0; i < 8; ++i) {
    Pos new_move = {p.x + moves[i].x, p.y + moves[i].y};

    // check if it's inside the board

    if (!((new_move.x > 7 || new_move.x < 0) ||
          (new_move.y > 7 || new_move.y < 0))) {
      int index = cm_pos_to_index(new_move.x, new_move.y);

      // check if not empty

      if (board->pieces[index] == NONE) {
        cm_list_append(&head, p, new_move);
      } else {
        CMPieces piece = cm_get_piece_from_pos(board, p);
        CMPieces new_piece = cm_get_piece_from_pos(board, new_move);

        // if the current pos is none then just add it
        if (piece == NONE) {
          cm_list_append(&head, p, new_move);
        } else {
          if (cm_is_piece_black(piece) && !cm_is_piece_black(new_piece)) {
            cm_list_append(&head, p, new_move);
          } else if (!cm_is_piece_black(piece) &&
                     cm_is_piece_black(new_piece)) {
            cm_list_append(&head, p, new_move);
          }
        }
      }
    }
  }

  // cm_list_print(head);

  return head;
}

Move* cm_generate_moves(CMBoardState* board, CMPieces piece, Pos pos) {
  Move* ret = NULL;
  switch (piece) {
    case BLACK_KNIGHT:
    case WHITE_KNIGHT:
      ret = cm_generate_knight_moves(board, pos);
      return ret;
    case BLACK_PAWN:
      ret = cm_generate_black_pawn_moves(board, pos);
      return ret;
    case WHITE_PAWN:
      ret = cm_generate_white_pawn_moves(board, pos);
      return ret;
    case WHITE_BISHOP:
    case BLACK_BISHOP:
      ret = cm_generate_bishop_moves(board, pos);
      return ret;
    case WHITE_ROOK:
    case BLACK_ROOK:
      ret = cm_generate_rook_moves(board, pos);
      return ret;
    case BLACK_QUEEN:
    case WHITE_QUEEN:
      ret = cm_generate_queen_moves(board, pos);
      return ret;
    case BLACK_KING:
    case WHITE_KING:
      ret = cm_generate_king_moves(board, pos);
      return ret;
    default:
      ret = NULL;
  }
  return ret;
}

CMPieces cm_get_piece_from_pos(CMBoardState* board, Pos p) {
  return board->pieces[cm_pos_to_index(p.x, p.y)];
}

// Don't care if it's different or not just update it
//
void cm_update_possible_moves(CMBoardState* board) {
  CMPieces curr_piece = cm_get_piece_from_pos(board, board->selected);
  board->possible_moves = cm_generate_moves(board, curr_piece, board->selected);
}

// return if the move was made

// Check if the it's the same color as active_color
// Check if the p is in end of any of the possible moves
// if it's in the possible move then change the board->pieces and return true

bool cm_make_move(CMBoardState* board, Pos p) {
  Move* curr = board->possible_moves;

  bool found = false;

  CMPieces piece = cm_get_piece_from_pos(board, board->selected);

  if (!cm_is_piece_active_color(board, piece)) return false;

  while (curr) {
    if (is_pos_equal(curr->end, p)) {
      found = true;

      // add to audit

      cm_append_audit(board, &board->audit, curr->start, p);

      CMPieces piece = cm_get_piece_from_pos(board, curr->start);
      i8 i = cm_pos_to_index(curr->start.x, curr->start.y);
      board->pieces[i] = NONE;
      board->pieces[cm_pos_to_index(curr->end.x, curr->end.y)] = piece;

      //
    }
    curr = curr->next;
  }

  return found;
}

void cm_delete_list(Move** head_ptr) {
  Move* prev = *head_ptr;
  Move* curr = prev;

  while (curr) {
    prev = curr->next;
    free(curr);
    curr = prev;
  }
  *head_ptr = NULL;
}

// tthis should probably the only function that changes the state of the game
void cm_update_game(CMBoardState* board) {
  Pos pos = get_curr_mouse_pos();
  // cm_print_pos(board->selected);

  if (cm_make_move(board, pos)) {
    board->selected.x = -1;
    board->selected.y = -1;
    cm_update_possible_moves(board);
    cm_generate_fen(board, board->fen);

    if (board->active_color == 'b')
      board->active_color = 'w';
    else
      board->active_color = 'b';
  } else if (cm_is_piece_active_color(board,
                                      cm_get_piece_from_pos(board, pos))) {
    board->selected = pos;
    cm_delete_list(&board->possible_moves);
    cm_update_possible_moves(board);
  } else {
    cm_delete_list(&board->possible_moves);
    board->selected = pos;
  }
}
