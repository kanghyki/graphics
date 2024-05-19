#include "mesh_component.h"
#include "opengl_device.h"
#include "transform_component.h"

MeshComponent::MeshComponent() : RenderComponent(ComponentType::MESH)
{
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Render(ProgramPtr program)
{
    // TODO:
    if (!mesh_)
    {
        return;
    }
    OpenGLDevice::GetInstance()->matrices_ubo()->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_model), sizeof(glm::mat4),
                    glm::value_ptr(GetTransformComponent()->transform().CalcModelMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    mesh()->Draw(program.get());
}
