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
#include "sago/SagoMisc.hpp"
#include <memory>

namespace {
	void DrawOneBlock(const SingleBlock &b, const sf::Rect<int> &place, int i, int j, 
		const sago::SagoSpriteHolder &spHolder, float fTime, int pixels, sf::RenderWindow &target) {
		sf::IntRect part;
		part.left = 0;
		part.top = 0;
		part.width = 50;
		if (j == -1) {
			part.height = pixels;
		}
		else {
			part.height = 50;
		}
		if (b.type == b.Blue) {
			 const sago::SagoSprite &mySprite = spHolder.GetSprite("block_blue");
			 mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
		if (b.type == b.Green) {
			 const sago::SagoSprite &mySprite = spHolder.GetSprite("block_green");
			 mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
		if (b.type == b.Grey) {
			 const sago::SagoSprite &mySprite = spHolder.GetSprite("block_grey");
			 mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
		if (b.type == b.Purple) {
			 const sago::SagoSprite &mySprite = spHolder.GetSprite("block_purple");
			 mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
		if (b.type == b.Red) {
			 const sago::SagoSprite &mySprite = spHolder.GetSprite("block_red");
			 mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
		if (b.type == b.Turkish) {
			 const sago::SagoSprite &mySprite = spHolder.GetSprite("block_turkish");
			 mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
		if (b.type == b.Yellow) {
			const sago::SagoSprite &mySprite = spHolder.GetSprite("block_yellow");
			mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
		if (b.clearing) {
			const sago::SagoSprite &mySprite = spHolder.GetSprite("block_bomb");
			mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
		if (j == -1 && pixels > 0) {
			const sago::SagoSprite &mySprite = spHolder.GetSprite("block_trans");
			mySprite.Draw(target,fTime,place.left+50*i,place.top+place.height-50-50*j-pixels,part);
		}
	}
	
	void DrawBlockGame(const BlockGame &g, const sf::Rect<int> &place, const sago::SagoSpriteHolder &spHolder, float fTime, sf::RenderWindow &target) {
		const sago::SagoSprite &background = spHolder.GetSprite("boardbackback");
		const sago::SagoSprite &backgroundinner = spHolder.GetSprite("backboard");
		const sago::SagoSprite &cursor = spHolder.GetSprite("cursor");
		sf::Text mytext;
		mytext.setFont(*spHolder.GetDataHolder().getFontPtr("FreeSerif"));
		mytext.setColor(sf::Color::White);
		background.Draw(target,fTime,place.left,place.top);
		sago::DrawText(target,mytext,"Score",place.left+330,place.top+80,16);
		sago::DrawText(target,mytext,"Time",place.left+330,place.top+130,16);
		sago::DrawText(target,mytext,"Chain",place.left+330,place.top+180,16);
		sago::DrawText(target,mytext,"Speed",place.left+330,place.top+230,16);
		backgroundinner.Draw(target,fTime,place.left,place.top);
		for (int i=0; i< g.coloms; i++) {
			for (int j=0;j< 12;j++) {
				DrawOneBlock(g.GetBoard(i,j), place,i,j,spHolder,fTime,g.GetPixels(), target);
			}
		}
		for (int i=0;i < g.coloms; i++) {
			DrawOneBlock(g.GetNextLine(i), place,i,-1,spHolder,fTime,g.GetPixels(), target);
		}
		BlockGame::GameState gamestate = g.GetStatus();
		if (gamestate == BlockGame::GameOver) {
			sago::DrawText(target,mytext,"Game Over",place.left+50,place.top+300,52);
		}
		if (gamestate < BlockGame::GameOver) {
			int cursorx, cursory;
			g.GetCursor(cursorx,cursory);
			cursor.Draw(target,fTime,place.left+50*cursorx,place.top-50+50*12-50*cursory-g.GetPixels());
		}
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
	sf::Rect<int> p1palce = sf::Rect<int>(150,100,300,600);
	float p1UpdateTime = 0.0;
};

BlockClient::BlockClient(const sago::SagoDataHolder &dHolder) : data(new BlockClientData()) {
	data->dataHolder = &dHolder;
	data->sprites = std::shared_ptr<sago::SagoSpriteHolder>(new sago::SagoSpriteHolder(*(data->dataHolder)));
	data->exitSprite = &data->sprites->GetSprite("bexit");
	data->p1.Action(BlockGame::UpdateNextRow,0,0,0,"");
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
	if ( data->p1UpdateTime + 20.0f < data->fTime) {
		data->p1UpdateTime = data->fTime;
		data->p1.Action(BlockGame::AdcanceTime,data->p1UpdateTime,0,0,"");
	}
	for (size_t i = 0; i < input.GetCommandQueue().size();i++) {
		std::string cmd = input.GetCommandQueue().at(i);
		if (cmd == "BACK") {
			this->data->active = false;
		}
		if (cmd == "NORTH") {
			data->p1.Action(BlockGame::Move,0,0,0,"");
		}
		if (cmd == "SOUTH") {
			data->p1.Action(BlockGame::Move,1,0,0,"");
		}
		if (cmd == "WEST") {
			data->p1.Action(BlockGame::Move,2,0,0,"");
		}
		if (cmd == "EAST") {
			data->p1.Action(BlockGame::Move,3,0,0,"");
		}
		if (cmd == "SWITCH") {
			data->p1.Action(BlockGame::Switch,0,0,0,"");
		}
		if (cmd == "PUSH") {
			data->p1.Action(BlockGame::Push,0,0,0,"");
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
	
