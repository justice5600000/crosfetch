#include "widgets.h"
#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <notcurses/notcurses.h>
// for functions that use the notcurses library #chonk #includeshouldntbeliteral
int gen_filsel(int oc, ncplane*& fsp, ncplane* pp, ncplane_options fso) { // open counter, file selector plane(passed as refrence *&), parent plane, fsp options
 if(oc == 0) {
  fsp = ncplane_create(pp, &fso);
  ncplane_move_top(fsp);
  ncplane_set_base(fsp, " ", 0, 0);
  oc = 1;
 } else if(oc == 1) {
  ncplane_destroy(fsp);
  fsp = nullptr;
  oc = 0;
 }

 return oc;
}

std::string update_filsel(ncplane*& fsp, std::vector<std::string> fils, int height, int position, std::string dir, int parentlen) {
 std::string selected_s;
 std::string temp;
 ncplane_erase(fsp);
  for(int i = 0; i < std::min(fils.size(), (size_t)height); i++) {
   if(i == position) {
    selected_s = fils[i];
    temp = "[*] " + fils[i];
    ncplane_putstr_yx(fsp, i, 0, temp.c_str());
   } else {
    temp = "[ ] " + fils[i];
    ncplane_putstr_yx(fsp, i, 0, temp.c_str());
   }
  }

 return selected_s;
}
