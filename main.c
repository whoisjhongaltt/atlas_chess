#include <stdio.h>           
#define GLT_CHESS_IMPLEMENTATION 1 
#include "glt_chess.h"       
                             
#include "raylib.h"          
                             
typedef struct {             
        glt_chess_board board;
                             
        Texture2D textures[13]; //leave the first one empty

        glt_move* possible_moves;

        Font font;

        char fen[100]; //current board's fen
}GameState;                  
                             
const int screen_width = 1000;
const int screen_height = 800;
                             
static GameState game_state; 
                             
static void initialize_textures(void){
        game_state.textures[GLT_white_pawn  ]    = LoadTexture("./data/1_white_pawn.png");  
        game_state.textures[GLT_white_king  ]    = LoadTexture("./data/2_white_king.png");  
        game_state.textures[GLT_white_queen ]    = LoadTexture("./data/3_white_queen.png");  
        game_state.textures[GLT_white_rook  ]    = LoadTexture("./data/4_white_rook.png");  
        game_state.textures[GLT_white_bishop]    = LoadTexture("./data/5_white_bishop.png");  
        game_state.textures[GLT_white_knight]    = LoadTexture("./data/6_white_knight.png");  
        game_state.textures[GLT_black_pawn  ]    = LoadTexture("./data/7_black_pawn.png");  
        game_state.textures[GLT_black_king  ]    = LoadTexture("./data/8_black_king.png");  
        game_state.textures[GLT_black_queen ]   = LoadTexture("./data/9_black_queen.png");  
        game_state.textures[GLT_black_rook  ]   = LoadTexture("./data/10_black_rook.png");  
        game_state.textures[GLT_black_bishop]   = LoadTexture("./data/11_black_bishop.png");  
        game_state.textures[GLT_black_knight]   = LoadTexture("./data/12_black_knight.png");  
}

void draw_current_board(void)
{
        for (size_t i = 0; i < 64; i++)
        {
                glt_pos pos = glt_index_to_pos(i);

                if ((pos.x + pos.y) % 2 != 0) {
                        DrawRectangle(pos.x * 75, pos.y * 75, 75, 75, GRAY);
                } else {
                        DrawRectangle(pos.x * 75, pos.y * 75, 75, 75, WHITE);
                }

        }
        for (size_t i = 0; i < 64; i++){
                glt_pos pos = glt_index_to_pos(i);
                glt_piece curr_piece = game_state.board.pieces[glt_pos_to_index(pos)];

                if (curr_piece != GLT_none)
                {
                        DrawTexture(game_state.textures[curr_piece],   (pos.x) * 75 + 5,   (9 - pos.y) * 75 + 7, WHITE);
                }


        }

        //display possible moves

        glt_move* curr = game_state.possible_moves;

        while(curr){
                DrawCircle(curr->end.x * 75 + 40, 37 + (9 - curr->end.y) * 75 , 15, BEIGE);
                curr = curr->next;
        }
        
}

glt_pos get_curr_mouse_pos(){
        Vector2 mouse_pos = GetMousePosition();

        glt_pos ret;

        if(mouse_pos.x < 0){
                ret.x = -1;
                ret.y = -1;
                return ret;
        }

        if(mouse_pos.x >  9 * 75){
                ret.x = -1;
                ret.y = -1;
                return ret;
        }
        if(mouse_pos.y >  9 * 75){
                ret.x = -1;
                ret.y = -1;
                return ret;
        }

        ret.x = mouse_pos.x / 75.0f;
        ret.y = mouse_pos.y / 75.0f;
        ret.y = 9 - ret.y;
        //printf("%d, %d\n", ret.x, ret.y);
        return ret;
}

void update_game(){
        glt_pos pos = get_curr_mouse_pos();

        if(game_state.possible_moves){
                i8 made_move = 0;

                glt_move*  curr = game_state.possible_moves;
                while(curr){

                        if(glt_pos_is_equal(pos, curr->end)){
                                if(glt_make_move(&game_state.board, *curr) == 1){
                                        made_move = 1;
                                        glt_get_fen_from_board(&game_state.board, game_state.fen, 100);
                                }
                        }
                        curr = curr->next;
                }

                glt_moves_delte(&game_state.possible_moves);
        }else{
                glt_moves_delte(&game_state.possible_moves);
                game_state.possible_moves = glt_generate_moves(&game_state.board, pos);
        }

        printf("%s\n", game_state.fen);

}

void DrawTextB(const char *text, int posX, int posY, int fontSize, Color color)
{
    DrawTextEx(game_state.font, text, (Vector2){ posX, posY }, fontSize, 1, color);
}


int main(void)
{

        glt_initilize_board(&game_state.board);
        glt_get_fen_from_board(&game_state.board, game_state.fen, 100);

        InitWindow(screen_width, screen_height, "Atlas chess");

        SetTargetFPS(60);
        initialize_textures();

        Camera2D camera;
        camera.zoom = 1.0f;
        camera.target.x = screen_width/2, 
        camera.target.y = screen_height/2;
        camera.offset.x  = screen_width/2;
        camera.offset.y  = screen_height/2;

        game_state.font = LoadFontEx("c:/Windows/Fonts/Consola.ttf", 17, NULL, 0);
        
        //camera.rotation = 90.0f;
        while(!WindowShouldClose())
        {
                  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        update_game();
                  }
                  BeginDrawing();
                  ClearBackground(GRAY);
                  // DrawText("Hello world", 190, 200, 20, LIGHTGRAY);
                  BeginMode2D(camera);
                  // DrawRectangle(screen_width/2 , screen_height/2 , 75, 75,
                  // WHITE);
                  draw_current_board();

                  DrawTextB(game_state.fen, 50, 700, 17, BLACK); 

                  EndMode2D();
                  EndDrawing();

        }
        CloseWindow();
        return 0;
}
