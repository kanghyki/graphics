#include "regular_file.h"

RegularFilePtr RegularFile::Create(const std::string &path)
{
    auto file = RegularFilePtr(new RegularFile());
    file->Init(path);

    return file;
}

RegularFile::RegularFile()
{
}

RegularFile::~RegularFile()
{
}

void RegularFile::Init(const std::string &path)
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