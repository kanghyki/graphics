#include "model.h"
#include "texture.h"
#include <spdlog/spdlog.h>

ModelPtr Model::Load(const std::string &filename)
{
    auto model = ModelPtr(new Model());

    if (!model || !model->LoadByAssimp(filename))
    {
        return nullptr;
    }
    return model;
}

ModelPtr Model::Create(const std::vector<MeshPtr> &meshes)
{
    auto model = ModelPtr(new Model());

    if (!model)
    {
        return nullptr;
    }
    model->meshes_ = meshes;

    return model;
}

Model::Model() : meshes_(), materials_()
{
}

Model::~Model()
{
}

void Model::Draw(const Program *program) const
{
    for (auto &mesh : meshes_)
    {
        mesh->Draw(program);
    }
}

TexturePtr LoadTexture(const std::string &dirname, aiMaterial *ai_material, aiTextureType ai_texture_type)
{
    if (ai_material->GetTextureCount(ai_texture_type) <= 0)
    {
        return nullptr;
    }
    aiString filepath;
    ai_material->GetTexture(ai_texture_type, 0, &filepath);
    auto image = Image::Load(fmt::format(dirname + "/" + filepath.C_Str()));
    if (!image)
    {
        return nullptr;
    }
    return Texture::Create(image.get());
}

bool Model::LoadByAssimp(const std::string &filename)
{
    Assimp::Importer importer;

    auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        SPDLOG_ERROR("failed to load model: {}", filename);
        return false;
    }
    // FIXME: path
    auto dirname = filename.substr(0, filename.find_last_of("\\"));
    for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
    {
        MaterialPtr material = Material::Create();

        aiMaterial *scene_material = scene->mMaterials[i];
        material->set_texture(TextureType::ALBEDO, LoadTexture(dirname, scene_material, aiTextureType_DIFFUSE));
        material->set_texture(TextureType::SPECULAR, LoadTexture(dirname, scene_material, aiTextureType_SPECULAR));
        material->set_texture(TextureType::AO, LoadTexture(dirname, scene_material, aiTextureType_AMBIENT_OCCLUSION));
        material->set_texture(TextureType::NORMAL, LoadTexture(dirname, scene_material, aiTextureType_NORMALS));
        material->set_texture(TextureType::ROUGHNESS,
                              LoadTexture(dirname, scene_material, aiTextureType_DIFFUSE_ROUGHNESS));
        // Metallic = metalness ?
        material->set_texture(TextureType::METAILLIC, LoadTexture(dirname, scene_material, aiTextureType_METALNESS));
        material->set_texture(TextureType::EMISSIVE, LoadTexture(dirname, scene_material, aiTextureType_EMISSIVE));

        materials_.push_back(material);
    }
    ProcessNode(scene->mRootNode, scene);
    return true;
}

void Model::ProcessNode(aiNode *ai_node, const aiScene *ai_scene)
{
    for (uint32_t i = 0; i < ai_node->mNumMeshes; ++i)
    {
        auto mesh_index = ai_node->mMeshes[i];
        auto mesh = ai_scene->mMeshes[mesh_index];
        ProcessMesh(mesh, ai_scene);
    }
    for (uint32_t i = 0; i < ai_node->mNumChildren; ++i)
    {
        ProcessNode(ai_node->mChildren[i], ai_scene);
    }
}

void Model::ProcessMesh(aiMesh *ai_mesh, const aiScene *ai_scene)
{
    std::vector<Vertex> vertices(ai_mesh->mNumVertices);
    std::vector<uint32_t> indices(ai_mesh->mNumFaces * 3);

    for (uint32_t i = 0; i < ai_mesh->mNumVertices; ++i)
    {
        Vertex &vertex = vertices[i];
        if (ai_mesh->HasPositions())
        {
            vertex.position = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
        }
        if (ai_mesh->HasNormals())
        {
            vertex.normal = glm::vec3(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
        }
        if (ai_mesh->HasTextureCoords(0))
        {
            vertex.uv = glm::vec2(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y);
        }
        if (ai_mesh->HasTangentsAndBitangents())
        {
            vertex.tangent = glm::vec3(ai_mesh->mTangents[i].x, ai_mesh->mTangents[i].y, ai_mesh->mTangents[i].z);
        }
    }

    for (uint32_t i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        indices[i * 3] = ai_mesh->mFaces[i].mIndices[0];
        indices[i * 3 + 1] = ai_mesh->mFaces[i].mIndices[1];
        indices[i * 3 + 2] = ai_mesh->mFaces[i].mIndices[2];
    }

    MeshPtr mesh = Mesh::Create(vertices, indices, GL_TRIANGLES);
    if (ai_mesh->mMaterialIndex >= 0)
    {
        mesh->set_material(materials_[ai_mesh->mMaterialIndex]);
    }
    meshes_.push_back(mesh);
}
