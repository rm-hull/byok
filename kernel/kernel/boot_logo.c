#include <string.h>
#include <kernel/tty.h>

char *logo =
"__/XXXXXXXXXXXXX____/XXX________/XXX____/XXXXX________/XXX________/XXX_        n" \
" _X/XXX/////////XXX_X///XXX____/XXX/___/XXX///XXX_____X/XXX_____/XXX//__       n" \
"  _X/XXX_______X/XXX___X///XXX/XXX/___/XXX/__X///XXX___X/XXX__/XXX//_____      n" \
"   _X/XXXXXXXXXXXXXX______X///XXX/____/XXX______X//XXX__X/XXXXXX//XXX_____     n" \
"    _X/XXX/////////XXX_______X/XXX____X/XXX_______X/XXX__X/XXX//_X//XXX____    n" \
"     _X/XXX_______X/XXX_______X/XXX____X//XXX______/XXX___X/XXX____X//XXX___   n" \
"      _X/XXX_______X/XXX_______X/XXX_____X///XXX__/XXX_____X/XXX_____X//XXX__  n" \
"       _X/XXXXXXXXXXXXX/________X/XXX_______X///XXXXX/______X/XXX______X//XXX_ n" \
"        _X/////////////__________X///__________X/////________X///________X///__n";

void draw_logo(void)
{
    char c, *data;
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
