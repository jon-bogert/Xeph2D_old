#ifdef _EDITOR
#include "Editor/TransformGizmo.h"
#include "Systems/Debug.h"
#include "Systems/WindowManager.h"

#include <cmath>

using namespace Xeph2D::Edit;

void TransformGizmo::Draw()
{
	if (_currentTransform == nullptr)
		return;

	Debug::VertexChain vertChain;
	Vector2 out;
	Vector2 cwUnit = { WindowManager::PixelToUnit(_centerWidth), WindowManager::PixelToUnit(_centerWidth) };
	float swUnit = WindowManager::PixelToUnit(_symbolWidth);
	Color clearBlue = Color::Blue;
	clearBlue.a = 0.25f;

	switch (_mode)
	{
	case Mode::Position:
		vertChain.push_back(_currentTransform->position);
		out = WindowManager::PixelToUnit({ _radius, 0.f });
		vertChain.push_back(_currentTransform->position + out);
		vertChain.push_back(_currentTransform->position + out + Vector2(0.f, swUnit * 0.5f));
		vertChain.push_back(_currentTransform->position + out + Vector2(swUnit, 0.f));
		vertChain.push_back(_currentTransform->position + out + Vector2(0.f, -swUnit * 0.5f));
		vertChain.push_back(_currentTransform->position + out);
		Debug::DrawChainLine(vertChain, Color::Red);

		vertChain.clear();
		vertChain.push_back(_currentTransform->position);
		out = WindowManager::PixelToUnit({ 0.f, _radius });
		vertChain.push_back(_currentTransform->position + out);
		vertChain.push_back(_currentTransform->position + out + Vector2(-swUnit * 0.5f, 0.f));
		vertChain.push_back(_currentTransform->position + out + Vector2(0.f, swUnit));
		vertChain.push_back(_currentTransform->position + out + Vector2(swUnit * 0.5f, 0.f));
		vertChain.push_back(_currentTransform->position + out);
		Debug::DrawChainLine(vertChain, Color::Green);

		Debug::DrawBoxFilled(_currentTransform->position + cwUnit * 0.5f, cwUnit, clearBlue);
		break;
	case Mode::Rotation:
		Debug::DrawCircleOutline(_currentTransform->position, WindowManager::PixelToUnit(_radius), Color::Blue);
		out = Vector2(std::cosf(_currentTransform->rotation.GetRad()), -std::sinf(_currentTransform->rotation.GetRad())) * WindowManager::PixelToUnit(_radius);
		Debug::DrawLine(_currentTransform->position, _currentTransform->position + out, Color::Blue);
		break;
	case Mode::Scale:
		vertChain.push_back(_currentTransform->position);
		out = WindowManager::PixelToUnit({ _radius, 0.f });
		vertChain.push_back(_currentTransform->position + out);
		vertChain.push_back(_currentTransform->position + out + Vector2(0.f, swUnit * 0.5f));
		vertChain.push_back(_currentTransform->position + out + Vector2(swUnit, swUnit * 0.5f));
		vertChain.push_back(_currentTransform->position + out + Vector2(swUnit, -swUnit * 0.5f));
		vertChain.push_back(_currentTransform->position + out + Vector2(0.f, -swUnit * 0.5f));
		vertChain.push_back(_currentTransform->position + out);
		Debug::DrawChainLine(vertChain, Color::Red);

		vertChain.clear();
		vertChain.push_back(_currentTransform->position);
		out = WindowManager::PixelToUnit({ 0.f, _radius });
		vertChain.push_back(_currentTransform->position + out);
		vertChain.push_back(_currentTransform->position + out + Vector2(-swUnit * 0.5f, 0.f));
		vertChain.push_back(_currentTransform->position + out + Vector2(-swUnit * 0.5f, swUnit));
		vertChain.push_back(_currentTransform->position + out + Vector2(swUnit * 0.5f, swUnit));
		vertChain.push_back(_currentTransform->position + out + Vector2(swUnit * 0.5f, 0.f));
		vertChain.push_back(_currentTransform->position + out);
		Debug::DrawChainLine(vertChain, Color::Green);

		Debug::DrawBoxFilled(_currentTransform->position + cwUnit * 0.5f, cwUnit, clearBlue);
		break;
	default:
		Debug::LogErr("TransfromGizmo::Draw -> Enum type not supported");
		break;
	}
}

void TransformGizmo::SetCurrentTransform(Transform* transform)
{
	_currentTransform = transform;
}

TransformGizmo::Mode TransformGizmo::GetMode() const
{
	return _mode;
}

void TransformGizmo::SetMode(const Mode mode)
{
	_mode = mode;
}

#endif // _EDITOR


