#include <Xeph2D.h>

namespace Xeph2D
{
	class TestScript : public Component
	{
		SpriteRenderer* renderer = nullptr;
		AudioSource* audioSource = nullptr;
		float speed = 10.f;
		Vector2 moveAxis = {};

	public:
		COMP_HEADER_STD(25);
		
		void Start() override;
		void Update() override;

	private:
		void MoveInput(InputAction* ctx);
		void OnSpace(InputAction* ctx);
	};

}
