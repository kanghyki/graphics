#ifndef INCLUDED_REGULAR_FILE_H
#define INCLUDED_REGULAR_FILE_H

#include "ptr.h"
#include <string>

CLASS_PTR(RegularFile);
class RegularFile
{
  public:
    static RegularFilePtr Create(const std::string &path);
    ~RegularFile();

  private:
    RegularFile();

    void Init(const std::string &path);

    std::string path_;
    std::string name_;
    uint32_t size_;
};

#endif