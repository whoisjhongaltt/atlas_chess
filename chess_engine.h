/* This is my attempt to create stb_ like single file header library for chess
        https://github.com/nothings/stb/blob/master/docs/stb_howto.txt


        Do this #define CHESSLIB_IMPLEMENTATION
        before you include this file when you need function implementation
*/

#define TSZ_CHESS_API static

// typedefs for convience might need to exclude it later

#include <stdint.h>
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;
typedef int8_t i8;
typedef float f32;
typedef double f64;


typedef struct {

}sg_pieces;