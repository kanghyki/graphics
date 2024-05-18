#ifndef INCLUDED_MODEL_H
#define INCLUDED_MODEL_H

// clang-format off
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// clang-format on

#include "material.h"
#include "mesh.h"
#include "ptr.h"

#include <string>

CLASS_PTR(Model);
class Model
{
  public:
    static ModelPtr Load(const std::string &filename);
    static ModelPtr Create(const std::vector<MeshPtr> &meshes);
    ~Model();

    void Draw(const Program *program) const;

    MeshPtr mesh(int i)
    {
        if (i >= 0 && i < meshes_.size())
        {
            return meshes_[i];
        }
        return nullptr;
    }
    size_t meshes_count() const
    {
        return meshes_.size();
    }

  private:
    Model();
    Model(const Model &model);

    bool LoadByAssimp(const std::string &filename);
    void ProcessMesh(aiMesh *ai_mesh, const aiScene *ai_scene);
    void ProcessNode(aiNode *ai_node, const aiScene *ai_scene);

    std::vector<MeshPtr> meshes_;
    std::vector<MaterialPtr> materials_;
};

#endif
