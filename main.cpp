#include <iostream>
#include <notcurses/notcurses.h>
#include <locale>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>

int main() {
//BOILERPLATE - initilize all the bs holding the program together
std::setlocale(LC_ALL, ""); // set2 a locale notcurses supports (UTF-8)
notcurses_options ncopt { // emtpy options struct 2use defaults

};
struct winsize size; // create winsize struct
ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
unsigned int row = size.ws_row;
unsigned int col = size.ws_col;
struct notcurses* nc = notcurses_init(&ncopt, stdout); // init notcurses screen (FULLSCREEN MODE)
ncinput in;
// std::cout will NOT work aswell as standard terminal io... (NOMORE calls to std::cout after this point!!!!)
notcurses_mice_enable(nc, NCMICE_ALL_EVENTS); // enable mouse support
ncplane_options scropt { // constant structure representing the options for ncplane (menu) screen
 0, // y
 0,  // x
 row, // rows
 col  // cols
};
struct ncplane* screen = ncplane_create(notcurses_stdplane(nc), &scropt);
//const char* butt = "THE TEXT IS WORKING!!!!"; -- REFRENCE, how to format strings to shove into functions
//define menu items, sections & options (IN THAT ORDER!!)
ncmenu_item nitems[] = { // double bracket'd array to pass multiple vars (new items)
 { "Folder |", 'F' }, // description, shortcut
 { "File |", 'I'}
};
ncmenu_item oitems[] = { // open items
  { "Directory |", 'D' }
};
ncmenu_section sections[] = {
 { "New..", 2, nitems }, // label, itemcout, items
 { "Open..", 1, oitems }
};
ncmenu_options mopts{
  sections, // sections
  2 // sectioncount
};
// define menu(s)
struct ncmenu* menubar = ncmenu_create(screen, &mopts);
const char* diddy = "fortnite";
ncinput ids;
ncplane_putstr_yx(screen, 10, 10, diddy);
notcurses_render(nc);
// ENDING CALLS
while(true) { // render loop
uint32_t c = notcurses_get(nc, NULL, &in); // grab any input & shove into the in struct
if(c == '`') break; // kill program if escape key detected
ncmenu_offer_input(menubar, &in); // enable mice for tempmen@the in array
ncmenu_mouse_selected(menubar, &in, &ids);
uint32_t sel = ids.id; // extrect id of selected option
char ascii = sel; // convert to char to compare
if(ascii == 'F') {
 break;
}
notcurses_render(nc); // render to the screen
}
int sd = notcurses_stop(nc); // stop detect
return 0; // return successful program execution
}
