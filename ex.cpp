#include <notcurses/notcurses.h>

#include <locale>



int main() {

    setlocale(LC_ALL, "");



    notcurses_options ncopt{};

    struct notcurses* nc = notcurses_init(&ncopt, stdout);



    ncselector_item items[] = {

        { "the option", "and the desc" },

        { nullptr, nullptr }

    };



    ncselector_options seleopt{};

    seleopt.title = "el title";

    seleopt.secondary = "el secondary";

    seleopt.footer = "the feet";

    seleopt.items = items;

    seleopt.defidx = 0;



    ncplane_options scropt{};

    scropt.y = 2;

    scropt.x = 4;

    scropt.rows = 10;

    scropt.cols = 40;



    ncplane* plane = ncplane_create(notcurses_stdplane(nc), &scropt);

    ncselector* sel = ncselector_create(plane, &seleopt);



    ncinput in;

    while (true) {

        notcurses_render(nc);



        int ch = notcurses_get(nc, nullptr, &in);

        if (ch == 'q') break;



        ncselector_offer_input(sel, &in);

    }



    notcurses_stop(nc);

}
