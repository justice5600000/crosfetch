
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
mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
mode |= ENABLE_EXTENDED_FLAGS;
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
 NCBLIT_BRAILLE, // blit
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
csel.y = row / 2;
csel.x = col / 4 - 5;
csel.rows = 4;
csel.cols = col / 2 + 10;
// ENDING CALLS
int m1o = 0; // menu one open
// define subplanes
ncplane* idk = nullptr; // make default value of fileselector null (RENAME PLS)
ncplane* cs = nullptr; // choose sel
ncplane* tut = nullptr; // blabber screen
ncplane* rp = nullptr; // reading path
ncplane_move_bottom(bgplane); // move the background to the bottom of the pile
ncplane_move_top(screen); // move the screen to the top of the pile
char* d; //!!!!!!! unused(?)
int menu = 70; // huge number to store intro
// fileselector vars
std::string cfile;
std::string prev;
std::string temp;
int inputbuf = 0;
int selected = 0;
int sel2 = 0;
uint32_t c = 0;
uint32_t tmp = 0;
// render loop
while(true) {
tmp = notcurses_get(nc, &ts, &in);
if(tmp && in.evtype == NCTYPE_PRESS) {
    c = 0;
} else {
    c = tmp;
}
if(c == '`') break; // kill program if escape key detected 
 char ascii = (char)c; // convert to char to compare
 // MENU OPTIONS
 if(c == 'e') {
  if(!idk) {
  menu = 1;
  m1o = gen_filsel(m1o, idk, screen, mansel);
  }
}
 if(c == 'q' && !rp && !cs) {
  if(idk && menu != 2) {
  menu = 0;
  m1o = gen_filsel(m1o, idk, screen, mansel); // close the file selector menu
  root = "/";
  rootlen = root.length();
  files = getdir(root);
  selected = 0;
  }
 }

if(menu == 70) { // tut men
 ncplane_options tutopt {};
 tutopt.y = row / 8;
 tutopt.x = col / 8;
 tutopt.rows = row / 4 * 3;
 tutopt.cols = col / 4 * 3;
 int cenner = tutopt.cols / 4;
 if(!tut) tut = ncplane_create(screen, &tutopt);
 ncplane_putstr_yx(tut, 0, cenner, "--- CR0TCH - WINDOWS BRANCH ---");
 ncplane_putstr_yx(tut, 2, cenner, "Welcome to the beta of crotch for windows,");
 ncplane_putstr_yx(tut, 3, cenner, "codenamed croswin");
 ncplane_putstr_yx(tut, 4, cenner, "----------- CONTROLS ----------");
 ncplane_putstr_yx(tut, 5, cenner, "c - close this window");
 ncplane_putstr_yx(tut, 6, cenner, "e - open file selector");
 ncplane_putstr_yx(tut, 7, cenner, "m - select");
 ncplane_putstr_yx(tut, 8, cenner, "b - return to parent directory");
 ncplane_putstr_yx(tut, 9, cenner, "w/s - move up/down");
 ncplane_putstr_yx(tut, 10, cenner, "~/` - exit app");
 ncplane_putstr_yx(tut, 11, cenner, "--------- KNOWN BUGS ----------");
 ncplane_putstr_yx(tut, 12, cenner, "freezes causing input to hang - quit app and reopen");
 ncplane_set_base(tut, " ", 0, 0); // fill plane with spaces
 if(c == 'c') {
  ncplane_destroy(tut);
  tut = nullptr;
  menu = 0;
 }
} 
// Read.. | submenu
if(menu == 3) { // reading text menu (TEXT SCROLLING HAS TO COME LATER)
 ncplane_options redopt {}; // reading opts
 redopt.y = mansel.y;
 redopt.x = mansel.x;
 redopt.rows = mansel.rows;
 redopt.cols = mansel.cols; // <-- same size as fileselector
 if(!rp) rp = ncplane_create(screen, &redopt); // if the menu dosent exist, create it
 gen_rm(cfile, rp); // reading file & drawing logic
 if(c == 'q') { // if q is pressed
  ncplane_destroy(rp); // destroy the plane
  rp = nullptr; // set it to null
  menu = 1; // and return to the fileselctors scene
 }
}
// Open.. | submenu
if (menu == 2) { // if menu... equals two 
 if(!cs) cs = ncplane_create(screen, &csel); // create the submenu if it dosent exist
 if(c == 'w') { // if w is pressed
   if(sel2 != 0) sel2--; // decrement sel2 if it isnt 0
 }
 if(c == 's') { // if s is pressed
   sel2++; // increment sel2
 }
 int option = update_sm(cs, csel, sel2); // returns the selected option of the menu (submenu logic)
 if(c == 'm') { // option logic (maybe functionalize?)
  if(option == 1) { // open reader
    ncplane_destroy(cs);
    cs = nullptr;
    menu = 3;
  }
  if(option == 2) { // copy (save fp to be pasted)
   temp = cfile;
   ncplane_destroy(cs);
   cs = nullptr;
   menu = 1;
  }
  if(option == 3) { // paste
   paste(temp, cfile);
   ncplane_destroy(cs);
   cs = nullptr;
   menu = 1;
  }
 }
 if(c == 'q') {
  ncplane_destroy(cs);
  cs = nullptr;
  menu = 1;
 }
} 
if(menu == 1) { // if the file selector is open and that is our scene
 cfile = update_filsel(idk, files, mansel.rows, selected, root, rootlen); // menu drawing logic
 if(c == 'w') {
   if(selected != 0) selected--;
 }
 if(c == 's' && selected < files.size() - 1) {
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
   menu = 2;
  }
 }
 if(c == 'b') {
  prev = std::filesystem::path(root).parent_path().string();
  root = prev;
  rootlen = root.length();
  files = getdir(root);
  selected = 0;
 } 
}
notcurses_render(nc);
ncvisual_blit(nc, bimg, &bopt);
}
int sd = notcurses_stop(nc); // stop detect
return 0; // return successful program execution
}
