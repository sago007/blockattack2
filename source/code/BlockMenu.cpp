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

#include "BlockMenu.hpp"
#include "sago/SagoMusicBackground.hpp"

BlockMenu::BlockMenu(const sago::SagoDataHolder &texHolder) {
	sf::Text menutext;
	menutext.setFont(*texHolder.getFontPtr("PenguinAttack"));
	menutext.setCharacterSize(24);
	menutext.setColor(sf::Color::White);
	sago::menu::SagoMenu topmenu(menutext,false);
	sago::menu::SagoMenuButton b;
	b.SetLabel("Single player - endless");
	b.SetCommand("SP_ENDLESS");
	topmenu.addButton(b);
	b.SetLabel("Single player - time trial");
	b.SetCommand("SP_TT");
	topmenu.addButton(b);
	stack.PushMenu(topmenu);
	sago::music::Play("bgmusic");
}

bool BlockMenu::IsActive() {
	return true;
}

bool BlockMenu::IsBlockingDraw() {
	return true;
}

bool BlockMenu::IsBlockingUpdate() {
	return true;
}

void BlockMenu::Draw(sf::RenderWindow &target) {
	this->stack.DrawMenu(target);
}


void BlockMenu::ReadEvents(const sago::SagoCommandQueue &cmdQ) {
	for (size_t i = 0; i < cmdQ.GetCommandQueue().size();i++) {
			std::string cmd = cmdQ.GetCommandQueue().at(i);
		if (cmd == "POP_MENU") {
			stack.PopMenu();
		}
		if (cmd == "UP") {
			stack.Up();
		}
		if (cmd == "DOWN") {
			stack.Down();
		}
		if (cmd == "CONFIRM") {
			stack.Action(cmdQ,outQueue);
		}
	}
}

void BlockMenu::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	ReadEvents(input);
}

void BlockMenu::UpdateCommandQueue(sago::SagoCommandQueue& inout) {
	for (std::string item : this->outQueue) {
		inout.PushCommand(item);
	}
	this->outQueue.clear();
}