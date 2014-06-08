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
			AdvanceTo(param_int1);
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

int BlockGame::firstUnusedChain() {
	bool found=false;
	int i = 0;
	while (!found && i+1 < maxNumberOfChains) {
		if (!chainUsed[++i]) found=true;
	}
	return i;
}

void BlockGame::setGameSpeed(int globalSpeedLevel)
{
	switch (globalSpeedLevel)
	{
	case 0:
		baseSpeed=0.5;
		break;
	case 1:
		baseSpeed=0.4;
		break;
	case 2:
		baseSpeed=0.3;
		break;
	case 3:
		baseSpeed=0.25;
		break;
	case 4:
		baseSpeed=0.2;
		break;
	default:
		baseSpeed=0.15;
		break;
	};
}

void BlockGame::emptyGarbageStack() {
	for (int i=0; i<10; i++) {
		for (int j=0; j<3; j++) {
			garbageStack[i][j] = 0;
		}
	}
	garbageStackUsed = 0;
}

bool BlockGame::pushGarbage(int width, int height, int type) {
	if (garbageStackUsed>=garbageStackSize)
		return false;
	garbageStack[garbageStackUsed][0]=width;
	garbageStack[garbageStackUsed][1]=height;
	garbageStack[garbageStackUsed][2]=type;
	garbageStackUsed++;
	return true;
}

bool BlockGame::popGarbage(int &width, int &height, int &type) {
	if (garbageStackUsed<1)
		return false;
	garbageStackUsed--;
	width=garbageStack[garbageStackUsed][0];
	height=garbageStack[garbageStackUsed][1];
	type=garbageStack[garbageStackUsed][2];
	return true;
}

void BlockGame::setPlayerWon()
{
	if (status == BlockGame::Running) {
		status = BlockGame::Won;
	}
}

void BlockGame::setDraw()
{
	if (status == BlockGame::Running) {
		status = BlockGame::Draw;
	}
}

//Test if LineNr is an empty line, returns false otherwise.
bool BlockGame::LineEmpty(int lineNr)
{
	bool empty = true;
	for (int i = 0; i <coloms; i++) {
		if (board[i][lineNr].type != SingleBlock::Blank) {
			empty = false;
		}
	}
	return empty;
}

//Test if the entire board is empty (used for Puzzles)
bool BlockGame::BoardEmpty()
{
	bool empty = true;
	for (int i=0; i<coloms; i++) {
		for (int j=0; j<14; j++) {
			if (board[i][j].type != SingleBlock::Blank) {
				empty = false;
			}
		}
	}
	return empty;
}

bool BlockGame::hasStaticContent() {
	for (int i=0; i<coloms; i++) {
		for (int j=1; j<13; j++) {
			if (board[i][j].clearing || board[i][j].falling ) {
				return true;			//They are static
			}
		}
	}
	return false;					//Return false if no static object found
}

BlockGame::GameState BlockGame::GetStatus() const {
	return status;
}

const SingleBlock (&BlockGame::GetBoard() const)[coloms][rows] {
	return board;
}

const SingleBlock (&BlockGame::GetNextLine() const)[coloms] {
	return nextRow;
}

void BlockGame::GetCursor(int &x, int &y) const {
	x = cursorx;
	y = cursory;
}

int BlockGame::GetPixels() const {
	return pixels;
}


void BlockGame::SwitchAtCursor() {
	if (status != BlockGame::Running) {
		return;
	}
	SingleBlock leftBlock = board[cursorx][cursory];
	SingleBlock rightBlock = board[cursorx+1][cursory];
	if (leftBlock.type > SingleBlock::Grey || rightBlock.type > SingleBlock::Grey ) return;
	if (leftBlock.falling || rightBlock.falling) return;
	if (leftBlock.clearing || rightBlock.clearing) return;
	board[cursorx][cursory] = rightBlock;
	board[cursorx+1][cursory] = leftBlock;
}

void BlockGame::PushLine() {
	if (status != BlockGame::Running) {
		return;
	}
	for (int i = 0; i < coloms; i++) {
		for (int j = rows-1; j > 0; j--) {
			board[i][j] = board[i][j-1];
		}
		board[i][0] = nextRow[i];
	}
	SetNextLine();
	score++;
	if ((TowerHeight>12) && (!puzzleMode)&&(!bGameOver)&&(chain==0)) {
		/*if ((!vsMode)&&(theTopScoresEndless.isHighScore(score))&&(!AI_Enabled))
		{
			if (SoundEnabled)Mix_PlayChannel(1, applause, 0);
			theTopScoresEndless.addScore(name, score);
			if(verboseLevel)
				cout << "New high score!" << endl;
		}*/
		SetGameOver();
	}
}

void BlockGame::ReduceStuff() {
	int howMuchHang = (ticks - frameLength*hangTicks)/frameLength;
	if (howMuchHang>0)
	{
		for (int i=0; i<coloms; i++) {
			for (int j=0; j<rows; j++) {
				if (board[i][j].hanging) {
					int hangNumber = board[i][j].hang;
					if (hangNumber<=howMuchHang) {
						board[i][j].hanging = false;
						board[i][j].hang = 0;
					}
					else {
						board[i][j].hang-=howMuchHang;
					}
				}
				if (board[i][j].clearing) {
					int hangNumber = board[i][j].hang;
					if (hangNumber<=howMuchHang) {
						//The blocks must be cleared
						board[i][j].hang = 0;
					}
					else
					{
						board[i][j].hang-=howMuchHang;
					}
				}
			}
		}
	}
	hangTicks+=howMuchHang;
}

