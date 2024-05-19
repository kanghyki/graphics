#ifndef INCLUDED_DIRECTORY_H
#define INCLUDED_DIRECTORY_H

#include "ptr.h"
#include "regular_file.h"
#include <string>
#include <vector>

CLASS_PTR(Directory);
class Directory
{
  public:
    static DirectoryPtr Create(const std::string &path);
    ~Directory();

    void Load();

    std::vector<DirectoryPtr> directories()
    {
        return directories_;
    }
    std::vector<RegularFilePtr> files()
    {
        return files_;
    }

  private:
    Directory();
    void Init(const std::string &path);

    std::string path_;
    std::string name_;

    std::vector<DirectoryPtr> directories_;
    std::vector<RegularFilePtr> files_;
};

#endif
