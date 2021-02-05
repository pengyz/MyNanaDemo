#include <iostream>
#include <filesystem>

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

#include "easy-log.h"

#include "single_application.h"

bool IsUser32AndGdi32Available() {
    static auto is_user32_and_gdi32_available = []() {
        using GetProcessMitigationPolicyType =
            decltype(GetProcessMitigationPolicy)*;
        GetProcessMitigationPolicyType get_process_mitigation_policy_func =
            reinterpret_cast<GetProcessMitigationPolicyType>(GetProcAddress(
                GetModuleHandle(L"kernel32.dll"), "GetProcessMitigationPolicy"));

        if (!get_process_mitigation_policy_func)
            return true;

        PROCESS_MITIGATION_SYSTEM_CALL_DISABLE_POLICY policy = {};
        if (get_process_mitigation_policy_func(GetCurrentProcess(),
            ProcessSystemCallDisablePolicy,
            &policy, sizeof(policy))) {
            return policy.DisallowWin32kSystemCalls == 0;
        }

        return true;
    }();
    return is_user32_and_gdi32_available;
}


bool EnablePerMonitorV2() {
    if (!IsUser32AndGdi32Available())
        return false;

    static const auto set_process_dpi_awareness_context_func =
        reinterpret_cast<decltype(&::SetProcessDpiAwarenessContext)>(
            GetProcAddress(GetModuleHandle(L"user32.dll"), "SetProcessDpiAwarenessContext"));
    if (set_process_dpi_awareness_context_func) {
        return set_process_dpi_awareness_context_func(
            DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    }
    return false;
}

void setDpiAware(int value)
{
    HMODULE hLibrary = LoadLibraryA("Shcore.dll");
    if (!hLibrary) {
        SetProcessDPIAware();
        return;
    }
    using PFN_SetProcessDpiAwareness = HRESULT(WINAPI*)(int value);
    PFN_SetProcessDpiAwareness pSetProcessDpiAwareness = (PFN_SetProcessDpiAwareness)GetProcAddress(hLibrary, "SetProcessDpiAwareness");
    if (!pSetProcessDpiAwareness) {
        SetProcessDPIAware();
        FreeLibrary(hLibrary);
        return;
    }
    //使用PROCESS_PER_MONITOR_DPI_AWARE进行调用
    pSetProcessDpiAwareness(value);
    FreeLibrary(hLibrary);
}


int WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
    //EnablePerMonitorV2();

    auto absPath = std::filesystem::path(__wargv[0]).parent_path();
    SpdLogWrapper::inst().init((absPath / "logs").u8string(), "NanaDemo.log");
    SpdLogWrapper::inst().setLogLevel(spdlog::level::trace);

    //nana::form fm{ nana::API::make_center(600,160),nana::appear::bald<>() };
    single_application sApp;
    sApp.init();
    auto& fm = sApp.createWindow(600, 160);
    fm->bgcolor(nana::color(37, 37, 41));
    nana::dragger dg;
    dg.target(*fm);
    //nana::button btn(*fm);
    /*btn.caption(u8"退出");
    btn.events().click([&]() {
        nana::msgbox msgbox(fm->handle(), u8"提示", nana::msgbox::yes_no);
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
    });*/
    nana::picture minBtnPic(*fm);
    minBtnPic.load(sApp._minNormalImage, { 0,0,28,28 });
    minBtnPic.events().mouse_enter([&]() {
        minBtnPic.load(sApp._minHoverImage);
    });
    minBtnPic.events().mouse_leave([&]() {
        minBtnPic.load(sApp._minNormalImage);
    });
    minBtnPic.events().mouse_down([&]() {
        minBtnPic.load(sApp._minPressedImage);
    });
    minBtnPic.events().mouse_up([&]() {
        //nana::API::mouse_action()
    });
    nana::picture closeBtnPic(*fm);
    closeBtnPic.load(sApp._closeNormalImage, { 0,0,28,28 });
    closeBtnPic.events().mouse_enter([&]() {
        closeBtnPic.load(sApp._closeHoverImage);
    });
    closeBtnPic.events().mouse_leave([&]() {
        closeBtnPic.load(sApp._closeNormalImage);
    });
    closeBtnPic.events().mouse_down([&]() {
        closeBtnPic.load(sApp._closePressedImage);
    });
    minBtnPic.events().mouse_up([&]() {
        nana::msgbox msgbox(fm->handle(), u8"提示", nana::msgbox::yes_no);
        msgbox.icon(msgbox.icon_question);
        msgbox << u8"是否需要关闭此程序？";
        if (msgbox.pick_yes == msgbox())
            nana::API::exit_all();
    });
    nana::label lbTitle(*fm);
    lbTitle.text_align(nana::align::left, nana::align_v::center);
    lbTitle.transparent(true);
    /*nana::progress _progress(fm);
    _progress.unknown(false);
    _progress.amount(100);
    _progress.value(50);
    _progress.bgcolor(nana::colors::black);
    _progress.fgcolor(nana::color(123, 32, 243));*/

    nana::place place_(*fm);
    place_.div("<vert main <titleBar arrange=[variable,28,28] gap=[0,10,10]>>");
    place_["titleBar"] << lbTitle << minBtnPic << closeBtnPic;
    //place_["button"] << btn;
    //place_["progress"] << _progress;
    place_.collocate();
    dg.trigger(lbTitle);

    /*nana::timer _timer;
    _timer.elapse([&](const nana::arg_elapse& a) {
        if (_progress.value() == _progress.amount())
            _progress.value(0);
        _progress.inc();
    });
    _timer.interval(std::chrono::milliseconds(100));
    _timer.start();*/

    fm->show();
    sApp.exec();
}

