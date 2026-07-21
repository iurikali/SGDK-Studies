/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <resources.h>

#define WALKING 2

int main()
{
   
    //Background
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, bg1.palette->data, DMA);
    VDP_drawImageEx(BG_B, &bg1, 
    TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind),
    //Aqui o x e y usam tiles (8pixels)
    0, -4, FALSE, TRUE);
    //Mudando o index
    ind += bg1.tileset->numTile;
    

    //Foreground
    PAL_setPalette(PAL1, fg1.palette->data, DMA);
    VDP_drawImageEx(BG_A, &fg1, 
    TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, -4, FALSE, TRUE);
    ind += fg1.tileset->numTile;

    //Scrolling background
    int hscroll_offset = 0;
    int hscroll_offset_fore = 0;
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    

    //Sprites
    Sprite *player;
    SPR_init();
    PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
    //Aqui x e y usam pixels
    player = SPR_addSprite(&our_sprite, 100, 50, TILE_ATTR(PAL2, FALSE, FALSE, TRUE));
    //O segundo parametro eh a linha da nossa spritesheet
    //Aparentemente o SGDK ignora tiles totalmente transparentes
    SPR_setAnim(player, WALKING);
    //Pelo que eu entendi tu pode ter no maximo 20 sprites por scanline 
    //E no maximo 80 sprites na tela inteira
    //O tamanho maximo de um sprite eh 32x32 pixels, se passar disso o SGDK cuida 
    //para unir tudo em um mega sprite
    //SGDK cuida de otimizar os espacos vazios das sprites

    while(1)
    {
        //Scroll back
        VDP_setHorizontalScroll(BG_B, hscroll_offset);
        hscroll_offset--;
        //Scroll fore
        VDP_setHorizontalScroll(BG_A, hscroll_offset_fore);
        hscroll_offset_fore-= 2;
        
        SPR_update();

        //For versions prior to SGDK 1.60 use VDP_waitVSync instead.
        SYS_doVBlankProcess();




    }
    return (0);
}
