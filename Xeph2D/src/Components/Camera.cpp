#include "Components/Camera.h"

#include "Systems/WindowManager.h"
#include "Systems/Serializer.h"

using namespace Xeph2D;

void Camera::Serializables()
{
	SERIALIZE_DEFAULT;
}

void Camera::Awake()
{
	WindowManager::SetCamera(this);
}

void Camera::OnDestroy()
{
	WindowManager::ClearCamera(this);
}