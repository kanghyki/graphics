#ifndef INCLUDED_RESOURCE_H
#define INCLUDED_RESOURCE_H

#include <string>

class Resource
{
  public:
    Resource();
    ~Resource();

    std::string name()
    {
        return name_;
    }
    void set_name(const std::string &name)
    {
        name_ = name;
    }

    std::string relative_path()
    {
        return relative_path_;
    }
    void set_relative_path(const std::string &relative_path)
    {
        relative_path_ = relative_path;
    }

  private:
    Resource(const Resource &);
    Resource &operator=(const Resource &);

    std::string name_;
    std::string relative_path_;
};

#endif
