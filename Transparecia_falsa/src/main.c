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

u8 timer = 0;

static void handleInput();
static void transparency();

int main()
{
    u16 ind = TILE_USER_INDEX;
    SPR_init();


    PAL_setPalette(PAL0, kitchen_bg_image.palette->data, DMA);
    VDP_drawImageEx(BG_B, &kitchen_bg_image, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);


    PAL_setPalette(PAL2, spectre_sprite.palette->data, DMA);
    spectre = SPR_addSprite(&spectre_sprite, spectre_x, spectre_y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));

    PAL_setPalette(PAL3, sh_palette_sprite.palette->data, DMA);
    test_palette = SPR_addSprite(&sh_palette_sprite, test_palette_x, test_palette_y, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));



    while(1)
    {
        transparency();
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



static void transparency()
{
    if (timer == 0)
    {
        SPR_setVisibility(spectre, TRUE);
        timer++;
    }
    else 
    {
        SPR_setVisibility(spectre, FALSE);
        timer = 0;
    }
}