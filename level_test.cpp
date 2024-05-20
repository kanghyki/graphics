#include "level_test.h"
#include "actor.h"
#include "camera_component.h"
#include "input_manager.h"
#include "layer.h"
#include "level.h"
#include "level_manager.h"
#include "light_component.h"
#include "material.h"
#include "model_component.h"
#include "opengl_device.h"
#include "time_manager.h"
#include "transform_component.h"

class CameraTransformComponent : public TransformComponent
{
  public:
    CameraTransformComponent()
    {
        transform().position_ = {0.0f, 1.0f, 5.0f};
    }
    void Tick() override
    {
        float dt = TimeManager::GetInstance()->delta_time();

        glm::vec3 &cur_position = transform().position_;
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

        glm::vec2 delta = InputManager::GetInstance()->delta_cursor();
        float &yaw_ = transform().rotation_.x;
        float &pitch_ = transform().rotation_.y;
        yaw_ -= delta.x * rot_speed_;
        pitch_ -= delta.y * rot_speed_;

        if (yaw_ < 0.0f)
            yaw_ += 360.0f;
        if (yaw_ > 360.0f)
            yaw_ -= 360.0f;

        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

  private:
    float speed = 1.0f;
    float rot_speed_ = 0.15f;
};

class RotationTransformComponent : public TransformComponent
{
  public:
    RotationTransformComponent()
    {
        transform().scale_ = glm::vec3(0.5f);
    }
    void Tick()
    {
        float dt = TimeManager::GetInstance()->delta_time();
        transform().rotation_.x += x_rotation_speed * dt;
        transform().rotation_.y += y_rotation_speed * dt;
    }

  private:
    float x_rotation_speed = 45.0f;
    float y_rotation_speed = 20.0f;
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

    auto diffuse_tex = Texture2d::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)).get());
    auto specular_tex = Texture2d::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).get());

    auto box_material = Material::Create();
    box_material->set_texture(TextureType::DIFFUSE, diffuse_tex);
    box_material->set_texture(TextureType::SPECULAR, specular_tex);
    box_material->set_shineness(30.0f);
    box_material->uniform().m_vec4_0 = glm::vec4(1.0f, 0.5f, 0.4f, 1.0f);
    auto box_mesh = Mesh::CreateBox();
    box_mesh->set_material(box_material);

    auto sphere_material = Material::Create();
    sphere_material->set_texture(TextureType::DIFFUSE, diffuse_tex);
    sphere_material->set_texture(TextureType::SPECULAR, specular_tex);
    sphere_material->uniform().m_vec4_0 = glm::vec4(1.0f);
    sphere_material->set_shineness(30.0f);
    auto sphere_mesh = Mesh::CreateSphere(20, 20);
    sphere_mesh->set_material(sphere_material);

    Actor *box = new Actor("Little box");
    box->SetComponent(std::shared_ptr<Component>(new RotationTransformComponent()));
    box->AddMeshComponent();
    box->GetModelComponent()->set_model(Model::Create({box_mesh}));

    Actor *sun = new Actor("My sun");
    sun->AddLightComponent();
    sun->AddMeshComponent();
    sun->GetModelComponent()->set_model(Model::Create({sphere_mesh}));
    sun->GetTransformComponent()->transform().position_ = glm::vec3(0.0f, 2.0f, 0.0f);
    sun->GetTransformComponent()->transform().scale_ *= 0.2;

    Actor *cam = new Actor("Camera man");
    cam->SetComponent(std::shared_ptr<Component>(new CameraTransformComponent()));
    cam->AddCameraComponent();
    cam->GetCameraComponent()->set_pso(OpenGLDevice::GetInstance()->GetPSO(GRAPHIC_PSO_TYPE::LIGHTING));

    auto layer00 = level->AddLayer("object");
    layer00->AddActor(box);
    auto layer01 = level->AddLayer("player");
    layer01->AddActor(cam);
    auto layer02 = level->AddLayer("light");
    layer02->AddActor(sun);

    LevelManager::GetInstance()->AddLevel(level);
    LevelManager::GetInstance()->SetCurrentLevel(level->name());

    Level *main = new Level("main");
    LevelManager::GetInstance()->AddLevel(main);
}
