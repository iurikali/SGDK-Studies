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

//O cara fez global para facilitar para os betinhas
Sprite *player;
int player_x = 100;
int player_y = 50;
int player_anim = IDLE;

int attack_timer = 0; 
int attack_duration = 56;

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
         player_x += 1;
         player_anim = WALKING;
         SPR_setHFlip(player, TRUE);
    }
    else if (value & BUTTON_LEFT)
    {
        player_x -= 1;
        player_anim = WALKING;
        SPR_setHFlip(player, FALSE);
    }
    if (value & BUTTON_UP)
    {
         player_y -= 1;
         player_anim = WALKING;
    }
    else if (value & BUTTON_DOWN)
    {
        player_y += 1;
        player_anim = WALKING;
    }

    if (!(value & BUTTON_DIR))
    {
        player_anim = IDLE;
    }

    SPR_setAnim(player, player_anim);
    
    //Atualizando a posicao do player
    //Utiliza pixels e nao tiles
    SPR_setPosition(player, player_x, player_y);
}

//Funcao dos eventos unicos (ataque, jump etc)
//Eventos no estilo apertou uma vez e precisa soltar e apertar de novo
static void joyEvent(u16 joy, u16 changed, u16 state)
{
    if (changed & state & BUTTON_A && attack_timer == 0)
    {
        SPR_setAnim(player, ATTACK);
        attack_timer += 1;
    }
}

static void attack_function()
{
    //Decidindo se o player vai andar ou atacar
    //Tudo isso aqui pode ser substituido por uma maquina de estados
    if (attack_timer == 0)
    {
        handleInput();
    }
    else if (attack_timer > 0 && attack_timer < attack_duration)
        attack_timer += 1;
    else
        attack_timer = 0;
}


int main()
{
   
    //Background
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, bg1.palette->data, DMA);
    VDP_drawImageEx(BG_B, &bg1, 
    TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind),
    //Aqui o x e y usam tiles (8pixels)
    0, 0, FALSE, TRUE);
    //Mudando o index
    ind += bg1.tileset->numTile;
    

    //Foreground
    PAL_setPalette(PAL1, fg1.palette->data, DMA);
    VDP_drawImageEx(BG_A, &fg1, 
    TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    ind += fg1.tileset->numTile;

    //Scrolling background
    int hscroll_offset = 0;
    int hscroll_offset_fore = 0;
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    

    //Sprites
    SPR_init();
    PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
    //Aqui x e y usam pixels
    player = SPR_addSprite(&our_sprite, player_x, player_y, TILE_ATTR(PAL2, FALSE, FALSE, TRUE));
    
    SPR_setAnim(player, player_anim);
    //Sobre a funcao de cima
    //O segundo parametro eh a linha da nossa spritesheet
    //Aparentemente o SGDK ignora tiles totalmente transparentes
    //Pelo que eu entendi tu pode ter no maximo 20 sprites por scanline 
    //E no maximo 80 sprites na tela inteira
    //O tamanho maximo de um sprite eh 32x32 pixels, se passar disso o SGDK cuida 
    //para unir tudo em um mega sprite
    //SGDK cuida de otimizar os espacos vazios das sprites
    

    //Eventos do controle (attk, jump etc)
    JOY_setEventHandler(joyEvent);
    

    while(1)
    {
        //Scroll back
        /*VDP_setHorizontalScroll(BG_B, hscroll_offset);
        hscroll_offset--;
        //Scroll fore
        VDP_setHorizontalScroll(BG_A, hscroll_offset_fore);
        hscroll_offset_fore-= 2;*/
        



        attack_function();

        //Atualizando todas as sprites
        SPR_update();
        //Limpando a tela
        SYS_doVBlankProcess();




    }
    return (0);
}
