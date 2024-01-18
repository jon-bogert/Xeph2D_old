#pragma once
#include <any>
#include <SFML.hpp>

namespace Xeph2D
{
	class AudioData
	{
	public:
		AudioData() = default;
		virtual ~AudioData() = default;

		bool LoadAssetData(const std::string& filepath);
		bool LoadAssetData(const std::string& filepath, bool isStreamed);

		void SetIsStreamed(const bool isStreamed);
		bool GetIsStreamed() const;
		
		sf::SoundBuffer* GetBuffer() const;
		sf::Music* GetStream() const;

	protected:
		std::any m_source;
		bool m_isStreamed = true;
		std::string m_filepath = "";
	};
}