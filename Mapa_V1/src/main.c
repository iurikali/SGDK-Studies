/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <resources.h>


u16 ind = TILE_USER_INDEX;
Map *level_1_map;

int scroll_x = 0;

int main()
{
    //VRAM tem 64KB
    //ROM tem 4MB
    //Na rom fica armazena uma matriz com o mapa, acho que eh uma matriz do tamanho da imagem e 
    //4bits cada elemento da matriz (até 16 tiles)
    //So oq esta sendo exibido na tela vai para a VRAM
    //Quando comeca o scroll, ele lê da ROM e transfere para a VRAM a nova parte do mapa
    //Apertar V no blastem para ver a VRAM


    //Carrendo o tileset na vram
    VDP_loadTileSet(&our_tileset, ind, DMA);

    //Colocando mapa tela
    level_1_map = MAP_create(&our_level_map, BG_B, TILE_ATTR_FULL(PAL0, false, false, false, ind));

    //Setando a paleta de cores
    PAL_setPalette(PAL0, our_palette.data, DMA);

    MAP_scrollTo(level_1_map, scroll_x, 768);

    //VDP_drawText("Hello Sega!!", 10,13);
    while(1)
    {
        scroll_x += 1;

        MAP_scrollTo(level_1_map, scroll_x, 768);   
        //For versions prior to SGDK 1.60 use VDP_waitVSync instead.
        SYS_doVBlankProcess();
    }
    return (0);
}