bool BlockGame::CreateGarbage(int wide, int height)
{
	if (wide>coloms) wide = coloms;
	if (height>12) height = 12;
	int startPosition = 12;
	while ((!(LineEmpty(startPosition))) || (startPosition == 29)) {
		startPosition++;
	}
	if (startPosition == 29) return false; //failed to place blocks
	if (29-startPosition<height) return false;	//not enough space
	int start; 
	int end;
	if (bGarbageFallLeft) {
		start=0;
		end=start+wide;
	}
	else {
		start=6-wide;
		end = 6;
	}
	for (int i = startPosition; i <startPosition+height; i++) {
		for (int j = start; j < end; j++)
		{
			board[j][i].type = SingleBlock::GarbageColor;
			board[j][i].match = nextGarbageNumber;
		}
	}
	nextGarbageNumber++;
	if (nextGarbageNumber>999999) nextGarbageNumber = 1;
	bGarbageFallLeft = !(bGarbageFallLeft);
	return true;
}

bool BlockGame::CreateGreyGarbage()
{
		int startPosition = 12;
		while ((!(LineEmpty(startPosition))) || (startPosition == 29))
			startPosition++;
		if (startPosition == 29) return false; //failed to place blocks
		if (29-startPosition<1) return false;	//not enough space
		int start, end;
		{
			start=0;
			end=6;
		}
		for (int i = startPosition; i <startPosition+1; i++) {
			for (int j = start; j < end; j++) {
				board[j][i].type = SingleBlock::GarbageGray;
				board[j][i].match = nextGarbageNumber;
			}
		}
		nextGarbageNumber++;
		if (nextGarbageNumber>999999) {
			nextGarbageNumber = 1;
		}
		return true;
}

//Clears garbage, must take one the lower left corner!
int BlockGame::GarbageClearer(int x, int y, int number, bool aLineToClear, int chain)
{
	if ((board[x][y]).match != number) return -1;
	if (aLineToClear) {
		board[x][y].type = static_cast<SingleBlock::BlockType>(1 + (rand() % 6) );
		board[x][y].hanging = true;
		board[x][y].hang = hangTime;
		board[x][y].chainId = chain;
	}
	garbageToBeCleared[x][y] = false;
	GarbageClearer(x+1, y, number, aLineToClear, chain);
	GarbageClearer(x, y+1, number, false, chain);
	return 1;
}

//Marks garbage that must be cleared
int BlockGame::GarbageMarker(int x, int y)
{
	if ((x>5)||(x<0)||(y<0)||(y>29)) return -1;
	if ((board[x][y].GarbageColor == board[x][y].GarbageColor )&&(garbageToBeCleared[x][y] == false)) {
		garbageToBeCleared[x][y] = true;
		//Float fill
		GarbageMarker(x-1, y);
		GarbageMarker(x+1, y);
		GarbageMarker(x, y-1);
		GarbageMarker(x, y+1);
	}
	return 1;
}

int BlockGame::FirstGarbageMarker(int x, int y)
{
	if ((x>5)||(x<0)||(y<0)||(y>29)) return -1;
	if ((board[x][y].GarbageColor == board[x][y].GarbageGray)&&(garbageToBeCleared[x][y] == false))
	{
		for (int i=0; i<coloms; i++)
			garbageToBeCleared[i][y] = true;
	}
	else if ((board[x][y].GarbageColor == board[x][y].GarbageColor)&&(garbageToBeCleared[x][y] == false))
	{
		garbageToBeCleared[x][y] = true;
		//Float fill
		GarbageMarker(x-1, y);
		GarbageMarker(x+1, y);
		GarbageMarker(x, y-1);
		GarbageMarker(x, y+1);
	}
	return 1;
}

void BlockGame::SetNextLine() {
	for (int i = 0; i < coloms; i++) {
		nextRow[i].type = static_cast<SingleBlock::BlockType>(1+(rand2()%6));
	}
}

void BlockGame::FallDown() {
	for (int i = 0; i < coloms; i++) {
		for (int j=0; j< rows-1; j++) {
			if (board[i][j].type == SingleBlock::Blank && board[i][j+1].falling) {
				board[i][j] = board[i][j+1];
				board[i][j].type = SingleBlock::Blank;
			}
		}
	}
	nrFellDown++;
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
	std::cerr << "Pushed pixel" << std::endl;
	nrPushedPixel++;
	if ((pixels < bsize) && TowerHeight<13) {
		pixels++;
	}
	else {
		PushLine();
	}
	if (pixels>bsize) {
		pixels=0;
	}
}

void BlockGame::SetGameOver() {
	status = BlockGame::GameOver;
	bGameOver = true;
}

void BlockGame::ClearBlocks() {
	bool toBeCleared[coloms][rows];
}

void BlockGame::AdvanceTo(const int time2advance) {
	if (status == BlockGame::NotStarted) {
		if (time2advance > gameStatedAt) {
			status = BlockGame::Running;
			std::cout << "Game started" << std::endl;
		}
	}
	if (status == BlockGame::Running) {
		ticks = time2advance-gameStatedAt;
	}
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
	if ((ticks>nrPushedPixel*1000*speed) && (!bGameOver)&&(!stop)) {
		while ((ticks>nrPushedPixel*1000*speed)&&(!(puzzleMode))) {
			PushPixels();
		}
	}
	if (status == Running) {
		ClearBlocks();
	}
}
