#include <Xeph2D.h>

namespace Xeph2D
{
	class CameraController : public Component
	{
		Vector2 moveAxis{};
		float _speed = 5.f;

	public:
		COMP_HEADER_STD(26);
		
		void Start() override;
		void Update() override;
		void OnDestroy() override;

	private:
		void MoveInput(InputAction* ctx);
	};
}
