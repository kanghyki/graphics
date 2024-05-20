#include "model_component.h"
#include "opengl_device.h"
#include "transform_component.h"

ModelComponent::ModelComponent() : RenderComponent(ComponentType::MODEL)
{
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::Render(ProgramPtr program)
{
    // TODO:
    if (!model_)
    {
        return;
    }
    OpenGLDevice::GetInstance()->matrices_ubo()->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_model), sizeof(glm::mat4),
                    glm::value_ptr(GetTransformComponent()->transform().CalcModelMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    model()->Draw(program.get());
}
