#include "opengl_device.h"
#include "uniform_struct.h"

void Unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MatricesUniform::Sub() const
{
    OpenGLDevice::GetInstance()->matrices_ubo()->Bind();

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_view), sizeof(glm::vec4), glm::value_ptr(t_view));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_proj), sizeof(glm::vec4), glm::value_ptr(t_proj));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_model), sizeof(glm::vec4), glm::value_ptr(t_model));

    Unbind();
}

void MaterialUniform::Sub() const
{
    OpenGLDevice::GetInstance()->material_ubo()->Bind();

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_int_0), sizeof(int), &m_int_0);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_int_1), sizeof(int), &m_int_1);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_int_2), sizeof(int), &m_int_2);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_int_3), sizeof(int), &m_int_3);

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_float_0), sizeof(float), &m_float_0);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_float_1), sizeof(float), &m_float_1);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_float_2), sizeof(float), &m_float_2);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_float_3), sizeof(float), &m_float_3);

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec2_0), sizeof(glm::vec2),
                    glm::value_ptr(m_vec2_0));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec2_1), sizeof(glm::vec2),
                    glm::value_ptr(m_vec2_1));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec2_2), sizeof(glm::vec2),
                    glm::value_ptr(m_vec2_2));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec2_3), sizeof(glm::vec2),
                    glm::value_ptr(m_vec2_3));

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec4_0), sizeof(glm::vec4),
                    glm::value_ptr(m_vec4_0));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec4_1), sizeof(glm::vec4),
                    glm::value_ptr(m_vec4_1));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec4_2), sizeof(glm::vec4),
                    glm::value_ptr(m_vec4_2));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_vec4_3), sizeof(glm::vec4),
                    glm::value_ptr(m_vec4_3));

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_mat4_0), sizeof(glm::mat4),
                    glm::value_ptr(m_mat4_0));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_mat4_1), sizeof(glm::mat4),
                    glm::value_ptr(m_mat4_1));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_mat4_2), sizeof(glm::mat4),
                    glm::value_ptr(m_mat4_2));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MaterialUniform, m_mat4_3), sizeof(glm::mat4),
                    glm::value_ptr(m_mat4_3));

    Unbind();
}

void LightUniform::Sub() const
{
    OpenGLDevice::GetInstance()->lights_ubo()->Bind();

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, type), sizeof(int), &type);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, constant), sizeof(float), &constant);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, linear), sizeof(float), &linear);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, quadratic), sizeof(float), &quadratic);

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, position), sizeof(glm::vec3), glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, direction), sizeof(glm::vec3), glm::value_ptr(direction));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, cutoff), sizeof(glm::vec2), glm::value_ptr(cutoff));

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, ambient), sizeof(glm::vec3), glm::value_ptr(ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, diffuse), sizeof(glm::vec3), glm::value_ptr(diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightUniform, specular), sizeof(glm::vec3), glm::value_ptr(specular));

    Unbind();
}

void GlobalUniform::Sub() const
{
    OpenGLDevice::GetInstance()->global_ubo()->Bind();

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(GlobalUniform, g_resolution), sizeof(glm::vec2),
                    glm::value_ptr(g_resolution));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(GlobalUniform, g_time), sizeof(float), &g_time);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(GlobalUniform, g_delta_time), sizeof(float), &g_delta_time);

    Unbind();
}
