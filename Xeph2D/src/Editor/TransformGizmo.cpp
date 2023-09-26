#ifdef _EDITOR
#include "Editor/Editor.h"
#include "Editor/TransformGizmo.h"
#include "Systems/Debug.h"
#include "Systems/Serializer.h"
#include "Systems/WindowManager.h"

#include <cmath>

#define __ROT_SPEED 3.f

using namespace Xeph2D::Edit;

void Xeph2D::Edit::TransformGizmo::UpdateMouse(const Vector2& mousePos)
{
	if (_currentObject == nullptr)
		return;

	Vector2 position = WindowManager::WorldToPixel(_currentObject->transform.position);
	
	if (!_applyingTransform && (_mode == Mode::Position || _mode == Mode::Scale))
	{
		//both Square
		if (mousePos.x >= position.x && mousePos.x <= position.x + _centerWidth &&
			mousePos.y <= position.y && mousePos.y >= position.y - _centerWidth)
		{
			_xSelected = true;
			_ySelected = true;
		}
		// X line
		else if (mousePos.x >= position.x + _centerWidth && mousePos.x <= position.x + _radius + _symbolWidth &&
			mousePos.y <= position.y + _centerWidth * 0.5f && mousePos.y >= position.y - _centerWidth * 0.5f)
		{
			_xSelected = true;
			_ySelected = false;
		}
		// Y line
		else if (mousePos.x <= position.x + _centerWidth * 0.5f && mousePos.x >= position.x - _centerWidth * 0.5f &&
			mousePos.y <= position.y - _centerWidth  && mousePos.y >= position.y - _radius - _symbolWidth)
		{
			_xSelected = false;
			_ySelected = true;
		}
		else
		{
			_xSelected = false;
			_ySelected = false;
		}
	}
	else if (_mode == Mode::Rotation)
	{
		float mag = Math::Magnitude(position - mousePos);
		if (mag < _radius + _symbolWidth && mag > _radius - _symbolWidth)
		{
			_xSelected = true;
			_ySelected = true;
		}
		else
		{
			_xSelected = false;
			_ySelected = false;
		}
	}

	if ((_xSelected || _ySelected) &&
		(InputSystem::GetMouseDown(Mouse::Button::Left) && !InputSystem::GetKeyHold(Key::LAlt)))
	{
		_applyingTransform = true;
	}
	else if (InputSystem::GetMouseUp(Mouse::Button::Left))
	{
		_applyingTransform = false;
	}

	ApplyTransform();
}

