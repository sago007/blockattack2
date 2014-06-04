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

#ifndef BLOCKMENU_HPP
#define	BLOCKMENU_HPP

#include "GameState.hpp"
#include "sago/SagoMenu.hpp"
#include "sago/SagoDataHolder.hpp"
#include <vector>

class BlockMenu : public GameState {
public:
	BlockMenu(const sago::SagoDataHolder &texHolder);
	bool IsActive();
	bool IsBlockingDraw();
	bool IsBlockingUpdate();
	void Draw(sf::RenderWindow &target);
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input);
	void UpdateCommandQueue(sago::SagoCommandQueue &inout);
private:
	void ReadEvents(const sago::SagoCommandQueue &queue);
	sago::menu::SagoMenuStack stack;
	std::vector<std::string> outQueue;
};

#endif	/* BLOCKMENU_HPP */

