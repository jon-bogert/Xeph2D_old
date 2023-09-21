#include <Xeph2D.h>

namespace Xeph2D
{
	class AudioSource : public Component
	{
	public:
		COMP_HEADER_STD(3);
		
		void Start() override;
		void Update() override;
	};
}
