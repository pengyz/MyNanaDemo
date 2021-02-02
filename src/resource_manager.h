#pragma once;
#include <vector>
#include <map>
#include <string>
#include <windows.h>
#ifdef _USE_OPENGL3
#include <glad/glad.h>
#else
#include <GL/GL.h>
#endif

struct GLTexture_ {
    int width;
    int height;
    GLuint tex;
};


class ResourceManager {
public:
    ~ResourceManager();
    static ResourceManager* get();
protected:
    ResourceManager() = default;

public:
    /**
     * @brief 加载RC图片资源到OpenGL纹理
     * @author Alex.peng
     */
    bool loadResourceImage(int rcId, LPCWSTR rcType, GLTexture_& texture);
    /**
     * @brief 获取RC配置文件
     * @author Alex.peng
     */
    bool loadResourceData(int rcId, LPCWSTR rcType, std::vector<char>& data);
    /**
     * @brief 从文件中读取texture
     * @author Alex.peng
     */
    bool loadTextureFromFile(const std::string& filename, GLTexture_& texture);
    /**
     * @brief 接在texture
     * @author Alex.peng
     */
    bool loadTexture(unsigned char* data, int width, int height, GLTexture_& texture);


private:
    static ResourceManager* m_instance;
};