/* 
 * File:   SagoMusicBackground.hpp
 * Author: poul
 *
 * Created on 3. juni 2014, 18:26
 */

#ifndef SAGOMUSICBACKGROUND_HPP
#define	SAGOMUSICBACKGROUND_HPP

#include "SagoDataHolder.hpp"

namespace sago {
	namespace music {
		void SetDataHolder(sago::SagoDataHolder &texHolder);
		void Play(const std::string &name);
		void Stop();
        bool isPlaying();
	}
}

#endif	/* SAGOMUSICBACKGROUND_HPP */

