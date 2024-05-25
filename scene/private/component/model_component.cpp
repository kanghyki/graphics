#include "model_component.h"
#include "renderer.h"
#include "transform_component.h"

ModelComponent::ModelComponent() : RenderComponent(ComponentType::MODEL)
{
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::Render(ProgramPtr program)
{
    if (!model_)
    {
        return;
    }
    Renderer::GetInstance()->GetUBO(UBOType::MATRICES)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_model), sizeof(glm::mat4),
                    glm::value_ptr(GetTransformComponent()->transform().CalcModelMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    model_->Draw(program.get());
}
