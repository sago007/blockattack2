/*
===========================================================================
blockattack - Block Attack - Rise of the Blocks
Copyright (C) 2014 Poul Sander

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

Source information and contacts persons can be found at
http://blockattack.sf.net
===========================================================================
*/

#ifndef SAGOSPRITE_HPP
#define	SAGOSPRITE_HPP

#include "SagoDataHolder.hpp"

namespace sago {

class SagoSprite {
public:
	SagoSprite(const SagoDataHolder &texHolder, const std::string &texture,const sf::IntRect &initImage,const int animationFrames, const int animationFrameLength);
	void Draw(sf::RenderWindow &target, sf::Int32 frameTime, float x, float y) const;
	void Draw(sf::RenderWindow &target, sf::Int32 frameTime, float x, float y,const sf::IntRect &part) const;
	void SetOrigin(const sf::Vector2i &newOrigin);
	virtual ~SagoSprite();
private:
	SagoSprite(const SagoSprite& base) = delete;
    SagoSprite& operator=(const SagoSprite& base) = delete;
	struct SagoSpriteData;
	SagoSpriteData *data;
};

}

#endif	/* SAGOSPRITE_HPP */

