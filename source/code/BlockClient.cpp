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
#include "BlockGame.hpp"
#include "sago/SagoSpriteHolder.hpp"
#include <memory>

namespace {
	void DrawOneBlock(const SingleBlock &b, const sf::Rect<int> &place, int i, int j, const sago::SagoSpriteHolder &spHolder, float fTime, sf::RenderWindow &target) {
		if (b.type == b.Blue) {
			 const sago::SagoSprite &mySprite = spHolder.GetSprite("block_blue");
			 mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j);
		}
	}
	
	void DrawBlockGame(const BlockGame &g, const sf::Rect<int> &place, const sago::SagoSpriteHolder &spHolder, float fTime, sf::RenderWindow &target) {
		const sago::SagoSprite &background = spHolder.GetSprite("boardbackback");
		const sago::SagoSprite &cursor = spHolder.GetSprite("cursor");
		background.Draw(target,fTime,place.left,place.top);
		const auto &board = g.GetBoard();
		for (int i=0; i< g.coloms; i++) {
			for (int j=0;j< g.rows;j++) {
				DrawOneBlock(board[i][j], place,i,j,spHolder,fTime,target);
			}
		}
		int cursorx, cursory;
		g.GetCursor(cursorx,cursory);
		cursor.Draw(target,fTime,place.left+50*cursorx,place.top-50+50*12-50*cursory);
 	}
}  //anonymous namespace

struct BlockClient::BlockClientData  { 
	bool active = true;
	const sago::SagoDataHolder *dataHolder;
	std::shared_ptr<sago::SagoSpriteHolder> sprites;
	const sago::SagoSprite *exitSprite;
	sf::Rect<int> exitButton = sf::Rect<int>(0,0,0,0);
	float fTime = 0.0;
	bool mousePressed = true;
	BlockGame p1;
	sf::Rect<int> p1palce = sf::Rect<int>(200,150,300,600);
};

BlockClient::BlockClient(const sago::SagoDataHolder &dHolder) : data(new BlockClientData()) {
	data->dataHolder = &dHolder;
	data->sprites = std::shared_ptr<sago::SagoSpriteHolder>(new sago::SagoSpriteHolder(*(data->dataHolder)));
	data->exitSprite = &data->sprites->GetSprite("bexit");
	data->p1.Action(BlockGame::PlaceBlock,0,0,2,"");
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
	DrawBlockGame(data->p1,data->p1palce,*data->sprites,data->fTime,target);
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
	
