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
    OpenGLDevice::GetInstance()->matrices_ubo()->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_model), sizeof(glm::mat4),
                    glm::value_ptr(GetTransformComponent()->transform().CalcModelMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    OpenGLDevice::GetInstance()->material_ubo()->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec4_0), sizeof(glm::vec4),
                    glm::value_ptr(glm::vec4(1.0f)));
    if (material_)
    {
        material_->SetToProgram(program.get());
    }
    mesh()->Draw();
}
