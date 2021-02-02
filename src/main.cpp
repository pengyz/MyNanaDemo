#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/dragger.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/msgbox.hpp>
#include <nana/gui/widgets/picture.hpp>

#include <tchar.h>
#include <windows.h>

int WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{    
    nana::form fm{ nana::API::make_center(600,160),nana::appear::bald<>() };
    fm.bgcolor(nana::color(37, 37, 41));
    nana::dragger dg;
    dg.target(fm);
    nana::button btn(fm);
    btn.caption(u8"退出");
    btn.events().click([&]() {
        nana::msgbox msgbox(fm.handle(), u8"提示", nana::msgbox::yes_no);
        msgbox.icon(msgbox.icon_question);
        msgbox << u8"是否需要关闭此程序？";
        if (msgbox.pick_yes == msgbox())
            nana::API::exit_all();
    });
    auto originalBg = btn.bgcolor();
    btn.events().mouse_enter([&]() {
        btn.bgcolor(nana::colors::green);
    });
    btn.events().mouse_leave([&]() {
        btn.bgcolor(originalBg);
    });
    btn.events().mouse_down([&]() {
        btn.bgcolor(nana::colors::black);
    });
    nana::label lbTitle(fm);
    lbTitle.text_align(nana::align::left, nana::align_v::center);
    lbTitle.transparent(true);
    nana::progress _progress(fm);
    _progress.unknown(false);
    _progress.amount(100);
    _progress.value(50);
    _progress.bgcolor(nana::colors::black);
    _progress.fgcolor(nana::color(123, 32, 243));

    nana::place place_(fm);
    place_.div("<vert main <weight=40px titleBar><weight=50px button><weight=20px progress>>");
    place_["titleBar"] << lbTitle;
    //place_["button"] << btn;
    //place_["progress"] << _progress;
    place_.collocate();
    dg.trigger(lbTitle);

    nana::timer _timer;
    _timer.elapse([&](const nana::arg_elapse& a) {
        if (_progress.value() == _progress.amount())
            _progress.value(0);
        _progress.inc();
    });
    _timer.interval(std::chrono::milliseconds(100));
    _timer.start();

    fm.show();
    nana::exec(
#ifdef NANA_AUTOMATIC_GUI_TESTING
        2, 1, [&btn]() { click(btn); }
#endif
    );
    return 0;
}

