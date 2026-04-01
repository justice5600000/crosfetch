#include "widgets.h"
#include <notcurses/notcurses.h>

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

