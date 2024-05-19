#ifndef INCLUDED_RESOURCE_H
#define INCLUDED_RESOURCE_H

#include <string>

class Resource
{
  public:
    Resource();
    virtual ~Resource();

    void Load(const std::string &relative_path)
    {
        relative_path_ = relative_path;
        size_t i = relative_path_.find_last_of('/');
        if (i == std::string::npos)
        {
            name_ = relative_path_;
        }
        else
        {
            name_ = relative_path_.substr(i + 1);
        }
        LoadImpl(std::string(RESOURCE_DIR_PATH) + relative_path);
    }

    uint32_t id()
    {
        return id_;
    }
    std::string name()
    {
        return name_;
    }
    std::string relative_path()
    {
        return relative_path_;
    }

  protected:
    virtual void LoadImpl(const std::string &full_path) = 0;

  private:
    Resource(const Resource &);
    Resource &operator=(const Resource &);

    static uint32_t kId;

    uint32_t id_;
    std::string name_;
    std::string relative_path_;
};

#endif
