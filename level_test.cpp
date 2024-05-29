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
#include "skybox_component.h"
#include "time_manager.h"
#include "transform_component.h"
#include <spdlog/spdlog.h>

class CameraTransformComponent : public TransformComponent
{
  public:
    CameraTransformComponent()
    {
        set_position(glm::vec3(0.0f, 1.0f, 5.0f));
    }
    void Tick() override
    {
        float dt = TimeManager::GetInstance()->delta_time();

        glm::vec3 camera_front = glm::rotate(glm::mat4(1.0f), glm::radians(rotation().x), glm::vec3(0.0f, 1.0f, 0.0f)) *
                                 glm::rotate(glm::mat4(1.0f), glm::radians(rotation().y), glm::vec3(1.0f, 0.0f, 0.0f)) *
                                 glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        glm::vec3 pp = position();
        if (KEY_HOLD(Key::W))
        {
            pp += dt * speed * camera_front;
        }
        if (KEY_HOLD(Key::S))
        {
            pp -= dt * speed * camera_front;
        }
        glm::vec3 camera_right = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), -camera_front));
        if (KEY_HOLD(Key::A))
        {
            pp -= dt * speed * camera_right;
        }
        if (KEY_HOLD(Key::D))
        {
            pp += dt * speed * camera_right;
        }
        glm::vec3 camera_up = glm::normalize(glm::cross(-camera_front, camera_right));
        if (KEY_HOLD(Key::Q))
        {
            pp += dt * speed * camera_up;
        }
        if (KEY_HOLD(Key::E))
        {
            pp -= dt * speed * camera_up;
        }
        set_position(pp);
        if (MOUSE_HOLD(Mouse::RIGHT))
        {
            glm::vec2 delta = InputManager::GetInstance()->delta_cursor();
            glm::vec3 rr = rotation();
            float &yaw_ = rr.x;
            float &pitch_ = rr.y;
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
            set_rotation(rr);
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
        set_scale(glm::vec3(0.5f));
    }
    void Tick()
    {
        float dt = TimeManager::GetInstance()->delta_time();

        glm::vec3 rr = rotation();
        rr.x += x_rotation_speed * dt;
        if (rr.x >= 360.0f)
        {
            rr.x -= 360.0f;
        }
        rr.y += y_rotation_speed * dt;
        if (rr.y >= 360.0f)
        {
            rr.y -= 360.0f;
        }
        set_rotation(rr);
    }

  private:
    float x_rotation_speed = 45.0f;
    float y_rotation_speed = 20.0f;
};

