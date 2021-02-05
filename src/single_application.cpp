#include "single_application.h"
#include "resource_manager.h"
#include "resource.h"
#include "common.h"

#include <cassert>
#include <nana/gui.hpp>

bool single_application::init()
{
    //获取临时目录
    auto resource_base_path = resource_manager::get()->get_unique_tempdir();
    if (resource_base_path.empty())
        return false;
    //auto loadResourceImg = std::bind(loadImage, resource_base_path, L"PNG", std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    auto loadImage = [&](int resourceId, const std::filesystem::path& suffix, nana::paint::image& img) {
        bool bRet = resource_manager::get()->release_image_file(resourceId, L"PNG", (resource_base_path  / suffix).u8string());
        if (!bRet) {
            ERR() << "load resource id:" << resourceId << ", type: PNG failed !";
            return false;
        }
        bRet = img.open(resource_base_path / suffix);
        if (!bRet) {
            ERR() << "open file: " << (resource_base_path / suffix) << " failed !";
            return false;
        }
        return true;
    };

    bool bRet = true;
    bRet &= loadImage(IDB_PNG_BG, "icon_update.png", _bgImage);
    bRet &= loadImage(IDB_PNG_LOGO_UPDATE, "LOGO_Update.png", _logoImage);
    bRet &= loadImage(IDB_PNG_CLOSE_HOVER, "main-icon-close-hover.png", _closeHoverImage);
    bRet &= loadImage(IDB_PNG_CLOSE_NORMAL, "main-icon-close-normal.png", _closeNormalImage);
    bRet &= loadImage(IDB_PNG_CLOSE_PRESSED, "main-icon-close-pressed.png", _closePressedImage);
    bRet &= loadImage(IDB_PNG_MIN_HOVER, "main-icon-min-hover.png", _minHoverImage);
    bRet &= loadImage(IDB_PNG_MIN_NORMAL, "main-icon-min-normal.png", _minNormalImage);
    bRet &= loadImage(IDB_PNG_MIN_PRESSED, "main-icon-min-pressed.png", _minPressedImage);

    assert(bRet);
    return true;
}

std::shared_ptr<nana::form> single_application::createWindow(int width, int height)
{
    std::shared_ptr<nana::form> pForm = std::make_shared<nana::form>(nana::API::make_center(width, height), nana::appearance{ nana::appear::bald<>() });
    return pForm;
}

std::shared_ptr<nana::form> single_application::createWindow(int x, int y, int width, int height)
{
    std::shared_ptr<nana::form> pForm = std::make_shared<nana::form>(nana::rectangle{ x,y,width, height }, nana::appearance{ nana::appear::bald<>() });
    return pForm;
}

int single_application::exec()
{
    nana::exec();
    return 0;
}
