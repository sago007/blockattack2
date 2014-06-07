/* 
 * File:   BlockGame.cpp
 * Author: poul
 * 
 * Created on 6. juni 2014, 15:49
 */

#include "BlockGame.hpp"
#include <iostream>

BlockGame::BlockGame() {
}

void BlockGame::Action(ActionType type, int param_int1, int param_int2, int param_int3, const std::string &param_s) {
	switch (type) {
		case ActionType::AdcanceTime:
			break;
		case ActionType::Move:
			break;
		case ActionType::PlaceBlock:
			if (param_int1 < 0 || param_int1 > coloms) break;
			if (param_int2 < 0 || param_int2 > rows) break;
			if (param_int3 < 1 || param_int3 > SingleBlock::Gray) break;
			board[param_int1][param_int2].type = static_cast<SingleBlock::BlockType>(param_int3);
			break;
		default:
			std::cerr << "Unknown type: " << type << std::endl;
	};
}

BlockGame::GameState BlockGame::GetStatus() const {
	return status;
}

const SingleBlock (&BlockGame::GetBoard() const)[coloms][rows] {
	return board;
}

void BlockGame::GetCursor(int &x, int &y) const {
	x = cursorx;
	y = cursory;
}