class LightTT : public TransformComponent
{
  public:
    LightTT()
    {
    }
    void Tick()
    {
        float dt = TimeManager::GetInstance()->delta_time();
        set_position(glm::rotate(glm::mat4(1.0f), dt * 3.141592f * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                     glm::vec4(position(), 1.0f));
    }
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
    floor_material->albedo_color_ = glm::vec3(0.5f, 0.5f, 0.5f);
    floor_material->specular_alpha_ = 0.0f;
    floor->GetModelComponent()->set_model(Model::Create({floor_mesh}));
    floor->GetTransformComponent()->set_scale(glm::vec3(10.0f, 0.01f, 10.0f));
    floor->GetTransformComponent()->set_position(glm::vec3(0.0f, 0.0f, 0.0f));

    Actor *box = new Actor("Box");
    auto box_mesh = Mesh::CreateBox();
    box_mesh->set_material(Material::Create());
    auto box_material = box_mesh->material();
    box_material->specular_alpha_ = 1.0f;
    box->SetComponent(std::shared_ptr<Component>(new RotationTransformComponent()));
    box->AddModelComponent();
    box->GetModelComponent()->set_model(Model::Create({box_mesh}));
    box->GetTransformComponent()->set_position(glm::vec3(0.0f, 0.5f, -0.5f));

    Actor *sphere = new Actor("Sphere");
    auto sphere_mesh = Mesh::CreateSphere(30, 30);
    sphere_mesh->set_material(Material::Create());
    auto sphere_material = sphere_mesh->material();
    sphere_material->specular_alpha_ = 1.0f;
    sphere->AddModelComponent();
    sphere->GetModelComponent()->set_model(Model::Create({sphere_mesh}));
    sphere->GetTransformComponent()->set_position(glm::vec3(1.0f, 1.0f, 0.0f));
    sphere->GetTransformComponent()->set_scale(glm::vec3(0.6f));

    Actor *backpack = new Actor("Backpack");
    backpack->AddModelComponent();
    backpack->GetModelComponent()->set_model(Model::Load(resource_path + "\\model\\backpack.obj"));
    backpack->GetTransformComponent()->set_scale(glm::vec3(0.1f, 0.1f, 0.1f));
    backpack->GetTransformComponent()->set_position(glm::vec3(-1.0f, 0.22f, -0.5f));
    backpack->GetTransformComponent()->set_rotation(glm::vec3(0.0f, 50.0f, 0.0f));

    Actor *light_0 = new Actor("Light 1");
    light_0->AddLightComponent();
    light_0->AddModelComponent();
    auto red = Mesh::CreateSphere(30, 30);
    red->set_material(Material::Create());
    red->material()->set_texture(
        TextureType::EMISSIVE,
        Texture::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(0.95f, 0.95f, 0.95f, 1.0f)).get()));
    light_0->GetModelComponent()->set_model(Model::Create({red}));
    light_0->GetTransformComponent()->set_position(glm::vec3(0.0f, 2.5f, 0.0f));
    light_0->GetTransformComponent()->set_scale(glm::vec3(0.035f));
    light_0->GetTransformComponent()->set_rotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    light_0->GetLightComponent()->set_type(LightType::SPOT);
    light_0->GetLightComponent()->set_use_shadow(true);
    light_0->GetLightComponent()->set_color(glm::vec3(0.95f, 0.95f, 0.95f));

    Actor *light_2 = new Actor("Light 2");
    light_2->SetComponent(std::shared_ptr<Component>(new LightTT()));
    light_2->AddLightComponent();
    light_2->AddModelComponent();
    auto green = Mesh::CreateSphere(30, 30);
    green->set_material(Material::Create());
    green->material()->set_texture(
        TextureType::EMISSIVE,
        Texture::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(0.95f, 0.95f, 0.75f, 1.0f)).get()));
    light_2->GetModelComponent()->set_model(Model::Create({green}));
    light_2->GetLightComponent()->set_type(LightType::SPOT);
    light_2->GetLightComponent()->set_color(glm::vec3(0.95f, 0.95f, 0.75f));
    light_2->GetLightComponent()->set_use_shadow(true);
    light_2->GetTransformComponent()->set_rotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    light_2->GetTransformComponent()->set_position(glm::vec3(1.0f, 2.5f, 0.0f));
    light_2->GetTransformComponent()->set_scale(glm::vec3(0.035f));

    Actor *cam = new Actor("Camera man");
    cam->SetComponent(std::shared_ptr<Component>(new CameraTransformComponent()));
    cam->AddCameraComponent();

    auto layer00 = level->AddLayer("object");
    layer00->AddActor(box);
    layer00->AddActor(sphere);
    layer00->AddActor(floor);
    layer00->AddActor(backpack);
    auto layer01 = level->AddLayer("player");
    layer01->AddActor(cam);
    auto layer02 = level->AddLayer("light");
    layer02->AddActor(light_0);
    layer02->AddActor(light_2);

    auto layer03 = level->AddLayer("skybox");
    Actor *skybox = new Actor("Skybox");
    auto cubeRight = Image::Load("../../asset/cube_texture/0/right.jpg", false);
    auto cubeLeft = Image::Load("../../asset/cube_texture/0/left.jpg", false);
    auto cubeTop = Image::Load("../../asset/cube_texture/0/top.jpg", false);
    auto cubeBottom = Image::Load("../../asset/cube_texture/0/bottom.jpg", false);
    auto cubeFront = Image::Load("../../asset/cube_texture/0/front.jpg", false);
    auto cubeBack = Image::Load("../../asset/cube_texture/0/back.jpg", false);
    auto cube_texture = CubeTexture::Create({
        cubeRight.get(),
        cubeLeft.get(),
        cubeTop.get(),
        cubeBottom.get(),
        cubeFront.get(),
        cubeBack.get(),
    });
    skybox->AddSkyboxComponent();
    skybox->GetSkyboxComponent()->set_cube_texture(cube_texture);
    skybox->GetTransformComponent()->set_scale(glm::vec3(200.0f));
    layer03->AddActor(skybox);

    LevelManager::GetInstance()->SetCurrentLevel(level->name());

    LevelManager::GetInstance()->AddLevel("main");
}
