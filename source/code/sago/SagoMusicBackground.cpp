/* 
 * File:   SagoMusicBackground.cpp
 * Author: poul
 * 
 * Created on 3. juni 2014, 18:26
 */

#include "SagoMusicBackground.hpp"

namespace {
	std::string currentMusic;
	sf::Music* themusic = nullptr;
	sago::SagoDataHolder *dataHolder;
}

namespace sago {
	namespace music {
		
		void SetDataHolder(SagoDataHolder &texHolder) {
			dataHolder = &texHolder;
		}

		bool isPlaying() {
			if (themusic) {
				return themusic->getStatus() == sf::Music::Playing;
			}
			return false;
		}

		std::string getCurrentMusic() {
			return currentMusic;
		}

		void Play(const std::string& name) {
			if (name != currentMusic) {
				currentMusic = name;
				themusic = dataHolder->getMusicPtr(name);
				themusic->setLoop(true);
				themusic->play();
			}
		}

		void Stop() {
			if(themusic) {
				themusic->stop();
				themusic = nullptr;
				currentMusic = "";
			}
		}
	}
}