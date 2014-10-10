#include <string.h>
#include <kernel/tty.h>

unsigned char *logo =
"_/XXXXXXXXXXXXX____/XXX________/XXX_______/XXXXX_______/XXX________/XXX_       n" \
"_X/XXX/////////XXX_X///XXX____/XXX/______/XXX///XXX____X/XXX_____/XXX//__      n" \
" _X/XXX_______X/XXX___X///XXX/XXX/______/XXX/__X///XXX__X/XXX__/XXX//_____     n" \
"  _X/XXXXXXXXXXXXXX______X///XXX/_______/XXX______X//XXX_X/XXXXXX//XXX_____    n" \
"   _X/XXX/////////XXX_______X/XXX_______X/XXX_______X/XXX_X/XXX//_X//XXX____   n" \
"    _X/XXX_______X/XXX_______X/XXX_______X//XXX______/XXX__X/XXX____X//XXX___  n" \
"     _X/XXX_______X/XXX_______X/XXX________X///XXX__/XXX____X/XXX_____X//XXX__ n" \
"      _X/XXXXXXXXXXXXX/________X/XXX__________X///XXXXX/_____X/XXX______X//XXX_n" \
"       _X/////////////__________X///_____________X/////_______X///________X///_n";

void draw_logo(void)
{
    unsigned char c, *data;
    terminal_clear();
    data = logo;
    while ((c = *data++) != '\0' )
    {
        switch (c)
        {
            case '_':
                terminal_setcolor(0x08); // Dark Grey
                terminal_putchar('_');
                break;

            case '/':
                terminal_setcolor(0x0A); // Green
                terminal_putchar('/');
                break;

            case 'X':
                terminal_setcolor(0x0A); // Green
                terminal_putchar('\\');
                break;

            case ' ':
                terminal_putchar(' ');
                break;

            case 'n':
                terminal_putchar('\n');
                break;
        }
    }

    terminal_setcolor(0x07); // Light Grey
}
