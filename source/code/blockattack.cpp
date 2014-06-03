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
#include "sago/SagoDataHolder.hpp"
#include "sago/SagoMenu.hpp"
#include "sago/SagoCommandQueue.hpp"
#include "sago/SagoSprite.hpp"
#include "sago/SagoSpriteHolder.hpp"
#include "sago/FrameCounter.hpp"
#include "sago/SagoMusicBackground.hpp"
#include <SFML/Graphics.hpp>
#include <physfs.h>
#include <iostream>
#include <vector>
#include <unistd.h>

using namespace std;

namespace {
	void SetStandardKeyBinds(sago::SagoCommandQueue &cmdQ) {
		cmdQ.BindKey(sf::Keyboard::Up,"UP");
		cmdQ.BindKeyCommand("UP","UP");
		cmdQ.BindKey(sf::Keyboard::Down,"DOWN");
		cmdQ.BindKeyCommand("DOWN","DOWN");
		cmdQ.BindKey(sf::Keyboard::Return,"RETURN");
		cmdQ.BindKeyCommand("RETURN","CONFIRM");
		cmdQ.BindKey(sf::Keyboard::Escape,"ESC");
		cmdQ.BindKeyCommand("ESC","BACK");
	}
}

int main(int argc, const char* argv[])
{
	//Init the file system abstraction layer
	PHYSFS_init(argv[0]);
	PHYSFS_addToSearchPath(PHYSFS_getBaseDir(),0);
	sago::SagoDataHolder dataHolder;
	sago::SagoSpriteHolder spriteHolder(dataHolder);
	sago::SagoCommandQueue cmdQ;
	SetStandardKeyBinds(cmdQ);
	BlockMenu menu(dataHolder);
	sago::music::SetDataHolder(dataHolder);
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Block Attack - Rise of the blocks");
	sf::Clock clock;  //start the clock
	sf::Int32 lastFrameTime = 0;
	sago::FrameCounter fc(dataHolder);
	while (window.isOpen()) {
		sf::Int32 frameTime = clock.getElapsedTime().asMilliseconds();
		sf::Int32 deltaTime = frameTime - lastFrameTime;
		float fDeltaTime = static_cast<float>(deltaTime);
		lastFrameTime = frameTime;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		cmdQ.ReadKeysAndAddCommands();
		menu.ReadEvents(cmdQ);
		for (size_t i = 0; i < cmdQ.GetCommandQueue().size();i++) {
			std::string cmd = cmdQ.GetCommandQueue().at(i);
			if (cmd == "QUIT") {
				window.close();
			}
		}
		cmdQ.ClearCommands();
		window.clear();
		spriteHolder.GetSprite("background").Draw(window,frameTime,0,0);
		menu.DrawMenu(window);
		fc.Draw(window,frameTime);
		window.display();
		sago::music::Play("bgmusic");
		usleep(10000);
	}
	PHYSFS_deinit();
	return 0;
}
