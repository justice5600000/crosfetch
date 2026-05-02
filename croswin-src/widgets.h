#ifndef WIDGETS_H

#define WIDGETS_H

#include <notcurses/notcurses.h>
#include <string>
#include <vector>
#include <cstdint>

int gen_filsel(int oc, ncplane*& fsp, ncplane* pp, ncplane_options fso);
std::string update_filsel(ncplane*& fsp, std::vector<std::string> fils, int height, int position, std::string dir, int parentlen);
int update_sm(ncplane* smp, ncplane_options smo, int sel);
int gen_rm(std::string fp, ncplane* tp);
int paste(std::string from, std::string to);

#endif
