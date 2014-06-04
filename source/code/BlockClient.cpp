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

struct BlockClient::BlockClientData { 
	bool active = true;
};

BlockClient::BlockClient(const sago::SagoDataHolder &texHolder) {
	this->data = new BlockClient::BlockClientData();
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
	
}
void BlockClient::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	for (size_t i = 0; i < input.GetCommandQueue().size();i++) {
		std::string cmd = input.GetCommandQueue().at(i);
		if (cmd == "BACK") {
			this->data->active = false;
		}
	}
}
void BlockClient::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	
}
	
