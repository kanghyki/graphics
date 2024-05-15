#include "actor.h"
#include "camera_component.h"
#include "input_manager.h"
#include "level.h"
#include "level_manager.h"
#include "level_test.h"
#include "mesh_component.h"
#include "renderer.h"
#include "time_manager.h"
#include "transform_component.h"
#include <iostream>

class MyTransformComponent : public TransformComponent
{
  public:
    MyTransformComponent()
    {
        transform().set_position({0.0f, 0.0f, 2.0f});
    }
    void Tick() override
    {
        float dt = TimeManager::GetInstance()->delta_time();

        glm::vec3 cur_position = transform().position();
        if (KEY_HOLD(Key::W))
        {
            cur_position.z -= speed * dt;
        }
        if (KEY_HOLD(Key::S))
        {
            cur_position.z += speed * dt;
        }
        if (KEY_HOLD(Key::A))
        {
            cur_position.x -= speed * dt;
        }
        if (KEY_HOLD(Key::D))
        {
            cur_position.x += speed * dt;
        }
        if (KEY_HOLD(Key::Q))
        {
            cur_position.y -= speed * dt;
        }
        if (KEY_HOLD(Key::E))
        {
            cur_position.y += speed * dt;
        }
        transform().set_position(cur_position);
    }

  private:
    float speed = 1.0f;
};

CreateTestLevel::CreateTestLevel()
{
}

CreateTestLevel::~CreateTestLevel()
{
}

void CreateTestLevel::Create()
{
    Level *level = new Level("test_level");

    Actor *actor = new Actor("my actor");

    // TODO:
    CameraComponent *cam = new CameraComponent();
    cam->set_pso(Renderer::GetInstance()->GetPSO(GRAPHIC_PSO_TYPE::SIMPLE));

    actor->SetComponent(cam);
    actor->SetComponent(new MyTransformComponent());
    MeshComponent *mesh = new MeshComponent();
    auto model = Model::Create({Mesh::CreateBox()});
    mesh->set_model(model);
    actor->SetComponent(mesh);

    level->AddActor(actor, LayerType::PLAYER);

    LevelManager::GetInstance()->AddLevel(level);
    LevelManager::GetInstance()->SetCurrentLevel(level->name());
}
