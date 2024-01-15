#include <Xeph2D.h>

namespace Xeph2D
{
	class TestScript : public Component
	{
		SpriteRenderer* renderer = nullptr;
		AudioSource* audioSource = nullptr;
		Rigidbody* _rigidbody = nullptr;
		float speed = 10.f;
		float jumpVelocity = 15.f;
		Vector2 moveAxis = {};
		bool _showDebug = true;

	public:
		COMP_HEADER_STD(0x00000019);

		void Start() override;
		void Update() override;
		void DebugDraw() override;
		void OnDestroy() override;

	private:
		void MoveInput(InputAction* ctx);
		void OnSpace(InputAction* ctx);

		void OnCollisionEnter(Rigidbody* other);
	};

}
