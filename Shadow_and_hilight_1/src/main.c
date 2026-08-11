/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <resources.h>


#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION 224

#define PLAYER_WIDTH 64
#define PLAYER_HEIGHT 64

Sprite *test_palette;
s16 test_palette_x = 50;
s16 test_palette_y = 50;

Sprite *spectre;
s16 spectre_x = 100;
s16 spectre_y = 100;

static void handleInput();
//static void joyEvent(u16 joy, u16 changed, u16 state);


int main()
{
    u16 ind = TILE_USER_INDEX;
    SPR_init();
    //JOY_setEventHandler(joyEvent);


    PAL_setPalette(PAL0, kitchen_bg_image.palette->data, DMA);
    VDP_drawImageEx(BG_B, &kitchen_bg_image, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);

    //Tem que ser o pal3 para funcionar o hilight
    //PAL_setPalette(PAL3, spectre_sprite.palette->data, DMA);
    //spectre = SPR_addSprite(&spectre_sprite, spectre_x, spectre_y, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));

    //Tudo que tem prioridade true nao eh afetado pelo hilight
    //Pode aplicar a prioridade em tiles especificos do background pra fazer areas mais iluminadas
    //Pelo que eu entendi se tu ativa o hilightshadow, a 15 cor da paleta nao eh afetada
    //Perceba que uma parte da calca do axel ta bem mais clara que o resto das outras cores
    PAL_setPalette(PAL1, spectre_sprite.palette->data, DMA);
    spectre = SPR_addSprite(&spectre_sprite, spectre_x, spectre_y, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));

    
    PAL_setPalette(PAL2, sh_palette_sprite.palette->data, DMA);
    test_palette = SPR_addSprite(&sh_palette_sprite, test_palette_x, test_palette_y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));



    //Hilight e Shadow nao afeta quem esta na camada de prioridade
    //A transparencia desse negocio impede de desenhar os sprites que estao atras
    //para testar mude a ordem de desenho das sprites
    //Pelo que eu entendi soh funciona com cores cinzas
    VDP_setHilightShadow(TRUE);


    while(1)
    {
        
        handleInput();


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

    //Basicao soh para estudar
    if (value & BUTTON_RIGHT)
    {
        test_palette_x += 1;
    }
    else if (value & BUTTON_LEFT)
    {
        test_palette_x -= 1;
    }

    if (value & BUTTON_DOWN)
    {
        test_palette_y += 1;
    }   
    else if (value & BUTTON_UP)
    {
        test_palette_y -= 1;
    }

    if (value & BUTTON_A)
    {
        spectre_x -= 1;
    }
    else if (value & BUTTON_C)
    {
        spectre_x += 1;
    }

    if (value & BUTTON_Y)
    {
        spectre_y -= 1;
    }
    else if (value & BUTTON_B)
    {
        spectre_y += 1;
    }

    SPR_setPosition(spectre, spectre_x, spectre_y);
    SPR_setPosition(test_palette, test_palette_x, test_palette_y);
}


/*
//Funcao dos eventos unicos (ataque, jump etc)
//Eventos no estilo apertou uma vez e precisa soltar e apertar de novo
static void joyEvent(u16 joy, u16 changed, u16 state)
{
    
    if ((changed & state & BUTTON_C) && dpad_down)
    {
        fall_through = TRUE;
    }
    else if ((changed & state & BUTTON_C) && !jumping && player_y_vel <= 0) 
    {
        player_y_vel -= jumping_vel;
        jumping = TRUE;
    }
}*/