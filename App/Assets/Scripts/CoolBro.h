#include <Xeph2D.h>

namespace Xeph2D
{
	class CoolBro : public Component
	{
	public:
		COMP_HEADER_STD(0xb16265c9);
		
		void Start() override;
		void Update() override;
	};
}
