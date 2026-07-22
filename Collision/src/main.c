/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <resources.h>

//Quantidade de colunas por linha
const u8 LENGTH_MAP = 20;

#define FREE_TILE 0
#define SOLID_TILE 1
char info[10];

const u8 LEVEL_COL[280] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

u16 ind = TILE_USER_INDEX;
Map *level_map;

Sprite *player;
fix32 player_x = FIX32(100);
fix32 player_y = FIX32(100);
fix32 player_vel = FIX32(1.5);

bool player_move_left = FALSE;
bool player_move_right = FALSE;
bool player_move_up = FALSE;
bool player_move_down = FALSE;

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
#define MAP_WIDTH 320
#define MAP_HEIGHT 224

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

#define PLAYER_COLBOX_LEFT 10
#define PLAYER_COLBOX_RIGHT 23
#define PLAYER_COLBOX_TOP 18
#define PLAYER_COLBOX_BOTTOM 31

static void handleInput();
static void camera();
static void collision();



int main()
{
    SPR_init();

    //Setando a paleta de cores
    PAL_setPalette(PAL0, background_pal0.data, DMA);
    PAL_setPalette(PAL2, player_img.palette->data, DMA);

    //Carregando o tileset na vram
    VDP_loadTileSet(&background_tileset, ind, DMA);

    //Aparentemente o SGDK ja identificou que a cor rosa eh a nossa transparencia
    //Isso so acontece no BG_A
    level_map = MAP_create(&background_map, BG_B, TILE_ATTR_FULL(PAL0, false, false, false, ind));
    ind += background_tileset.numTile;
    MAP_scrollTo(level_map, current_camera_x, current_camera_y);


    player = SPR_addSprite(&player_img, F32_toInt(player_x), F32_toInt(player_y), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));


    //VDP_drawText("Hello Sega!!", 10,13);
    while(1)
    {
        
        handleInput();
        collision();
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
         //player_x += player_vel;
         //SPR_setHFlip(player, FALSE);
         player_move_right = TRUE;
         player_move_left = FALSE;
    }
    else if (value & BUTTON_LEFT)
    {
        //player_x -= player_vel;
        //SPR_setHFlip(player, TRUE);
        player_move_right = FALSE;
        player_move_left = TRUE;
    }
    else
    {
        player_move_right = FALSE;
        player_move_left = FALSE;
    }

    if (value & BUTTON_UP)
    {
        //player_y -= player_vel;
        player_move_up = TRUE;
        player_move_down = FALSE;
    }
    else if (value & BUTTON_DOWN)
    {
        //player_y += player_vel;
        player_move_up = FALSE;
        player_move_down = TRUE;
    }
    else
    {
        player_move_up = FALSE;
        player_move_down = FALSE;
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


//Essa colisao eh bem besta
//mas ai entra a duvida 
//oq eh melhor para o mega drive? um codigo besta ou um mais elegante? o mega drive tem capacidade para um pixel perfect?
//Essa eh do tipo entra e depois empurra
static void collision()
{
    //Pegando as coordenadas da nossa colbox
    s16 player_left = 0;
    s16 player_right = 0;
    s16 player_top = F32_toInt(player_y) + PLAYER_COLBOX_TOP;
    s16 player_bottom = F32_toInt(player_y) + PLAYER_COLBOX_BOTTOM;

    //Dividindo por 16 (tamanho do bloco)
    s16 tile_left = 0;
    s16 tile_right = 0;
    s16 tile_top = player_top >> 4;
    s16 tile_bottom = player_bottom >> 4;

    //Index na array de colisao
    u16 index_topleft = 0;
    u16 index_topright = 0; 
    u16 index_bottomleft = 0;
    u16 index_bottomright = 0;

    u8 tile_type_topleft = 0;
    u8 tile_type_topright = 0;
    u8 tile_type_bottomleft = 0;
    u8 tile_type_bottomright = 0;

    s16 blocked_left = 0;
    s16 blocked_right = 0;
    s16 blocked_up = 0;
    s16 blocked_down = 0;

    //Verificando a colisao
    if (player_move_left)
    {
        player_x -= player_vel;
        
        player_left = F32_toInt(player_x) + PLAYER_COLBOX_LEFT;
        tile_left = player_left >> 4;
        index_topleft = tile_left + (tile_top * LENGTH_MAP);
        tile_type_topleft = LEVEL_COL[index_topleft];

        index_bottomleft = tile_left + (tile_bottom * LENGTH_MAP);
        tile_type_bottomleft = LEVEL_COL[index_bottomleft];


        if (tile_type_topleft == SOLID_TILE || tile_type_bottomleft == SOLID_TILE)
        {
            blocked_left = (tile_left << 4) + 16 - PLAYER_COLBOX_LEFT;
            player_x = FIX32(blocked_left);
            //player_x += FIX32(0.1);
        }
    }
    else if (player_move_right)
    {
        player_x += player_vel;

        player_right = F32_toInt(player_x) + PLAYER_COLBOX_RIGHT;
        tile_right = player_right >> 4;
        index_topright = tile_right + (tile_top * LENGTH_MAP);
        tile_type_topright = LEVEL_COL[index_topright];

        index_bottomright = tile_right + (tile_bottom * LENGTH_MAP);
        tile_type_bottomright = LEVEL_COL[index_bottomright];

        if (tile_type_topright == SOLID_TILE || tile_type_bottomright == SOLID_TILE)
        {
            blocked_right = (tile_right << 4) - PLAYER_COLBOX_RIGHT;
            player_x = FIX32(blocked_right);
            player_x -= FIX32(0.1);
        }
    }

    if (player_move_up)
    {
        player_y -= player_vel;

        player_top = F32_toInt(player_y) + PLAYER_COLBOX_TOP;
        tile_top = player_top >> 4;

        player_left = F32_toInt(player_x) + PLAYER_COLBOX_LEFT;
        tile_left = player_left >> 4;
        index_topleft = tile_left + (tile_top * LENGTH_MAP);
        tile_type_topleft = LEVEL_COL[index_topleft];

        player_right = F32_toInt(player_x) + PLAYER_COLBOX_RIGHT;
        tile_right = player_right >> 4;
        index_topright = tile_right + (tile_top * LENGTH_MAP);
        tile_type_topright = LEVEL_COL[index_topright];

        if (tile_type_topleft == SOLID_TILE || tile_type_topright == SOLID_TILE)
        {
            blocked_up = (tile_top << 4) + 16 - PLAYER_COLBOX_TOP;
            player_y = FIX32(blocked_up);
        }
    }

    if (player_move_down)
    {
        player_y += player_vel;

        player_bottom = F32_toInt(player_y) + PLAYER_COLBOX_BOTTOM;
        tile_bottom = player_bottom >> 4;

        player_left = F32_toInt(player_x) + PLAYER_COLBOX_LEFT;
        tile_left = player_left >> 4;
        index_bottomleft = tile_left + (tile_bottom * LENGTH_MAP);
        tile_type_bottomleft = LEVEL_COL[index_bottomleft];

        player_right = F32_toInt(player_x) + PLAYER_COLBOX_RIGHT;
        tile_right = player_right >> 4;
        index_bottomright = tile_right + (tile_bottom * LENGTH_MAP);
        tile_type_bottomright = LEVEL_COL[index_bottomright];

        if (tile_type_bottomleft == SOLID_TILE || tile_type_bottomright == SOLID_TILE)
        {
            blocked_down = (tile_bottom << 4) - PLAYER_COLBOX_BOTTOM;
            player_y = FIX32(blocked_down);
            player_y -= FIX32(0.1);
        }
    }

    //sprintf(info, "%10i", F32_toInt(player_x));
    sprintf(info, "%10i", tile_type_topleft);
    VDP_drawTextBG(BG_A, info, 28, 5);
}