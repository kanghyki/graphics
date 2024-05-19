#ifndef INCLUDED_MODEL_RESOURCE_H
#define INCLUDED_MODEL_RESOURCE_H

#include "model.h"
#include "ptr.h"
#include "resource.h"

CLASS_PTR(ModelResource);
class ModelResource : public Resource
{
  public:
    static ModelResourcePtr Create();
    ~ModelResource();

    ModelPtr model()
    {
        return model_;
    }

  private:
    ModelResource();
    ModelResource(const ModelResource &rhs);
    ModelResource &operator=(const ModelResource &rhs);

    void LoadImpl(const std::string &full_path) override;

    ModelPtr model_;
};

#endif