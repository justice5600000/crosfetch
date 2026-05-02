#include "widgets.h"
#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <notcurses/notcurses.h>
#include <fstream>
// for functions that use the notcurses library #chonk #includeshouldntbeliteral
// generate file selector
int gen_filsel(int oc, ncplane*& fsp, ncplane* pp, ncplane_options fso) { // open counter, file selector plane(passed as refrence *&), parent plane, fsp options
 if(oc == 0) { // if the fileselector isnt open
  fsp = ncplane_create(pp, &fso); // create it on the parent path using given options
  ncplane_move_top(fsp); // move plane to the top of the stack
  ncplane_set_base(fsp, " ", 0, 0); // fill plane with spaces
  oc = 1; // mark as opened
 } else if(oc == 1) { // but if it is open
  ncplane_destroy(fsp); // destroy the plane
  fsp = nullptr; // set the plane to null to put the memory somewhere
  oc = 0; // and mark as closed
 }

 return oc; // return the opened status
}
// update file selector (file picker)
std::string update_filsel(ncplane*& fsp, std::vector<std::string> fils, int height, int position, std::string dir, int parentlen) {
 int max = std::min(fils.size(), (size_t)height);
 int buf = 0; // pad for indexing
 std::string selected_s; // selected string
 std::string temp; // buffer to create whats drawn 
 // progression logic
 if(position > height - 1) { // if the cursor is past the visible reigon (incorecctly offsetted if not minused by one)
  buf = position - (height - 1); // shift rendered list down to the cursors position minus the visible reigon (height minus one)
 }
 ncplane_erase(fsp); // erase the file picker plane upon invocation
  for(int i = 0; i < max; i++) { // for the length of the plane
//   if(buf + i >= fils.size()) break; // edge case 
   if(i == position - buf) { // if the string to be printed is the same as the currently selected string
    selected_s = fils[buf + i]; // return what file path the cursor is on
    temp = "[*] " + fils[buf + i]; // decorate output
    ncplane_putstr_yx(fsp, i, 0, temp.c_str()); // render
   } else {
    temp = "[ ] " + fils[buf + i];
    ncplane_putstr_yx(fsp, i, 0, temp.c_str());
   }
  }

 return selected_s; // return hovered fp
}
// update submenu (select menu)
int update_sm(ncplane* smp, ncplane_options smo, int sel) {
ncplane_erase(smp);
ncplane_move_top(smp); // move plane to the top of the stack
ncplane_set_base(smp, " ", 0, 0); // fill plane with spaces
int size = smo.rows;
int selected = 0;
std::vector<std::string> options;
options.push_back("Read.. (ascii data)");
options.push_back("Copy..");
options.push_back("Paste..");
ncplane_putstr_yx(smp, 0, 0, "Open.. ");
for(int i = 0; i < options.size(); i++) {
 int row = i + 1;
 if(i == sel) {
  std::string buf = "! " + options[i];
  ncplane_putstr_yx(smp, row, 0, buf.c_str());
  selected = i + 1; // indexed at 0 so + returns true value
 } else {
  std::string buf = "  " + options[i];
  ncplane_putstr_yx(smp, row, 0, buf.c_str());
 }
}

 return selected;
}
// generate read menu
int gen_rm(std::string fp, ncplane* tp) { // parent plane, filepath of txt to read, text plane
 ncplane_erase(tp);
 ncplane_set_base(tp, " ", 0, 0);
 std::string contents;
 std::ifstream in(fp); // input file
 std::stringstream buf; // buffer to hold streamed data
 if(in.is_open()) { // if the file opens 
  buf << in.rdbuf(); // stream the whole file into buf 
  contents = buf.str(); // make a new string and convert the buf into string
 }
 size_t strsize = contents.size();
 ncplane_puttext(tp, 0, NCALIGN_LEFT, contents.c_str(), &strsize);

 return 0;
}
// paste
int paste(std::string from, std::string to) {
 std::filesystem::path fp = std::filesystem::path(from); // create from path
 std::filesystem::path fn = fp.filename(); // filename
 std::filesystem::path tp = std::filesystem::path(to).parent_path() / fn; // create to path
 fp = fp.lexically_normal();
 tp = tp.lexically_normal();
 std::filesystem::copy_file(fp, tp); // copy over the file

 return 0;
}