#include <iostream>
#include <notcurses/notcurses.h>
#include <locale>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "widgets.h"
#include <windows.h>
#include <shlwapi.h>


std::vector<std::string> getdir(std::string in) {
 std::vector<std::string> list;
 for(const auto & entry : std::filesystem::directory_iterator(in)) { // megabrain c++ logic i dont understand (https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c)
  list.push_back(entry.path().string());
 }

 return list;
}

int main() {
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
DWORD mode;
GetConsoleMode(hIn, &mode);
mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
SetConsoleMode(hIn, mode);
//BOILERPLATE - initilize all the bs holding the program together
std::setlocale(LC_ALL, ""); // set2 a locale notcurses supports (UTF-8)
notcurses_options ncopt { // emtpy options struct 2use defaults

};
struct notcurses* nc = notcurses_init(&ncopt, stdout); // init notcurses screen (FULLSCREEN MODE)
unsigned int row, col;
notcurses_stddim_yx(nc, &row, &col);
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
ncplane_options bgopt {}; // initilize background (image) options
bgopt.y = 0; // y
bgopt.x = 0; // x
bgopt.rows = row; // height
bgopt.cols = col; // length
ncplane* bgplane = ncplane_create(notcurses_stdplane(nc), &bgopt); // create plane using the standard plane as its parent, pass a pointer to the options
std::string bimgp = "./assets/crotch.png"; // path to background image
ncvisual* bimg = ncvisual_from_file(bimgp.c_str()); // load ncvisual from the image located at bimgp
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
ncinput ids; // ncinput var to hold the shortcut of the selected menu
// define FILE selector(s)
std::string root = "/";
int rootlen = root.length();
std::vector<std::string> files = getdir(root);
ncplane_options mansel {}; // manual selector (yay!)
mansel.y = row / 4; // y ( set to one fourth of the screens height
mansel.x = col / 4; // x ^
mansel.rows = 25; // length
mansel.cols = col / 2; // width set to exactly half of screen
// define choose menu
int coc = 2 + 1; // choose option counter (plus buffer for title)
ncplane_options csel {};
csel.y = row / 4 + 10;
csel.x = col / 4 - 5;
csel.rows = coc;
csel.cols = col / 2 + 10;
// ENDING CALLS
int m1o = 0; // menu one open
// define subplanes
ncplane* idk = nullptr; // make default value of fileselector null (RENAME PLS)
ncplane* cs = nullptr; // choose sel
ncplane_move_bottom(bgplane); // move the background to the bottom of the pile
ncplane_move_top(screen); // move the screen to the top of the pile
char* d; //!!!!!!! unused(?)
// fileselector vars
std::string cfile;
std::string temp;
int inputbuf = 0;
int selected = 0;
// render loop
while(true) {
uint32_t c = notcurses_get_nblock(nc, &in); // grab any input & shove into the in struct (without blocking)
if(c == '`') break; // kill program if escape key detected 
if(c) {
 uint32_t sel = ids.id; // extrect id of selected option
 char ascii = sel; // convert to char to compare
 // MENU OPTIONS
 if(ascii == 'E') break; // exit crotch
 if(ascii == 'B') {
  if(bimg) {
  ncvisual_destroy(bimg);
  bimg = nullptr;
  }
 }
 if(c == 'e') {
  if(!idk) {
  m1o = gen_filsel(m1o, idk, screen, mansel);
  }
}
 if(c == 'q') {
  if(idk && !cs) {
  m1o = gen_filsel(m1o, idk, screen, mansel);
  }
  if(cs) {
   ncplane_destroy(cs);
   cs = nullptr;
  }
 }
}
if(idk) { // if the file selector is open
 cfile = update_filsel(idk, files, mansel.rows, selected, root, rootlen); // menu drawing logic
 if(c == 'w') {
   if(selected != 0) selected--;
 }
 if(c == 's') {
   selected++;
 }
 if(c == 'm') {
  std::filesystem::path path(cfile);
  std::error_code ec;
  if(std::filesystem::is_directory(path, ec)) { // if nonfolder selected
   root = cfile;
   rootlen = cfile.length();
    files = getdir(cfile);
    selected = 0;
  } else {
   if(!cs) {
    cs = ncplane_create(screen, &csel);
    ncplane_move_top(cs);
    ncplane_set_base(cs, " ", 0, 0);    
    ncplane_putstr_yx(cs, 0, 0, "Choose..");
    ncplane_putstr_yx(cs, 1, 0, "Say");
    ncplane_putstr_yx(cs, 2, 0, "Wahalli");
   }
  }
 }
 
}
notcurses_render(nc);
ncvisual_blit(nc, bimg, &bopt);
}
int sd = notcurses_stop(nc); // stop detect
return 0; // return successful program execution
}
