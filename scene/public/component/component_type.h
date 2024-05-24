#ifndef INCLUDED_COMPONENT_TYPE_H
#define INCLUDED_COMPONENT_TYPE_H

enum class ComponentType
{
    TRANSFORM = 0,
    CAMERA,
    LIGHT,
    /* Render component */
    MODEL,
    SKYBOX,
    EOL
};

#endif
