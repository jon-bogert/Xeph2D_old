/* ==================================================================================
* MIT License
*
* Copyright (c) 2023 Jon Bogert
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
================================================================================== */

#include "Systems/InputSystem/InputSystem.h"

#include "GamepadHandler.h"
#include "KeyHandler.h"

using namespace Xeph2D;

namespace
{
	POINT WindowCenter(HWND hwnd)
	{
		POINT pnt;
		RECT windowRect;
		GetWindowRect(hwnd, &windowRect);
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		int halfTitleBarHeight = (windowRect.bottom - windowRect.top - clientRect.bottom - clientRect.top) * 0.5f;
		// Calculate the center position
		pnt.x = (int)(windowRect.left + (windowRect.right - windowRect.left) * 0.5f);
		pnt.y = (int)(windowRect.top + (windowRect.bottom - windowRect.top) * 0.5f + halfTitleBarHeight);
		return pnt;
	}
}

Xeph2D::InputSystem::InputSystem()
{
	_gamepadHandler = new GamepadHandler();
	_keyHandler = new KeyHandler();
	GetCursorPos(&_mousePos);
}

InputSystem& Xeph2D::InputSystem::Get()
{
	static InputSystem inst;
	return inst;
}

Xeph2D::InputSystem::~InputSystem()
{
	delete _keyHandler;
	delete _gamepadHandler;
}

//================================================================================
//                       STATIC INTERFACES
//================================================================================


void Xeph2D::InputSystem::Initialize(HWND& hwnd)
{
	Get()._Initialize(hwnd);
}

void Xeph2D::InputSystem::Update()
{
	Get()._Update();
}

bool Xeph2D::InputSystem::IsControllerConnected(const uint8_t num)
{
	return Get()._gamepadHandler->IsControllerConnected(num);
}

bool Xeph2D::InputSystem::GetGamepadHold(Gamepad::Button button, uint8_t num)
{
	return Get()._gamepadHandler->GetGamepadHold(button, num);
}

bool Xeph2D::InputSystem::GetGamepadDown(Gamepad::Button button, uint8_t num)
{
	return Get()._gamepadHandler->GetGamepadDown(button, num);
}

bool Xeph2D::InputSystem::GetGamepadUp(Gamepad::Button button, uint8_t num)
{
	return Get()._gamepadHandler->GetGamepadUp(button, num);
}

void Xeph2D::InputSystem::GetGamepadAxis(float* out_v2, Gamepad::Axis axis, uint8_t num)
{
	Get()._gamepadHandler->GetGamepadAxis(out_v2, axis, num);
}

float Xeph2D::InputSystem::GetTriggerThreshold()
{
	return Get()._gamepadHandler->GetTriggerThreshold();
}

void Xeph2D::InputSystem::SetTriggerThreshold(const float threshold)
{
	Get()._gamepadHandler->SetTriggerThreshold(threshold);
}

bool Xeph2D::InputSystem::GetKeyHold(Key keycode)
{
	return Get()._keyHandler->GetKeyHold(keycode);
}

bool Xeph2D::InputSystem::GetKeyDown(Key keycode)
{
	return Get()._keyHandler->GetKeyDown(keycode);
}

bool Xeph2D::InputSystem::GetKeyUp(Key keycode)
{
	return Get()._keyHandler->GetKeyUp(keycode);
}

bool Xeph2D::InputSystem::GetMouseHold(Mouse::Button btncode)
{
	return Get()._keyHandler->GetMouseHold(btncode);
}

bool Xeph2D::InputSystem::GetMouseDown(Mouse::Button btncode)
{
	return Get()._keyHandler->GetMouseDown(btncode);
}

bool Xeph2D::InputSystem::GetMouseUp(Mouse::Button btncode)
{
	return Get()._keyHandler->GetMouseUp(btncode);
}

float Xeph2D::InputSystem::GetKeyAxisComposite1D(Key negative, Key positive)
{
	return Get()._keyHandler->GetKeyAxisComposite1D(negative, positive);
}

void Xeph2D::InputSystem::GetKeyAxisComposite2D(float* out_v2, Key negX, Key posX, Key negY, Key posY)
{
	return Get()._keyHandler->GetKeyAxisComposite2D(out_v2, negX, posX, negY, posY);
}

void Xeph2D::InputSystem::GetMousePos(float* out_v2, bool relativeToWindow)
{
	Get()._GetMousePos(out_v2, relativeToWindow);
}

void Xeph2D::InputSystem::GetMouseDelta(float* out_v2)
{
	out_v2[0] = Get()._mouseDelta[0];
	out_v2[1] = Get()._mouseDelta[1];
}

void Xeph2D::InputSystem::SetCaptureMouse(const bool captureMouse)
{
	if (!Get()._hwnd) return;
	Get()._captureMouse = captureMouse;
}

bool Xeph2D::InputSystem::GetCaptureMouse()
{
	return Get()._captureMouse;
}

bool Xeph2D::InputSystem::MouseOverWindow()
{
	if (!Get()._hwnd) return false;
	float pos[2];
	RECT clientRect;
	GetMousePos(pos, true);
	GetClientRect(Get()._hwnd, &clientRect);
	return !(pos[0] < 0 || pos[0] >= (float)clientRect.bottom || pos[1] < 0 || pos[1] >= (float)clientRect.right);
}

Xeph2D::InputActionMap* Xeph2D::InputSystem::CreateInputActionMap(std::string name)
{
	InputActionMap* map = Get()._actionMaps.emplace_back(std::make_unique<InputActionMap>()).get();
	map->name = name;
	return map;
}

InputActionMap* Xeph2D::InputSystem::FindInputActionMap(std::string name)
{
	auto it = std::find_if(Get()._actionMaps.begin(), Get()._actionMaps.end(), [=](const std::unique_ptr<InputActionMap>& x) {return x->name == name; });
	return (it == Get()._actionMaps.end()) ? nullptr : it->get();
}

//================================================================================
//                       IMPLEMENTATION
//================================================================================


void Xeph2D::InputSystem::_Initialize(HWND& hwnd)
{
	_hwnd = hwnd;
	_gamepadHandler->Initialize();
}

void Xeph2D::InputSystem::_Update()
{
	POINT newPos;
	GetCursorPos(&newPos);
	if (_captureMouse)
	{
		if (_hwnd == nullptr) return; // Cannot capture mouse while window is null
		POINT center = WindowCenter(_hwnd);

		_mouseDelta[0] = newPos.x - center.x;
		_mouseDelta[1] = newPos.y - center.y;
		SetCursorPos(center.x, center.y);
	}
	else
	{
		_mouseDelta[0] = newPos.x - _mousePos.x;
		_mouseDelta[1] = newPos.y - _mousePos.y;
		_mousePos = newPos;
	}

	_gamepadHandler->Update();
	_keyHandler->Update();
	for (auto& map : _actionMaps)
		map->Update();
}

void Xeph2D::InputSystem::_GetMousePos(float* out_v2, bool relativeToWindow)
{
	if (_hwnd && relativeToWindow)
	{
		POINT copy = _mousePos;
		ScreenToClient(_hwnd, &copy);
		out_v2[0] = copy.x;
		out_v2[1] = copy.y;
	}
	else
	{
		out_v2[0] = _mousePos.x;
		out_v2[1] = _mousePos.y;
	}

}
