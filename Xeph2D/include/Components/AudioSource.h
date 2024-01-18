#include "GameObject.h"
#include "AudioData.h"

namespace Xeph2D
{
	enum class AudioStatus { Stopped, Paused, Playing };
	class AudioSource : public Component
	{
	public:
		COMP_HEADER_STD(0x00000003);

		void EditorInit() override;
		void Awake() override;
		
		void LoadAudioByTag(const std::string& tag);

		void Play();
		void Pause();
		void Stop();

		AudioStatus Status();

	private:
		bool IsStreamed();
		void SoundSetup();

		std::unique_ptr<sf::Sound> m_sound = nullptr;
		AudioData* m_data = nullptr;
		std::string m_audioDataKey = "";
	};
}
