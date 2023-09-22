#include "GameObject.h"
#include "AudioData.h"

namespace Xeph2D
{
	enum class AudioStatus { Stopped, Paused, Playing };
	class AudioSource : public Component
	{
		std::unique_ptr<sf::Sound> _sound = nullptr;
		AudioData* _data = nullptr;


	public:
		COMP_HEADER_STD(3);

		void LoadAudioByTag(const std::string& tag);

		void Play();
		void Pause();
		void Stop();

		AudioStatus Status();

	private:
		bool IsStreamed();
		void SoundSetup();
	};
}
