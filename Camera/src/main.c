/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <resources.h>

#define IDLE 1
#define WALKING 2
#define ATTACK 3

u16 ind = TILE_USER_INDEX;
Map *level_map;

Sprite *player;
fix32 player_x = FIX32(0);
fix32 player_y = FIX32(820);
fix32 player_vel = FIX32(1.5);
int player_anim = IDLE;

//Variaveis da camera
int current_camera_x = 0;
int current_camera_y = 0;
#define HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES 20
#define HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES 200
#define HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES 10
#define HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES 50

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION 224

#define MAP_WIDTH 10240
#define MAP_HEIGHT 1280

#define PLAYER_WIDTH 72
#define PLAYER_HEIGHT 78

static void handleInput();
static void camera();


int main()
{
    SPR_init();

    //Setando a paleta de cores
    PAL_setPalette(PAL0, our_palette.data, DMA);
    PAL_setPalette(PAL2, our_sprite.palette->data, DMA);

    //Carregando o tileset na vram
    VDP_loadTileSet(&our_tileset, ind, DMA);

    //Colocando mapa tela
    level_map = MAP_create(&our_level_map, BG_B, TILE_ATTR_FULL(PAL0, false, false, false, ind));
    ind += our_tileset.numTile;
    MAP_scrollTo(level_map, current_camera_x, current_camera_y);

    player = SPR_addSprite(&our_sprite, F32_toInt(player_x), F32_toInt(player_y), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));


    //VDP_drawText("Hello Sega!!", 10,13);
    while(1)
    {
        
        handleInput();
        camera();


        //For versions prior to SGDK 1.60 use VDP_waitVSync instead.
        SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}


//Static para evitar problemas de escopo
static void handleInput()
{
    //Funcao que le os inputs do controle
    u16 value = JOY_readJoypad(JOY_1);
    //Cada bit significa um botao do controle
    //Ex: Direita 000100...
    //Esquerda: 010000...
    //Direta e esqueda: 010100...
    //Por isso que utilizamos AND bit a bit para verificar qual botao apertou
    
    //Movendo
    //Basicao soh para estudar
    if (value & BUTTON_RIGHT)
    {
         player_x += player_vel;
         player_anim = WALKING;
         SPR_setHFlip(player, TRUE);
    }
    else if (value & BUTTON_LEFT)
    {
        player_x -= player_vel;
        player_anim = WALKING;
        SPR_setHFlip(player, FALSE);
    }
    if (value & BUTTON_UP)
    {
         player_y -= player_vel;
         player_anim = WALKING;
    }
    else if (value & BUTTON_DOWN)
    {
        player_y += player_vel;
        player_anim = WALKING;
    }

    if (!(value & BUTTON_DIR))
    {
        player_anim = IDLE;
    }

    SPR_setAnim(player, player_anim);
    
    //Atualizando a posicao do player
    //Utiliza pixels e nao tiles
    
}

//Eh uma funcao meio burra, mas estava apenas seguindo o tutorial
static void camera()
{
    //Impedindo o player de sair da tela
    if (player_x < FIX32(0))
    {
        player_x = FIX32(0);
    }
    else if (player_x > FIX32(MAP_WIDTH - PLAYER_WIDTH))
    {
        player_x = FIX32(MAP_WIDTH - PLAYER_WIDTH);
    }
    //Teria que fazer a mesma coisa no eixo Y, mas fds

    //Cooredenadas do player em int no mapa
    int player_x_int = F32_toInt(player_x);
    int player_y_int = F32_toInt(player_y);

    //Coordenadas do player na tela
    int player_x_screen = player_x_int - current_camera_x;
    int player_y_screen = player_y_int - current_camera_y;

    //Nova posicao da camera
    int new_camera_x;
    int new_camera_y;

    //Vendo se o player passou dos limites definidos
    //entao a gente move a camera
    if (player_x_screen > HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES)
    {
        new_camera_x = player_x_int - HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES;
    }
    else if (player_x_screen < HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES)
    {
        new_camera_x = player_x_int - HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES;
    }
    else
    {
        new_camera_x = current_camera_x;
    }

    if (player_y_screen > HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES)
    {
        new_camera_y = player_y_int - HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES;
    }
    else if (player_y_screen < HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES)
    {
        new_camera_y = player_y_int - HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES;
    }
    else
    {
        new_camera_y = current_camera_y;
    }

    //Impedindo a camera de ir para fora do mapa
    if (new_camera_x < 0)
    {
        new_camera_x = 0;
    }
    else if (new_camera_x > MAP_WIDTH - HORIZONTAL_RESOLUTION)
    {
        new_camera_x = MAP_WIDTH - HORIZONTAL_RESOLUTION;
    }

    if (new_camera_y < 0)
    {
        new_camera_y = 0;
    }
    else if (new_camera_y > MAP_HEIGHT - VERTICAL_RESOLUTION)
    {
        new_camera_y = MAP_HEIGHT - VERTICAL_RESOLUTION;
    }

    if ((new_camera_x != current_camera_x) || (new_camera_y != current_camera_y))
    {
        current_camera_x = new_camera_x;
        current_camera_y = new_camera_y;

        MAP_scrollTo(level_map, new_camera_x, new_camera_y);
    }

    SPR_setPosition(player, F32_toInt(player_x) - new_camera_x, F32_toInt(player_y) - new_camera_y);
}