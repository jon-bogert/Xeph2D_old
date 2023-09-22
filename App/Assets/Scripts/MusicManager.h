#include <Xeph2D.h>

namespace Xeph2D
{
	class MusicManager : public Component
	{
		AudioSource* audio = nullptr;
	public:
		COMP_HEADER_STD(27);
		
		void Start() override;
	};
}
