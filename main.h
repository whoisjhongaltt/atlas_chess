#ifndef MAIN_H
#define MAIN_H
#include <stdint.h>

typedef uint32_t u32;
typedef int32_t  i32;
typedef uint64_t u64;
typedef int64_t  i64;
typedef uint16_t u16;
typedef int16_t  i16;
typedef uint8_t  u8;
typedef int8_t  i8;
typedef float   f32;
typedef double  f64;

typedef enum {
  NONE = 0,
  WHITE_PAWN,
  WHITE_KING,
  WHITE_QUEEN,
  WHITE_ROOK,
  WHITE_BISHOP,
  WHITE_KNIGHT,
  BLACK_PAWN,
  BLACK_KING,
  BLACK_QUEEN,
  BLACK_ROOK,
  BLACK_BISHOP,
  BLACK_KNIGHT,
}CMPieces;

typedef struct 
{
  i8 x, y;
}Pos;

typedef struct Move Move;
struct Move
{
  Pos start, end;

  Move* next;
};

typedef struct MoveAudit MoveAudit;

struct MoveAudit
{
	Pos start, end;
	char str[10]; //move in notation 
	MoveAudit* next;
};

typedef struct Coord{
	char file;
	int  rank;
}Coord;

typedef struct{
  CMPieces  pieces[64];


  char active_color; //'w' for for white 'b' for black
  char castling[5]; // e.g "KQkg"  or "0";
  char en_passant[3]; //e.g "e3" or "-"

  u8 half_move_clock;
  u8 full_move_clock;


  Pos        selected;
  Move*      possible_moves;
  MoveAudit* audit;

  Texture2D textures[13]; //leave the first one blank because it's none
  char* fen;
}CMBoardState;

Pos get_curr_mouse_pos();
inline i8 cm_pos_to_index(i8 x, i8 y);
void cm_generate_fen(CMBoardState* board, char* fen);
void cm_append_audit(CMBoardState* board, MoveAudit** head_ptr, Pos start, Pos end);
char cm_get_fen_char(CMPieces piece);

#endif