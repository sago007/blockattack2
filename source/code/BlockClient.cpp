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

#include "BlockClient.hpp"
#include "sago/SagoSpriteHolder.hpp"
#include <memory>

struct BlockClient::BlockClientData  { 
	bool active = true;
	const sago::SagoDataHolder *dataHolder;
	std::shared_ptr<sago::SagoSpriteHolder> sprites;
	const sago::SagoSprite *exitSprite;
	sf::Rect<int> exitButton = sf::Rect<int>(0,0,0,0);
	float fTime = 0.0;
	bool mousePressed = true;
};

BlockClient::BlockClient(const sago::SagoDataHolder &dHolder) : data(new BlockClientData()) {
	data->dataHolder = &dHolder;
	data->sprites = std::shared_ptr<sago::SagoSpriteHolder>(new sago::SagoSpriteHolder(*(data->dataHolder)));
	data->sprites->ReadSprites();
	data->exitSprite = &data->sprites->GetSprite("bexit");
}

BlockClient::~BlockClient() {
	delete this->data;
}

bool BlockClient::IsActive() {
	return this->data->active;
}

bool BlockClient::IsBlockingDraw() {
	return true;
}

bool BlockClient::IsBlockingUpdate() {
	return true;
}

void BlockClient::Draw(sf::RenderWindow &target) {
	data->exitButton = sf::Rect<int>(target.getSize().x-150,target.getSize().y-150,target.getSize().x-50,target.getSize().y-50);
	data->exitSprite->Draw(target,data->fTime,data->exitButton.left,data->exitButton.top);
}

void BlockClient::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	data->fTime += fDeltaTime;
	for (size_t i = 0; i < input.GetCommandQueue().size();i++) {
		std::string cmd = input.GetCommandQueue().at(i);
		if (cmd == "BACK") {
			this->data->active = false;
		}
	}
	if (!this->data->mousePressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (data->exitButton.contains(input.GetMousePosition())) {
			this->data->active = false;
		}
	}
	this->data->mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}
void BlockClient::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	
}
	
