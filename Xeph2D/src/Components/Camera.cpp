#include "Components/Camera.h"

#include "Systems/WindowManager.h"

using namespace Xeph2D;

void Camera::Awake()
{
	WindowManager::SetCamera(this);
}

void Camera::OnDestroy()
{
	WindowManager::ClearCamera(this);
}