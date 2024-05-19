#include "model_resource.h"

ModelResourcePtr ModelResource::Create()
{
    auto resource = ModelResourcePtr(new ModelResource());

    return resource;
}

ModelResource::ModelResource()
{
}

ModelResource::~ModelResource()
{
}

void ModelResource::LoadImpl(const std::string &full_path)
{
}