void TransformGizmo::Draw()
{
	if (_currentObject == nullptr)
		return;

	Debug::VertexChain vertChain;
	Vector2 out;
	Vector2 cwUnit = { WindowManager::PixelToUnit(_centerWidth), WindowManager::PixelToUnit(_centerWidth) };
	float swUnit = WindowManager::PixelToUnit(_symbolWidth);
	Color xColor = (_xSelected) ? Color::Yellow : Color::Red;
	Color yColor = (_ySelected) ? Color::Yellow : Color::Green;
	Color zColor = (_xSelected || _ySelected) ? Color::Yellow : Color::Blue;
	Color bothColor = (_xSelected && _ySelected) ? Color::Yellow : Color::Blue;
	bothColor.a = 0.25f;

	switch (_mode)
	{
	case Mode::Position:
		vertChain.push_back(_currentObject->transform.position);
		out = WindowManager::PixelToUnit({ _radius, 0.f });
		vertChain.push_back(_currentObject->transform.position + out);
		vertChain.push_back(_currentObject->transform.position + out + Vector2(0.f, swUnit * 0.5f));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(swUnit, 0.f));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(0.f, -swUnit * 0.5f));
		vertChain.push_back(_currentObject->transform.position + out);
		Debug::DrawChainLine(vertChain, xColor);

		vertChain.clear();
		vertChain.push_back(_currentObject->transform.position);
		out = WindowManager::PixelToUnit({ 0.f, _radius });
		vertChain.push_back(_currentObject->transform.position + out);
		vertChain.push_back(_currentObject->transform.position + out + Vector2(-swUnit * 0.5f, 0.f));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(0.f, swUnit));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(swUnit * 0.5f, 0.f));
		vertChain.push_back(_currentObject->transform.position + out);
		Debug::DrawChainLine(vertChain, yColor);

		Debug::DrawBoxFilled(_currentObject->transform.position + cwUnit * 0.5f, cwUnit, bothColor);
		break;
	case Mode::Rotation:
		Debug::DrawCircleOutline(_currentObject->transform.position, WindowManager::PixelToUnit(_radius), zColor);
		out = Vector2(std::cosf(_currentObject->transform.rotation.GetRad()), -std::sinf(_currentObject->transform.rotation.GetRad())) * WindowManager::PixelToUnit(_radius);
		Debug::DrawLine(_currentObject->transform.position, _currentObject->transform.position + out, zColor);
		break;
	case Mode::Scale:
		vertChain.push_back(_currentObject->transform.position);
		out = WindowManager::PixelToUnit({ _radius, 0.f });
		vertChain.push_back(_currentObject->transform.position + out);
		vertChain.push_back(_currentObject->transform.position + out + Vector2(0.f, swUnit * 0.5f));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(swUnit, swUnit * 0.5f));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(swUnit, -swUnit * 0.5f));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(0.f, -swUnit * 0.5f));
		vertChain.push_back(_currentObject->transform.position + out);
		Debug::DrawChainLine(vertChain, xColor);

		vertChain.clear();
		vertChain.push_back(_currentObject->transform.position);
		out = WindowManager::PixelToUnit({ 0.f, _radius });
		vertChain.push_back(_currentObject->transform.position + out);
		vertChain.push_back(_currentObject->transform.position + out + Vector2(-swUnit * 0.5f, 0.f));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(-swUnit * 0.5f, swUnit));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(swUnit * 0.5f, swUnit));
		vertChain.push_back(_currentObject->transform.position + out + Vector2(swUnit * 0.5f, 0.f));
		vertChain.push_back(_currentObject->transform.position + out);
		Debug::DrawChainLine(vertChain, yColor);

		Debug::DrawBoxFilled(_currentObject->transform.position + cwUnit * 0.5f, cwUnit, bothColor);
		break;
	default:
		Debug::LogErr("TransfromGizmo::Draw -> Enum type not supported");
		break;
	}
}

void TransformGizmo::SetCurrentObject(GameObject* gameObject)
{
	_currentObject = gameObject;
}

TransformGizmo::Mode TransformGizmo::GetMode() const
{
	return _mode;
}

void TransformGizmo::SetMode(const Mode mode)
{
	_mode = mode;
}

void Xeph2D::Edit::TransformGizmo::ApplyTransform()
{
	if (!_applyingTransform)
		return;

	Vector2 mouseDelta{};
	InputSystem::GetMouseDelta(&mouseDelta.x);
	mouseDelta = WindowManager::PixelToUnit(mouseDelta);
	mouseDelta.y *= -1;

	float scaleBoth = (mouseDelta.x + mouseDelta.y) * 0.5f;
	Vector2 globalMousePos{};

	switch (_mode)
	{
	case Mode::Position:
		if (_xSelected)
			_currentObject->transform.position.x += mouseDelta.x;
		if (_ySelected)
			_currentObject->transform.position.y += mouseDelta.y;
		break;
	case Mode::Rotation:
		InputSystem::GetMousePos(&globalMousePos.x, false);
		_currentObject->transform.rotation.AddDeg(mouseDelta.x * __ROT_SPEED);
		break;
	case Mode::Scale:
		if (_xSelected && _ySelected)
		{
			_currentObject->transform.scale += Vector2(scaleBoth, scaleBoth);
			break;
		}
		if (_xSelected)
			_currentObject->transform.scale.x += mouseDelta.x;
		if (_ySelected)
			_currentObject->transform.scale.y += mouseDelta.y;
		break;
	}
	for (Serializer::EdVarEntry& e : Serializer::GetDataFromInstance(_currentObject->instID)->go_variables)
	{
		if (e.name == "transform")
		{
			Editor::SetHasSaved(false);
			e.RefreshData<Transform>();
			break;
		}
	}
}

#endif // _EDITOR


