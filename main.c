#include "main.h"

#include <errno.h>
#include <stdlib.h>

#include "audit.c"
#include "moves.c"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// display tiles

// The pieces are in no particular order
// the order could be the points of the pieces

Font f;

// Initilizes the board to a state when a new game is started
// We always suppose white is at bottom and black is at top
// If we wanna flip the board we just flip it in render function
void cm_init_board(CMBoardState* board) {
  board->pieces[0] = BLACK_ROOK;
  board->pieces[1] = BLACK_KNIGHT;
  board->pieces[2] = BLACK_BISHOP;
  board->pieces[3] = BLACK_QUEEN;
  board->pieces[4] = BLACK_KING;
  board->pieces[5] = BLACK_BISHOP;
  board->pieces[6] = BLACK_KNIGHT;
  board->pieces[7] = BLACK_ROOK;

  for (int i = 0; i < 8; ++i) {
    board->pieces[8 + i] = BLACK_PAWN;
  }

  board->pieces[56 + 0] = WHITE_ROOK;
  board->pieces[56 + 1] = WHITE_KNIGHT;
  board->pieces[56 + 2] = WHITE_BISHOP;
  board->pieces[56 + 3] = WHITE_QUEEN;
  board->pieces[56 + 4] = WHITE_KING;
  board->pieces[56 + 5] = WHITE_BISHOP;
  board->pieces[56 + 6] = WHITE_KNIGHT;
  board->pieces[56 + 7] = WHITE_ROOK;

  for (int i = 0; i < 8; ++i) {
    board->pieces[48 + i] = WHITE_PAWN;
  }

  board->selected.x = -1;
  board->selected.y = -1;

  board->fen = (char*)malloc(1000);
  board->active_color = 'w';
  strcpy(board->castling, "KQkg");
  strcpy(board->en_passant, "-");
}

void cm_init_textures(CMBoardState* board) {
  board->textures[WHITE_PAWN] = LoadTexture("data/white_pawn.png");
  board->textures[WHITE_KING] = LoadTexture("data/white_king.png");
  board->textures[WHITE_QUEEN] = LoadTexture("data/white_queen.png");
  board->textures[WHITE_ROOK] = LoadTexture("data/white_rook.png");
  board->textures[WHITE_BISHOP] = LoadTexture("data/white_bishop.png");
  board->textures[WHITE_KNIGHT] = LoadTexture("data/white_knight.png");
  board->textures[BLACK_PAWN] = LoadTexture("data/black_pawn.png");
  board->textures[BLACK_KING] = LoadTexture("data/black_king.png");
  board->textures[BLACK_QUEEN] = LoadTexture("data/black_queen.png");
  board->textures[BLACK_ROOK] = LoadTexture("data/black_rook.png");
  board->textures[BLACK_BISHOP] = LoadTexture("data/black_bishop.png");
  board->textures[BLACK_KNIGHT] = LoadTexture("data/black_knight.png");
}

inline i8 cm_pos_to_index(i8 x, i8 y) {
  i8 ret = y * 8 + x;
  assert(ret < 64);
  return ret;
}

char cm_get_fen_char(CMPieces piece) {
  switch (piece) {
    case WHITE_PAWN:
      return 'P';
    case WHITE_KING:
      return 'K';
    case WHITE_QUEEN:
      return 'Q';
    case WHITE_ROOK:
      return 'R';
    case WHITE_BISHOP:
      return 'B';
    case WHITE_KNIGHT:
      return 'N';
    case BLACK_PAWN:
      return 'p';
    case BLACK_KING:
      return 'k';
    case BLACK_QUEEN:
      return 'q';
    case BLACK_ROOK:
      return 'r';
    case BLACK_BISHOP:
      return 'b';
    case BLACK_KNIGHT:
      return 'n';
    case NONE:
      return '1';  // Placeholder for empty square
    default:
      return ' ';
  }
}

/*
  FEN form

  PIECES active color  castling rights en pasant halfmove clock fullmove number
 */

void cm_generate_fen(CMBoardState* board, char* fen) {
  u8 empty_count = 0;
  u8 index = 0;

  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      CMPieces piece = board->pieces[cm_pos_to_index(file, rank)];

      if (piece == NONE) {
        empty_count += 1;
      } else {
        if (empty_count > 0) {
          fen[index++] = '0' + empty_count;  // Convert number to char
          empty_count = 0;
        }
        fen[index++] = cm_get_fen_char(piece);
      }
    }

    if (empty_count > 0) {
      fen[index++] = '0' + empty_count;
      empty_count = 0;
    }

    if (rank < 7) {
      fen[index++] = '/';
    }
  }

  fen[index++] = ' ';
  fen[index++] = board->active_color;

  // Add castling availability
  fen[index++] = ' ';
  strcpy(&fen[index], board->castling);
  index += strlen(board->castling);

  // Add en passant target square
  fen[index++] = ' ';
  strcpy(&fen[index], board->en_passant);
  index += strlen(board->en_passant);

  // Add halfmove clock
  index += sprintf(&fen[index], " %d", board->half_move_clock);

  // Add fullmove number
  index += sprintf(&fen[index], " %d", board->full_move_clock);

  fen[index] = '\0';
}

