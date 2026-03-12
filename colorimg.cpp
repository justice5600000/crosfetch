#include <notcurses/notcurses.h>
#include <iostream>
#include <notcurses/notcurses.h>
#include <locale>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>

int main() {
// setup notcurses
std::setlocale(LC_ALL, "");
notcurses_options ncopt{};
struct winsize size;
ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
unsigned int row = size.ws_row;
unsigned int col = size.ws_col;
struct notcurses* nc = notcurses_init(&ncopt, stdout);
// setup imgloader
const char* imgp = "./assets/d.png";
ncvisual* img = ncvisual_from_file(imgp);
const ncvisual_options iopt {
 notcurses_stdplane(nc),
 NCSCALE_STRETCH,
 0,
 0,
 0,
 0,
 0,
 0,
 NCBLIT_DEFAULT,
 0
};
// blit img
ncvisual_blit(nc, img, &iopt);
// define menu struct(s)
ncmenu_item citems[] = {
 { "Exit crotch..", 'E' }
};
ncmenu_section sections[] = {
 { "CR0TCH", 1, citems }
};
ncmenu_options mopts{
 sections,
 1
};
// define menu
ncplane_options mpopts{
 0,
 0,
 row,
 col
};
struct ncplane* menup = ncplane_create(notcurses_stdplane(nc), &mpopts);
struct ncmenu* menubar = ncmenu_create(menup, &mopts);
ncmenu_destroy(menubar);
// render loop
while(true) {
notcurses_render(nc);
}

}
