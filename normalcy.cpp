#include <iostream>
#include <notcurses/notcurses.h>
#include <locale>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>

int main() {
std::setlocale(LC_ALL, ""); // set2 a locale notcurses suppor>
notcurses_options ncopt { // emtpy options struct 2use defaul>

};
struct winsize size; // create winsize struct
ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); // grab winsize and >
unsigned int row = size.ws_row; // the screens rows
unsigned int col = size.ws_col; // the screens columns
struct notcurses* nc = notcurses_init(&ncopt, stdout); // ini>
ncinput in;
ncplane_options scropt { // constant structure representing t>
 0, // y
 0,  // x
 row, // rows
 col  // cols
};
// create ncplae for the main screen area
struct ncplane* screen = notcurses_stdplane(nc);
ncplane_options bgopt {};
bgopt.y = 0;
bgopt.x = 0;
bgopt.rows = row;
bgopt.cols = col;
ncplane* bgplane = ncplane_create(notcurses_stdplane(nc), &bgopt);
const char* bimgp = "./assets/crotch.png"; // path to backgro>
ncvisual* bimg = ncvisual_from_file(bimgp); // load ncvisual >
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
ncplane_options fuckop {};
fuckop.y = 0;
fuckop.x = 0;
fuckop.rows = 20;
fuckop.cols = 20;
ncplane* fuckplane = ncplane_create(notcurses_stdplane(nc), &fuckop);



while(true) {
 uint32_t c = notcurses_get_nblock(nc, &in);
 if(c) {
  if(c == '`') break;
  if(c == 'q') {
   if(fuckplane) {
    ncplane_destroy(fuckplane);
    fuckplane = nullptr;
   }
  }
 }
 ncvisual_blit(nc, bimg, &bopt);
 notcurses_render(nc);
}
 int sd = notcurses_stop(nc);
 return 0;
}
