#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/dragger.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>

#include <tchar.h>
#include <windows.h>

int WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
    nana::form fm{ nana::API::make_center(400,600),nana::appear::bald<>() };
    nana::dragger dg;
    dg.target(fm);
    //fm.caption("Hello Wordl !");
    nana::button btn(fm, nana::rectangle(20, 20, 150, 150));
    btn.caption("Quit");
    btn.events().click(nana::API::exit_all);
    nana::label lbTitle(fm);
    lbTitle.text_align(nana::align::left, nana::align_v::center);
    lbTitle.caption(u8"标题栏");
    lbTitle.bgcolor(nana::colors::gray);

    nana::place place_(fm);
    place_.div("<vert main <weight=30px titleBar>|<weight=50px button>>");
    place_["titleBar"] << lbTitle;
    place_["button"] << btn;
    place_.collocate();
    dg.trigger(lbTitle);
    fm.show();
    nana::exec(
#ifdef NANA_AUTOMATIC_GUI_TESTING
        2, 1, [&btn]() { click(btn); }
#endif
    );
    return 0;
}

