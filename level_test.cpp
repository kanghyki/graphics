#include "level_test.h"
#include "actor.h"
#include "camera_component.h"
#include "input_manager.h"
#include "layer.h"
#include "level.h"
#include "level_manager.h"
#include "libmath.h"
#include "light_component.h"
#include "material.h"
#include "model_component.h"
#include "renderer.h"
#include "time_manager.h"
#include "transform_component.h"
#include <spdlog/spdlog.h>

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

        glm::vec3 camera_front =
            glm::rotate(glm::mat4(1.0f), glm::radians(transform().rotation_.x), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(transform().rotation_.y), glm::vec3(1.0f, 0.0f, 0.0f)) *
            transform().direction_;
        if (KEY_HOLD(Key::W))
        {
            transform().position_ += dt * speed * camera_front;
        }
        if (KEY_HOLD(Key::S))
        {
            transform().position_ -= dt * speed * camera_front;
        }
        glm::vec3 camera_right = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), -camera_front));
        if (KEY_HOLD(Key::A))
        {
            transform().position_ -= dt * speed * camera_right;
        }
        if (KEY_HOLD(Key::D))
        {
            transform().position_ += dt * speed * camera_right;
        }
        glm::vec3 camera_up = glm::normalize(glm::cross(-camera_front, camera_right));
        if (KEY_HOLD(Key::Q))
        {
            transform().position_ += dt * speed * camera_up;
        }
        if (KEY_HOLD(Key::E))
        {
            transform().position_ -= dt * speed * camera_up;
        }
        if (MOUSE_HOLD(Mouse::RIGHT))
        {
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
        if (transform().rotation_.x >= 360.0f)
        {
            transform().rotation_.x -= 360.0f;
        }
        transform().rotation_.y += y_rotation_speed * dt;
        if (transform().rotation_.y >= 360.0f)
        {
            transform().rotation_.y -= 360.0f;
        }
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

    Level *level = LevelManager::GetInstance()->AddLevel("test_level");

    std::string resource_path(RESOURCE_DIR_PATH);
    Actor *floor = new Actor("Floor");
    floor->AddModelComponent();
    auto floor_mesh = Mesh::CreateBox();
    floor_mesh->set_material(Material::Create());
    auto floor_material = floor_mesh->material();
    floor_material->set_texture(TextureType::ALBEDO, Texture::Load(resource_path + "\\texture2d\\brickwall.jpg"));
    floor->GetModelComponent()->set_model(Model::Create({floor_mesh}));
    floor->GetTransformComponent()->transform().scale_ = glm::vec3(10.0f, 0.1f, 10.0f);
    floor->GetTransformComponent()->transform().position_ = glm::vec3(0.0f, 0.0f, 0.0f);

    Actor *box = new Actor("Box");
    auto box_mesh = Mesh::CreateBox();
    box_mesh->set_material(Material::Create());
    auto box_material = box_mesh->material();
    box_material->set_texture(TextureType::ALBEDO, Texture::Load(resource_path + "\\texture2d\\box.png"));
    box_material->set_texture(TextureType::SPECULAR, Texture::Load(resource_path + "\\texture2d\\box_spec.png"));
    box->SetComponent(std::shared_ptr<Component>(new RotationTransformComponent()));
    box->AddModelComponent();
    box->GetModelComponent()->set_model(Model::Create({box_mesh}));
    box->GetTransformComponent()->transform().position_ = glm::vec3(0.0f, 0.5f, -0.5f);

    Actor *backpack = new Actor("Backpack");
    backpack->AddModelComponent();
    backpack->GetModelComponent()->set_model(Model::Load(resource_path + "\\model\\backpack.obj"));
    backpack->GetTransformComponent()->transform().scale_ = glm::vec3(0.3f, 0.3f, 0.3f);
    backpack->GetTransformComponent()->transform().position_ = glm::vec3(3.0f, 0.5f, -0.5f);
    backpack->GetTransformComponent()->transform().rotation_ = glm::vec3(0.0f, -30.0f, 0.0f);

    Actor *light_0 = new Actor("Light_red");
    light_0->AddLightComponent();
    light_0->AddModelComponent();
    auto red = Mesh::CreateSphere(30, 30);
    red->set_material(Material::Create());
    red->material()->set_texture(
        TextureType::ALBEDO,
        Texture::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(0.8f, 0.0f, 0.0f, 1.0f)).get()));
    light_0->GetModelComponent()->set_model(Model::Create({red}));
    light_0->GetTransformComponent()->transform().position_ = glm::vec3(0.0f, 2.0f, -2.5f);
    light_0->GetTransformComponent()->transform().scale_ *= 0.2;
    light_0->GetLightComponent()->light().type_ = LightType::POINT;
    light_0->GetLightComponent()->light().strength_ = glm::vec3(0.8f, 0.0f, 0.0f);

    Actor *light_1 = new Actor("Light_green");
    light_1->AddLightComponent();
    light_1->AddModelComponent();
    auto blue = Mesh::CreateSphere(30, 30);
    blue->set_material(Material::Create());
    blue->material()->set_texture(
        TextureType::ALBEDO,
        Texture::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(0.0f, 0.8f, 0.0f, 1.0f)).get()));
    light_1->GetModelComponent()->set_model(Model::Create({blue}));
    light_1->GetTransformComponent()->transform().position_ = glm::vec3(0.0f, 2.0f, 3.0f);
    light_1->GetTransformComponent()->transform().scale_ *= 0.2;
    light_1->GetLightComponent()->light().type_ = LightType::POINT;
    light_1->GetLightComponent()->light().strength_ = glm::vec3(0.0f, 0.8f, 0.0f);

    Actor *light_2 = new Actor("Light_blue");
    light_2->AddLightComponent();
    light_2->AddModelComponent();
    auto green = Mesh::CreateSphere(30, 30);
    green->set_material(Material::Create());
    green->material()->set_texture(
        TextureType::ALBEDO,
        Texture::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(0.0f, 0.0f, 0.8f, 1.0f)).get()));
    light_2->GetModelComponent()->set_model(Model::Create({green}));
    light_2->GetTransformComponent()->transform().position_ = glm::vec3(-3.0f, 2.5f, 0.0f);
    light_2->GetTransformComponent()->transform().scale_ *= 0.2;
    light_2->GetLightComponent()->light().type_ = LightType::POINT;
    light_2->GetLightComponent()->light().strength_ = glm::vec3(0.0f, 0.0f, 0.8f);

    Actor *light_3 = new Actor("Light_yellow");
    light_3->AddLightComponent();
    light_3->AddModelComponent();
    auto yellow = Mesh::CreateSphere(30, 30);
    yellow->set_material(Material::Create());
    yellow->material()->set_texture(
        TextureType::ALBEDO,
        Texture::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(0.8f, 0.8f, 0.0f, 1.0f)).get()));
    light_3->GetModelComponent()->set_model(Model::Create({yellow}));
    light_3->GetTransformComponent()->transform().position_ = glm::vec3(3.0f, 2.0f, -3.0f);
    light_3->GetTransformComponent()->transform().scale_ *= 0.2;
    light_3->GetLightComponent()->light().type_ = LightType::POINT;
    light_3->GetLightComponent()->light().strength_ = glm::vec3(0.5f, 0.5f, 0.0f);

    Actor *cam = new Actor("Camera man");
    cam->SetComponent(std::shared_ptr<Component>(new CameraTransformComponent()));
    cam->AddCameraComponent();
    cam->GetCameraComponent()->set_render({Renderer::GetInstance()->GetPSO(PsoType::G_BUFFER),
                                           Renderer::GetInstance()->GetFramebuffer(FramebufferType::MAIN)});

    auto layer00 = level->AddLayer("object");
    layer00->AddActor(box);
    layer00->AddActor(floor);
    layer00->AddActor(backpack);
    auto layer01 = level->AddLayer("player");
    layer01->AddActor(cam);
    auto layer02 = level->AddLayer("light");
    layer02->AddActor(light_0);
    layer02->AddActor(light_1);
    layer02->AddActor(light_2);
    layer02->AddActor(light_3);

    LevelManager::GetInstance()->SetCurrentLevel(level->name());

    LevelManager::GetInstance()->AddLevel("main");
}
