#include "resource.h"

uint32_t Resource::kId = 0;

Resource::Resource() : id_(kId++)
{
}

Resource::~Resource()
{
}
