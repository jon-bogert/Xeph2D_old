#pragma once
#include <any>
#include <SFML.hpp>

namespace Xeph2D
{
	class AudioData
	{
	protected:
		std::any _source;
		bool _isStreamed = true;
		std::string _filepath = "";

	public:
		AudioData() = default;
		virtual ~AudioData() = default;

		void LoadAssetData(const std::string& filepath);
		void LoadAssetData(const std::string& filepath, bool isStreamed);

		void SetIsStreamed(const bool isStreamed);
		bool GetIsStreamed() const;
		
		sf::SoundBuffer* GetBuffer() const;
		sf::Music* GetStream() const;
	};
}