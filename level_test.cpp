#include "actor.h"
#include "camera_component.h"
#include "layer.h"
#include "level.h"
#include "level_manager.h"
#include "level_test.h"
#include "mesh_component.h"
#include "renderer.h"
#include "transform_component.h"

CreateTestLevel::CreateTestLevel()
{
}

CreateTestLevel::~CreateTestLevel()
{
}

void CreateTestLevel::Create()
{
    Level *level = new Level("test_level");
    Layer *layer00 = new Layer("Objects");

    Actor *actor = new Actor("my actor");
    actor->SetComponent(new CameraComponent());
    actor->SetComponent(new TransformComponent());
    MeshComponent *mesh = new MeshComponent();
    mesh->set_pso(Renderer::GetInstance()->GetPSO(GRAPHIC_PSO_TYPE::SIMPLE));
    actor->SetComponent(mesh);

    layer00->AddActor(actor);

    level->AddLayer(layer00);
    LevelManager::GetInstance()->AddLevel(level);
    LevelManager::GetInstance()->SetCurrentLevel(level->name());
}
