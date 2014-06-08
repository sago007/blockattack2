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
		case ActionType::Switch:
			SwitchAtCursor();
			break;
		case ActionType::Push:
			PushLine();
			break;
		case ActionType::Move:
		{
			if ((param_int1 == 0 /*N*/) && ((cursory<10)||(TowerHeight>12) ||(((pixels==50)||(pixels==0)) && (cursory<11))))
				cursory++;
			if ((param_int1 == 1 /*S*/) && (cursory>0))
				cursory--;
			if ((param_int1 == 2 /*W*/) && (cursorx>0))
				cursorx--;
			if ((param_int1 == 3 /*E*/) && (cursorx<4))
				cursorx++;
		}
			break;
		case ActionType::PlaceBlock:
			if (param_int1 < 0 || param_int1 > coloms) break;
			if (param_int2 < 0 || param_int2 > rows) break;
			if (param_int3 < 1 || param_int3 > SingleBlock::Grey) break;
			board[param_int1][param_int2].type = static_cast<SingleBlock::BlockType>(param_int3);
			break;
		case ActionType::UpdateNextRow:
			SetNextLine();
			break;
		default:
			std::cerr << "Unknown type: " << type << std::endl;
	};
}

sf::Uint16 BlockGame::rand2()
{
	nextRandomNumber = nextRandomNumber*1103515245 + 12345;
	return ((sf::Uint16)(nextRandomNumber/65536)) % 32768;
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


void BlockGame::SwitchAtCursor() {
	SingleBlock leftBlock = board[cursorx][cursory];
	SingleBlock rightBlock = board[cursorx+1][cursory];
	if (leftBlock.type > SingleBlock::Grey || rightBlock.type > SingleBlock::Grey ) return;
	if (leftBlock.falling || rightBlock.falling) return;
	if (leftBlock.clearing || rightBlock.clearing) return;
	board[cursorx][cursory] = rightBlock;
	board[cursorx+1][cursory] = leftBlock;
}

void BlockGame::PushLine() {
	for (int i = 0; i < coloms; i++) {
		for (int j = rows-1; j > 0; j--) {
			board[i][j] = board[i][j-1];
		}
		board[i][0] = nextRow[i];
	}
	SetNextLine();
	score++;
}

void BlockGame::SetNextLine() {
	for (int i = 0; i < coloms; i++) {
		nextRow[i].type = static_cast<SingleBlock::BlockType>(1+(rand2()%6));
	}
}

bool BlockGame::BoardEmpty() const {
	bool empty = true;
	for (int i=0; i<coloms; i++) {
		for (int j=1; j<rows; j++) {
			if (board[i][j].type != SingleBlock::Blank) {
				empty = false;
			}
		}
	}
	return empty;
}

void BlockGame::FindTowerHeight()
{
	/*
	 * Old implementation, used until I find the bug in the other.
	 * This function has a bug in stage clear! if an empty line appears.
	 */
	prevTowerHeight = TowerHeight;
	bool found = false;
	TowerHeight = 0;
	while (!found && TowerHeight < rows)
	{
		found = true;
		for (int j=0; j<coloms; j++) {
			if (board[j][TowerHeight].type != SingleBlock::Blank) {
				found = false;
			}
		}
		TowerHeight++;
	}
	TowerHeight--;
}

void BlockGame::PushPixels() {
	nrPushedPixel++;
	if ((pixels < bsize) && TowerHeight<13)
	{
		pixels++;
	}
	else
		PushLine();
	if (pixels>bsize)
		pixels=0;
}

void BlockGame::AdvanceTo(const int time2advance) {
	FindTowerHeight();
	if ((TowerHeight>12)&&(prevTowerHeight<13)&&(!puzzleMode)) {
		//if (SoundEnabled) Mix_PlayChannel(1, heartBeat, 0);
		stop+=1000;
	}
	if ((TowerHeight>12)&&(!puzzleMode)&&(status == Running))
	{
		bNearDeath = true;
	}
	while (ticks>nrStops*40) {
	//Increase stops, till we reach nowTime
		if (stop>0) {
			stop = stop-20;
			if (stop<=0) {
				nrPushedPixel=(int)((ticks)/(1000.0*speed));
			}
		}
		if (stop<0) {
			stop = 0;
		}
		nrStops++;
	}
	
	if ((ticks>20000*speedLevel)&&(speedLevel <99) && (status == Running)){
		speed = (baseSpeed*0.9)/((double)speedLevel*0.5);
		speedLevel++;
		nrPushedPixel=(int)((double)(ticks)/(1000.0*speed));
	}
}