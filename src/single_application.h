#pragma once;
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/paint/image.hpp>
#include <memory>

class single_application {
public:
	single_application() = default;
	/**
	 * @brief 初始化
	 * @author Alex.peng
	 */
	bool init();
	/**
	 * @brief 创建窗口
	 * @author Alex.peng
	 */
	std::shared_ptr<nana::form> createWindow(int width, int height);

    std::shared_ptr<nana::form> createWindow(uint_t x, uint_t y, int width, int height);


	int exec();

public:
	nana::paint::image _bgImage;
    nana::paint::image _logoImage;
    nana::paint::image _closeHoverImage;
    nana::paint::image _closeNormalImage;
    nana::paint::image _closePressedImage;
    nana::paint::image _minHoverImage;
	nana::paint::image _minNormalImage;
    nana::paint::image _minPressedImage;
};