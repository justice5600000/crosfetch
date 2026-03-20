#include <iostream>
#include <notcurses/notcurses.h>
#include <locale>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>

std::vector<std::string> getrootlist() {
 std::vector<std::string> list;
 std::string root = "/";
 for(const auto & entry : std::filesystem::directory_iterator(root)) { // megabrain c++ logic i dont understand (https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c)
  list.push_back(entry.path());
 }

 return list;
}

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
struct ncplane* screen = notcurses_stdplane(nc);
timespec ts; // create timespec to update get every 100ms
ts.tv_sec = 0; // every second
ts.tv_nsec = 100000000; // every one 100 million nanoseconds (100ms)
// define image blit &bglayer
ncplane_options bgopt {};
bgopt.y = 0;
bgopt.x = 0;
bgopt.rows = row;
bgopt.cols = col;
ncplane* bgplane = ncplane_create(notcurses_stdplane(nc), &bgopt);
const char* bimgp = "./assets/crotch.png"; // path to background image
ncvisual* bimg = ncvisual_from_file(bimgp); // load ncvisual from the image located at bimgp
const ncvisual_options bopt { // bgoptions
 bgplane, // ncplane
 NCSCALE_STRETCH, // scale
 0, // y
 0, // x
 0, // begy
 0, // begx
 0, // leny
 0, // lenx
 NCBLIT_2x1, // blit
 0 // flags
};
// blit bgimg (render to bg)
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
  { "Close widgets..", 'C'}
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
ncinput ids; // ncinput var to hold the shortcut of the selected menu
// define selector(s)
std::vector<std::string> files = getrootlist();
ncselector_item itemarray[files.size()]; // create one item struct for every entry in files
//array[item].variuble = content // how to access theese items
int c = 0;
for(int i = 0; i < files.size(); i++) { // INITILIZE ITEMS
 const char* idk = files[i].c_str();
 itemarray[i].option = idk; // make the item at the current number's filename the file at the current number
 itemarray[i].desc = "file                                                    ";
 c++;
}
// nullpoint the last options
itemarray[c].option = nullptr;
itemarray[c].desc = nullptr;
// define and shoot out stupidsel to fix color
//ncselector_options stopt { "THIS", "FIXESTHEBACKGROUND", "SOMEHOW", &itemarray[1], 1, 10, 1, 1, 1, 1, 1, 0 };
//struct ncselector* filesec = ncselector_create(screen, &stopt);
// main selectors
ncselector_options fpopt {};
fpopt.title = "Open file/";
fpopt.secondary = "el secondary";
fpopt.footer = "the feeter";
fpopt.items = itemarray;
fpopt.defidx = 0;
ncplane_options spopt {}; // selectorplane opts
spopt.rows = row / 2;
spopt.cols = col / 2;
spopt.y = (row - spopt.rows) / 2;
spopt.x = (col - spopt.cols) / 2;
//ncplane* selectorplane = ncplane_create(screen, &spopt);
//ncplane_set_base(selectorplane, "2x", 0, 0x202020); 
//ncselector* ssec  = ncselector_create(selectorplane, &fpopt);

// tracking int's's's'
int selectoropen = 0;
// ENDING CALLS






struct ncmenu* menubar = ncmenu_create(screen, &mopts);
ncplane_move_bottom(bgplane);
ncplane_move_top(screen);
// predefine structs
ncplane* selectorplane = nullptr;
ncselector* ssec = nullptr;
// render loop
while(true) {
uint32_t c = notcurses_get_nblock(nc, &in); // grab any input & shove into the in struct (without blocking)
if(c == '`') break; // kill program if escape key detected 
if(c) { // if input is availible (optimized)
 ncmenu_offer_input(menubar, &in); // enable mouse input for menubar
 ncmenu_mouse_selected(menubar, &in, &ids); // put the id of selected menu item in ids
 uint32_t sel = ids.id; // extrect id of selected option
 char ascii = sel; // convert to char to compare
 // MENU OPTIONS
 if(ascii == 'E') break; // exit crotch
 if(ascii == 'D' && (in.evtype & NCKEY_BUTTON1) != 0) { // open directory
    selectorplane = ncplane_create(screen, &spopt);
    ncplane_set_base(selectorplane, " ", 0, 0x202020);
    ssec  = ncselector_create(selectorplane, &fpopt);
    ncplane_move_top(selectorplane);
 }
 if(ascii == 'C' && ssec != nullptr) {
   ncplane_move_bottom(selectorplane);
 }
}
ncvisual_blit(nc, bimg, &bopt);
notcurses_render(nc); // render all planes to screen
}
int sd = notcurses_stop(nc); // stop detect
return 0; // return successful program execution
}