void cm_display_possible_moves(CMBoardState* board) {
  Move* curr = board->possible_moves;

  while (curr) {
    DrawCircle(137 + curr->end.x * 75, 37 + curr->end.y * 75, 15, BEIGE);
    curr = curr->next;
  }
}

// This returns the position in the board
Pos get_curr_mouse_pos() {
  Vector2 mouse_pos = GetMousePosition();

  f32 x_offset = 100.0f;

  mouse_pos.x -= x_offset;

  Pos ret;
  if (mouse_pos.x < 0) {
    ret.x = -1;
    ret.y = -1;
    return ret;
  }

  if (mouse_pos.x > 8 * 75) {
    ret.x = -1;
    ret.y = -1;
    return ret;
  }

  if (mouse_pos.y > 8 * 75) {
    ret.x = -1;
    ret.y = -1;
    return ret;
  }

  ret.x = mouse_pos.x / 75.0f;
  ret.y = mouse_pos.y / 75.0f;

  return ret;
}

void cm_display_guide() {
  for (int x = 0; x < 8; ++x) {
    char str[2];
    sprintf(str, "%c", 65 + x);
    Vector2 pos = {105 + x * 75, 8 * 75 + 10};
    DrawTextEx(f, str, pos, f.baseSize / 2.0f, 0, BLACK);
  }
  for (int y = 0; y < 8; ++y) {
    char str[2];

    sprintf(str, "%d", 8 - y);
    Vector2 pos = {90, y * 75};
    DrawTextEx(f, str, pos, f.baseSize / 2.0f, 0, BLACK);
  }
}

void cm_draw_audit_list(CMBoardState* board) {
  Rectangle rec;
  rec.x = 750;
  rec.y = 20;
  rec.width = 200;
  rec.height = 700;

  int active;
  int scroll;
  // const char *str = "Samrat;Ghale";

  MoveAudit* curr = board->audit;

  int len = 1;
  while (curr) {
    len += 1;
    curr = curr->next;
  }

  char* str = malloc(len * 15 * sizeof(char));

  curr = board->audit;
  int index = 0;

  index += sprintf(str, "Moves list");
  while (curr) {
    // index += sprintf(str + index, ";(%d, %d) (%d,%d)",curr->start.x,
    // curr->start.y, curr->end.x, curr->end.y);
    index += sprintf(str + index, ";%s", curr->str);
    curr = curr->next;
  }

  GuiListView(rec, str, &scroll, &active);

  free(str);

  // printf("(%d, %d) (%d,%d)\n",p.x, p.y, curr->start.x, curr->start.y);
}

int main() {
  const int screenWidth = 1000;
  const int screenHeight = 900;

  InitWindow(screenWidth, screenHeight, "Chess mate");
  SetTargetFPS(60);

  CMBoardState board = {0};

  cm_init_textures(&board);
  cm_init_board(&board);

  cm_generate_fen(&board, board.fen);

  Pos p = {6, 0};
  Move* moves = cm_generate_knight_moves(&board, p);

  Rectangle rect = {10, 700, 500, 50};

  f = LoadFont("c:/Windows/Fonts/Consola.ttf");

  Vector2 v = {200, 700};
  GuiSetStyle(DEFAULT, TEXT_SIZE, 14);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      cm_update_game(&board);
    }

    cm_display_guide();

    for (int x = 0; x < 8; ++x) {
      for (int y = 0; y < 8; ++y) {
        Pos now = {x, y};

        if ((x + y) % 2 == 0) {
          DrawRectangle(100 + x * 75, y * 75, 75, 75, GRAY);
        } else {
          DrawRectangle(100 + x * 75, y * 75, 75, 75, WHITE);
        }

        int index = cm_pos_to_index(x, y);
        if (board.pieces[index] != NONE) {
          DrawTexture(board.textures[board.pieces[index]], 105 + x * 75,
                      y * 75 + 5, WHITE);
        }

        if (now.x == board.selected.x && now.y == board.selected.y) {
          Rectangle rec = {100 + x * 75, y * 75, 75, 75};

          DrawRectangleLinesEx(rec, 5, GREEN);
        }
      }
    }

    cm_generate_fen(&board, board.fen);
    cm_display_possible_moves(&board);

    DrawTextEx(f, board.fen, v, f.baseSize / 2.0f, 0, BLACK);
    cm_draw_audit_list(&board);

    // GuiStatusBar(rect, fen);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
