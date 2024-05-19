#include "directory.h"
#include <filesystem>
#include <spdlog/spdlog.h>

DirectoryPtr Directory::Create(const std::string &path)
{
    auto dir = DirectoryPtr(new Directory());
    dir->Init(path);

    return dir;
}

void Directory::Init(const std::string &path)
{
    path_ = path;
    size_t i = path.find_last_of('/');
    if (i == std::string::npos)
    {
        name_ = path_;
    }
    else
    {
        name_ = path_.substr(i + 1);
    }
}

Directory::Directory()
{
}

Directory::~Directory()
{
}

void Directory::Load()
{
    for (const auto &entry : std::filesystem::directory_iterator(path_))
    {
        SPDLOG_INFO("{}", entry.path().filename().string());
        if (entry.is_directory())
        {
            auto new_dir = Directory::Create(entry.path().string());
            new_dir->Load();
            directories_.push_back(new_dir);
        }
        else if (entry.is_regular_file())
        {
            auto new_file = RegularFile::Create(entry.path().string());
            files_.push_back(new_file);
        }
    }
}