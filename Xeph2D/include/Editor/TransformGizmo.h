#pragma once
#ifdef _EDITOR

namespace Xeph2D
{
	class Transform;
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

		Transform* _currentTransform = nullptr;

		void Draw();
		void SetCurrentTransform(Transform* transform);

		Mode GetMode() const;
		void SetMode(const Mode mode);
	};
}

#endif //_EDITOR