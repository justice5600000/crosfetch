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
ncinput in;
ncselector_item item {
 "the option",
 "and the desc"
};
ncselector_options seleopt {
 "el title",
 "el secondary",
 "the feet",
 &item,
 1,
 10,
 1,
 1,
 1,
 1,
 1,
 0
};
ncplane_options scropt {
 0,
 0,
 row,
 col
};
ncplane* screen = ncplane_create(notcurses_stdplane(nc), &scropt);
ncselector* select = ncselector_create(screen, &seleopt);


while(true) {
notcurses_render(nc);
}

}
