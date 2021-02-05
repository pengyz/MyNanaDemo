#pragma once;
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include <windows.h>


class resource_manager {
public:
    ~resource_manager();
    static resource_manager* get();
protected:
    resource_manager() = default;

public:
    /**
     * @brief 获取RC配置文件
     * @author Alex.peng
     */
    bool release_image_file(int rcId, LPCWSTR rcType, const std::string& filePath);
    /**
     * @brief 获取唯一的临时路径
     * @author Alex.peng
     */
    const std::filesystem::path& get_unique_tempdir();

    uintmax_t cleanup(const std::filesystem::path& temporyPath);

private:
    static resource_manager* m_instance;

    std::filesystem::path _temporyPath;
};