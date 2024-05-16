#include "level_test.h"
#include "actor.h"
#include "camera_component.h"
#include "input_manager.h"
#include "level.h"
#include "level_manager.h"
#include "mesh_component.h"
#include "renderer.h"
#include "time_manager.h"
#include "transform_component.h"
#include <iostream>

class CameraTransformComponent : public TransformComponent
{
  public:
    CameraTransformComponent()
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

class RotationTransformComponent : public TransformComponent
{
  public:
    void Tick()
    {
        float dt = TimeManager::GetInstance()->delta_time();
        glm::vec3 rotation = transform().rotation();
        rotation.x *= x_rotation_speed * dt;
        rotation.y *= y_rotation_speed * dt;
        transform().set_rotation(rotation);
    }

  private:
    float x_rotation_speed = 180.f;
    float y_rotation_speed = 90.f;
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
    MeshComponent *mesh = new MeshComponent();
    mesh->set_model(Model::Create({Mesh::CreateBox()}));
    actor->SetComponent(mesh);
    actor->SetComponent(new RotationTransformComponent());

    Actor *camera_man = new Actor("Camera");
    CameraComponent *cam = new CameraComponent();
    cam->set_pso(Renderer::GetInstance()->GetPSO(GRAPHIC_PSO_TYPE::SIMPLE));
    camera_man->SetComponent(cam);
    camera_man->SetComponent(new CameraTransformComponent());

    level->AddActor(actor, LayerType::PLAYER);
    level->AddActor(camera_man, LayerType::PLAYER);

    LevelManager::GetInstance()->AddLevel(level);
    LevelManager::GetInstance()->SetCurrentLevel(level->name());
}
