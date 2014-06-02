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

#include <SFML/Graphics.hpp>

#ifndef TEXTUREHOLDER_HPP
#define	TEXTUREHOLDER_HPP

namespace sago {

class SagoDataHolder {
public:
	SagoDataHolder();
	const sf::Texture* getTexturePtr(const std::string &textureName) const;
	const sf::Font* getFontPtr(const std::string &fontName) const;
	virtual ~SagoDataHolder();
private:
	SagoDataHolder(const SagoDataHolder& base) = delete;
    SagoDataHolder& operator=(const SagoDataHolder& base) = delete;
	struct SagoDataHolderData;
	mutable SagoDataHolderData *data;
};

} //namespace sago

#endif	/* TEXTUREHOLDER_HPP */

