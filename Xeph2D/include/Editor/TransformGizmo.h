#pragma once
#ifdef _EDITOR

namespace Xeph2D
{
	class GameObject;
	struct Vector2;
}

namespace Xeph2D::Edit
{
	class TransformGizmo
	{
	public:
		enum class Mode { Position, Rotation, Scale};
		Mode _mode = Mode::Position;
		float _radius = 100.f;
		float _symbolWidth = 15.f;
		float _centerWidth = 25.f;

		bool _xSelected = false;
		bool _ySelected = false;

		bool _applyingTransform = false;

		GameObject* _currentObject = nullptr;

		void UpdateMouse(const Vector2& mousePos);
		void Draw();
		void SetCurrentObject(GameObject* gameObject);

		Mode GetMode() const;
		void SetMode(const Mode mode);

	private:
		void ApplyTransform();
	};
}

#endif //_EDITOR