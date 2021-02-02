#include "resource_manager.h"
//#include "stb_image.h"
#include <spdlog/spdlog.h>
#include "resource.h"



ResourceManager* ResourceManager::m_instance = nullptr;


ResourceManager::~ResourceManager()
{

}

ResourceManager* ResourceManager::get()
{
    if (!m_instance)
        m_instance = new ResourceManager();
    return m_instance;
}

bool ResourceManager::loadResourceImage(int rcId, LPCWSTR rcType, GLTexture_& texture)
{
    std::vector<char> data;
    bool bOk = loadResourceData(rcId, rcType, data);
    if (!bOk) {
        LOG(ERROR) << "loadResourceData failed !" << std::endl;
        return false;
    }

    int width = 0, height = 0, channel = 0;
    auto image_data = stbi_load_from_memory((const stbi_uc*)data.data(), data.size(), &width, &height, &channel, 4);
    if (!image_data) {
        LOG(ERROR) << "load png from memory failed !!!" << std::endl;
        return false;
    }
    loadTexture(image_data, width, height, texture);
    stbi_image_free(image_data);
    return true;
}

bool ResourceManager::loadResourceData(int rcId, LPCWSTR rcType, std::vector<char>& data)
{
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hRC = FindResourceW(hModule, MAKEINTRESOURCEW(rcId), rcType);
    if (!hRC) {
        LOG(ERROR) << "FindResource failed for resource id: " << rcId << ", type: " << SString(rcType).toUtf8() << " failed !!!" << std::endl;
        return false;
    }

    DWORD dwSize = SizeofResource(hModule, hRC);
    if (!dwSize) {
        LOG(ERROR) << "resource size zero !!!" << std::endl;
        return false;
    }
    HGLOBAL hGlobal = LoadResource(hModule, hRC);
    if (!hGlobal) {
        LOG(ERROR) << "LoadResource failed !!!" << std::endl;
        return false;
    }
    unsigned char* str = (unsigned char*)LockResource(hGlobal);
    data.resize(dwSize);
    memcpy(data.data(), str, dwSize);
    FreeResource(hGlobal);
    return true;
}


bool ResourceManager::loadTextureFromFile(const std::string& filename, GLTexture_& texture)
{
    memset(&texture, 0, sizeof(texture));
    // Load from file
    int width = 0, height = 0;
    unsigned char* image_data = stbi_load(filename.c_str(), &width, &height, NULL, 4);
    if (image_data == nullptr)
        return false;
    bool bOk = loadTexture(image_data, width, height, texture);
    stbi_image_free(image_data);
    return bOk;
}

bool ResourceManager::loadTexture(unsigned char* data, int width, int height, GLTexture_& texture)
{
    memset(&texture, 0, sizeof(texture));
    texture.width = width;
    texture.height = height;
    glGenTextures(1, &texture.tex);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    return true;
}