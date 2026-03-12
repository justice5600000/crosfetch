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
ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); // grab winsize and throw into struct
unsigned int row = size.ws_row; // the screens rows
unsigned int col = size.ws_col; // the screens columns
struct notcurses* nc = notcurses_init(&ncopt, stdout); // init notcurses screen (FULLSCREEN MODE)
ncinput in;
// std::cout will NOT work aswell as standard terminal io... (NOMORE calls to std::cout after this point!!!!)
notcurses_mice_enable(nc, NCMICE_ALL_EVENTS); // enable mouse support
//BOILERPLATE END
ncplane_options scropt { // constant structure representing the options for ncplane (menu) screen
 0, // y
 0,  // x
 row, // rows
 col  // cols
};
// create ncplae for the main screen area
struct ncplane* screen = ncplane_create(notcurses_stdplane(nc), &scropt);
timespec ts; // create timespec to update get every 100ms
ts.tv_sec = 0; // every second
ts.tv_nsec = 100000000; // every one 100 million nanoseconds (100ms)
// define image blit &bglayer
const char* bimgp = "./assets/crotch.png"; // path to background image
ncvisual* bimg = ncvisual_from_file(bimgp); // load ncvisual from the image located at bimgp
const ncvisual_options bopt { // bgoptions
 notcurses_stdplane(nc), // ncplane
 NCSCALE_STRETCH, // scale
 0, // y
 0, // x
 0, // begy
 0, // begx
 0, // leny
 0, // lenx
 NCBLIT_BRAILLE, // blit
 0 // flags
};
// blit bgimg (render to bg)
ncvisual_blit(nc, bimg, &bopt);
//define menu items, sections & options (IN THAT ORDER!!)
ncmenu_item nitems[] = { // double bracket'd array to pass multiple vars (new items)
 { "Folder", 'F' }, // description, shortcut
 { "File ", 'I'}
};
ncmenu_item oitems[] = { // open items
  { "Directory", 'D' }
};
ncmenu_item citems[] = { // crotchitems
  { "Exit crotch..", 'E'},
  { "Credits..", 'C'}
};
ncmenu_item aitems[] = { // agrigate items
 { "Select..", 'S' },
 { "Select multiple..", 'M' },
 { "Add to folder..", 'A' },
 { "Delete..", 'L' }
};
ncmenu_item vitems[] = {
 { "Preview image.. (monochrome)", 'P'},
 { "Set image as background.. (monochrome)", 'B'},
 { "View image.. (color)", 'V'}
};
ncmenu_section sections[] = { // sections of menu menubar
 { "CR0TCH", 2, citems },
 { "New..", 2, nitems }, // label, itemcout, items
 { "Open..", 1, oitems },
 { "Agrigate..", 4, aitems },
 { "View..", 3, vitems }
};
ncmenu_options mopts{ // menu options
  sections, // sections
  5 // sectioncount
};
// define menu(s)
struct ncmenu* menubar = ncmenu_create(screen, &mopts);
ncinput ids; // ncinput var to hold the shortcut of the selected menu






// ENDING CALLS
while(true) { // render loop
uint32_t c = notcurses_get_nblock(nc, &in); // grab any input & shove into the in struct (without blocking)
if(c == '`') break; // kill program if escape key detected 
if(c) { // if input is availible (optimized)
 ncmenu_offer_input(menubar, &in); // enable mouse input for menubar
 ncmenu_mouse_selected(menubar, &in, &ids); // put the id of selected menu item in ids
 uint32_t sel = ids.id; // extrect id of selected option
 char ascii = sel; // convert to char to compare
 // MENU OPTIONS
 if(ascii == 'E') break; // exit crotch
 if(ascii == 'D') { // open directory
  ncplane_options dcopt { // dirchooser opts
    row/2, // y
    col/2, // x
    1, // rows
    col - col/4 // cols
  };
  struct ncplane* dc = ncplane_create(screen, &dcopt);
  ncplane_putstr_yx(dc, 0, 0, "where dirchooser appears to be <--");
 }
}
notcurses_render(nc); // render all planes to screen
}
int sd = notcurses_stop(nc); // stop detect
return 0; // return successful program execution
}
