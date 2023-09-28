#include <Xeph2D.h>

namespace Xeph2D
{
	class MusicManager : public Component
	{
		AudioSource* audio = nullptr;
	public:
		COMP_HEADER_STD(0x0000001b);
		
		void Start() override;
	};
}
