#include "resource_manager.h"
#include "resource.h"
#include "common.h"

#include <fstream>
#include <nana/gui/widgets/picture.hpp>



resource_manager* resource_manager::m_instance = nullptr;


resource_manager::~resource_manager()
{

}

resource_manager* resource_manager::get()
{
	if (!m_instance)
		m_instance = new resource_manager();
	return m_instance;
}

bool resource_manager::release_image_file(int rcId, LPCWSTR rcType, const std::string& filePath)
{
	HMODULE hModule = GetModuleHandle(nullptr);
	HRSRC hRC = FindResourceW(hModule, MAKEINTRESOURCEW(rcId), rcType);
	if (!hRC) {
		ERR() << "FindResource failed for resource id: " << rcId << ", type: " << rcType << " failed !!!";
		return false;
	}

	DWORD dwSize = SizeofResource(hModule, hRC);
	if (!dwSize) {
		ERR() << "resource size zero !!!";
		return false;
	}
	HGLOBAL hGlobal = LoadResource(hModule, hRC);
	if (!hGlobal) {
		ERR() << "LoadResource failed !!!";
		return false;
	}
	auto strData = static_cast<const char*>(LockResource(hGlobal));
	auto targetFilePath = std::filesystem::u8path(filePath);
	std::fstream fs(targetFilePath, std::ios::out | std::ios::binary);
	if (!fs.is_open()) {
		ERR() << "open path: " << targetFilePath.u8string() << " failed !";
		return false;
	}
	fs.write(strData, dwSize);
	fs.close();
	FreeResource(hGlobal);
	return true;
}

const std::filesystem::path& resource_manager::get_unique_tempdir()
{
	if (!_temporyPath.empty() && std::filesystem::exists(_temporyPath))
		return _temporyPath;

	WCHAR szFilePath[MAX_PATH + 1] = { 0 };
	WCHAR szOutBuffer[MAX_PATH] = { 0 };
	DWORD dwSize = GetTempPathW(ARRAYSIZE(szFilePath), szFilePath);
	if (dwSize >= ARRAYSIZE(szFilePath))
		return _temporyPath;
	int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
	auto tmpFileName = (std::stringstream() << "NglPacman_" << timestamp).str();
	_temporyPath = std::filesystem::path(szFilePath) / std::filesystem::u8path(tmpFileName);
	if (std::filesystem::exists(_temporyPath)) {
		cleanup(_temporyPath);
	}
	if (!std::filesystem::create_directories(_temporyPath)) {
		ERR() << "create path: " << _temporyPath.u8string() << " failed !!!";
		_temporyPath.clear();
	}
	return _temporyPath;
}

uintmax_t resource_manager::cleanup(const std::filesystem::path& temporyPath)
{
	//删除临时目录
	if (!std::filesystem::exists(temporyPath)) {
		ERR() << "tempory path: " << temporyPath << " not exist !!!";
		return -1;
	}
	std::error_code ec;
	auto size = std::filesystem::remove_all(temporyPath, ec);
	if (ec) {
		ERR() << "remove all failed with: " << ec.message();
	}
	INF() << "remove directory: " << temporyPath << " success, " << size << " files deleted";
	return size;
}