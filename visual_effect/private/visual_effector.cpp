#include "opengl_device.h"
#include "visual_effector.h"

VisualEffector *VisualEffector::instance_ = nullptr;

VisualEffector *VisualEffector::GetInstance()
{
    if (!instance_)
    {
        instance_ = new VisualEffector();
    }
    return instance_;
}

VisualEffector::VisualEffector()
{
}

VisualEffector::~VisualEffector()
{
}

void VisualEffector::Render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    OpenGLDevice::GetInstance()->SwapBuffer();
}
