#include "Component.h"

#include "GameObject.h"

using namespace Xeph2D;

void Component::SetEnabled(const bool enabled)
{
	this->enabled = enabled;
	if (enabled)
		OnEnable();
	else
		OnDisable();
}
