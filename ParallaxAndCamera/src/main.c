/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <resources.h>


u16 ind = TILE_USER_INDEX;
Map *level_map;

Sprite *player;
fix32 player_x = FIX32(0);
fix32 player_y = FIX32(820);
fix32 player_vel = FIX32(1.5);

//Variaveis da camera
s16 current_camera_x = 0;
s16 current_camera_y = 0;

#define HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES 150
#define HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES 151
#define HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES 115
#define HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES 116

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION 224

//Tem que ser multiplo de 8
#define MAP_WIDTH 6176
#define MAP_HEIGHT 352

#define PLAYER_WIDTH 48
#define PLAYER_HEIGHT 40

static void handleInput();
static void camera();


int main()
{
    SPR_init();

    //Setando a paleta de cores
    PAL_setPalette(PAL0, background_image.palette->data, DMA);
    PAL_setPalette(PAL1, shinobi_foreground_pal.data, DMA);
    PAL_setPalette(PAL2, dog_img.palette->data, DMA);

    //Carregando o tileset na vram
    VDP_loadTileSet(&shinobi_foreground_tileset, ind, DMA);

    //Aparentemente o SGDK ja identificou que a cor rosa eh a nossa transparencia
    //Isso so acontece no BG_A
    level_map = MAP_create(&shinobi_foreground_map, BG_A, TILE_ATTR_FULL(PAL1, false, false, false, ind));
    ind += shinobi_foreground_tileset.numTile;
    MAP_scrollTo(level_map, current_camera_x, current_camera_y);

    VDP_drawImageEx(BG_B, &background_image, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

    player = SPR_addSprite(&dog_img, F32_toInt(player_x), F32_toInt(player_y), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));


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
         SPR_setHFlip(player, FALSE);
    }
    else if (value & BUTTON_LEFT)
    {
        player_x -= player_vel;
        SPR_setHFlip(player, TRUE);
    }
    if (value & BUTTON_UP)
    {
         player_y -= player_vel;
    }
    else if (value & BUTTON_DOWN)
    {
        player_y += player_vel;
    }
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
    if (player_y < FIX32(0))
    {
        player_y = FIX32(0);
    }
    else if (player_y > FIX32(MAP_HEIGHT - PLAYER_HEIGHT))
    {
        player_y = FIX32(MAP_HEIGHT - PLAYER_HEIGHT);
    }
    

    //Cooredenadas do player em int no mapa
    s16 player_x_int = F32_toInt(player_x);
    s16 player_y_int = F32_toInt(player_y);

    //Coordenadas do player na tela
    s16 player_x_screen = player_x_int - current_camera_x;
    s16 player_y_screen = player_y_int - current_camera_y;

    //Nova posicao da camera
    s16 new_camera_x;
    s16 new_camera_y;

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

    s16 b_hscroll = 0;
    s16 b_vscroll = 0;

    if ((new_camera_x != current_camera_x) || (new_camera_y != current_camera_y))
    {
        current_camera_x = new_camera_x;
        current_camera_y = new_camera_y;
        //Mega drive tem problemas com multiplicacao
        //por isso o 0-
        //Shift para direita
        b_hscroll = (0 - new_camera_x) >> 3;
        b_vscroll = new_camera_y >> 2;
        
        //Impedindo o problema no eixo vertical
        if (b_vscroll < 0) b_hscroll = 0;
        else if (b_vscroll > 32) b_hscroll = 32;        

        MAP_scrollTo(level_map, new_camera_x, new_camera_y);

        VDP_setHorizontalScroll(BG_B, b_hscroll);
        VDP_setVerticalScroll(BG_B, b_vscroll);
    }


    SPR_setPosition(player, F32_toInt(player_x) - new_camera_x, F32_toInt(player_y) - new_camera_y);
}