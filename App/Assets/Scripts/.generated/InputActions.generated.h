#pragma once
#include <Xeph2D.h>

#define __X2D_LOAD_INPUTACTIONS &Xeph2D::Generated::LoadInputActions

namespace Xeph2D::Generated
{
	void LoadInputActions(InputSystem* inp)
	{
		InputActionMap* m_00000000 = inp->CreateInputActionMap("Player");
		InputAction* a_00000000 = m_00000000->CreateAction("Camera", InputAction::Type::Axis2D);
		a_00000000->Add2DAxis(Key::Left, Key::Right, Key::Down, Key::Up);

		InputAction* a_00000001 = m_00000000->CreateAction("Move", InputAction::Type::Axis2D);
		a_00000001->Add2DAxis(Gamepad::Axis::LS);
		a_00000001->Add2DAxis(Key::A, Key::D, Key::S, Key::W);

		InputAction* a_00000002 = m_00000000->CreateAction("Space");
		a_00000002->AddButton(Key::Space);
	}
}