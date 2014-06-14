/* 
 * File:   BlockGame.cpp
 * Author: poul
 * 
 * Created on 6. juni 2014, 15:49
 */

#include "BlockGame.hpp"
#include <iostream>

namespace {
	void ReduceTime(SingleBlock &block, unsigned int amount) {
		if (block.hang > amount) {
			block.hang-=amount;
		}
		else {
			block.hang = 0;
		}
	}
}

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

const SingleBlock &BlockGame::GetNextLine(int x) const {
	if ( x<0 || x >= coloms ) throw std::exception();
	return nextRow[x];
}

const SingleBlock &BlockGame::GetBoard(int x, int y) const {
	if (x<0 || y < 0 || x >= coloms || y>= rows) throw std::exception();
	return board[x][y];
}

SingleBlock &BlockGame::GetNextLine(int x) {
	if ( x<0 || x >= coloms ) throw std::exception();
	return nextRow[x];
}

SingleBlock &BlockGame::GetBoard(int x, int y) {
	if (x<0 || y < 0 || x >= coloms || y>= rows) throw std::exception();
	return board[x][y];
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
	if ((TowerHeight>11) && (!puzzleMode)&&(!bGameOver)&&(chain==0)) {
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
				ReduceTime(GetBoard(i,j),howMuchHang);
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
	if ((x>5)||(x<0)||(y<0)||(y>29)) return -1;
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
	nrPushedPixel++;
	if ((pixels < bsize) && TowerHeight<12) {
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
	
	SingleBlock::BlockType previus=SingleBlock::Blank; //the last block checked
	int combo=0;
	for (int i=0; i<coloms; i++) {
		for (int j=0; j<rows; j++){
			toBeCleared[i][j] = false;
			garbageToBeCleared[i][j] = false;
		}
	}
	//First all blocks on the first line are stable!
	for (int i=0; i<coloms; i++) {
		if (GetBoard(i,0).type != SingleBlock::Blank) {
			GetBoard(i,0).falling = false;
		}
		else {
			GetBoard(i,0).falling = true;  //Note that blank blocks are marked as falling, so the blocks above them will fall too!
		}
	}
	for (int j=1; j<rows; j++) {
		//For each of the rows above the first one.
		for (int i=0; i<coloms; i++) {
			if ( (GetBoard(i,j).type != SingleBlock::Blank && !GetBoard(i,j-1).falling) || GetBoard(i,j-1).hanging || GetBoard(i,j-1).clearing) {
				GetBoard(i,j).falling = false;
			}
			else {
				GetBoard(i,j).falling = true;
			}
		}
		for (int i=1; i<coloms; i++) {
			if (GetBoard(i,j).type == SingleBlock::GarbageColor || GetBoard(i,j).type == SingleBlock::GarbageGray) {
				//this is a garbage block
				if (GetBoard(i,j).match == GetBoard(i-1,j).match && !GetBoard(i-1,j).falling) {
					//The block to the left is not falling and we are part of the same garbage block. So we are not falling either!
					GetBoard(i,j).falling = false;
				}
			}
		}
		for (int i=coloms-2;i>0;i--) {
			if (GetBoard(i,j).type == SingleBlock::GarbageColor || GetBoard(i,j).type == SingleBlock::GarbageGray) {
				//this is a garbage block
				if (GetBoard(i,j).match == GetBoard(i+1,j).match && !GetBoard(i+1,j).falling) {
					//The block to the right is not falling and we are part of the same garbage block. So we are not falling either!
					GetBoard(i,j).falling = false;
				}
			}
		}
	}
	
	for (int i=0; i<coloms; i++) {
		SingleBlock::BlockType previus = SingleBlock::Blank;
		int combo = 0;
		for (int j=0; j<rows; j++) {
			if (GetBoard(i,j).clearing || GetBoard(i,j).falling || GetBoard(i,j).hanging || GetBoard(i,j).type == SingleBlock::Blank || GetBoard(i,j).type > SingleBlock::Grey) {
				previus = SingleBlock::Blank;
				combo = 0;
				continue;
			}
			if (GetBoard(i,j).type == previus) {
				combo++;
			}
			else {
				if (combo>2) {
					for (int k = j-combo; k<j; k++)
					{
						toBeCleared[i][k] = true;
					}
				}
				combo=1;
				previus = GetBoard(i,j).type;
			}
		}
	}
	
	for (int j=0; j<rows; j++) {
		SingleBlock::BlockType previus = SingleBlock::Blank;
		int combo = 0;
		for (int i=0; i<coloms; i++) {
			if (GetBoard(i,j).clearing || GetBoard(i,j).falling || GetBoard(i,j).hanging || GetBoard(i,j).type == SingleBlock::Blank || GetBoard(i,j).type > SingleBlock::Grey) {
				previus = SingleBlock::Blank;
				combo = 0;
				continue;
			}
			if (GetBoard(i,j).type == previus) {
				combo++;
			}
			else {
				if (combo>2) {
					for (int k = i-combo; k<i; k++)
					{
						toBeCleared[k][j] = true;
					}
				}
				combo=1;
				previus = GetBoard(i,j).type;
			}
		}
	}
	
	for (int i=0; i<coloms; i++) {
		for (int j=0; j<rows; j++) {
			if(toBeCleared[i][j]) {
				GetBoard(i,j).clearing = true;  //Set bomb
				GetBoard(i,j).hang = 10*fallTime;  //Set bomb timer
			}
			if (!GetBoard(i,j).falling && !GetBoard(i,j).clearing && !GetBoard(i,j).hanging) {
				GetBoard(i,j).chainId = 0;  //Clear chain on stable blocks
			}
			if (GetBoard(i,j).clearing && GetBoard(i,j).hang == 0) {
				//Remove blocks
				GetBoard(i,j).type = SingleBlock::Blank;
			}
		}
	}
	
	

	//TO HERE
#if 0	
	combo = 0;
	chain = 0;
	for (int i=0; i<coloms; i++) {
		for (int j=0; j<rows; j++)
		{
			//Clears blocks marked for clearing
			SingleBlock temp = board[i][j];
			if (temp.clearing) {
				if (temp.hang <= 0) {
					if (chainSize[chain]<chainSize[board[i][j].chainId])
						chain = board[i][j].chainId;

					//theBallManeger.addBall(topx+40+i*bsize, topy+bsize*12-j*bsize, true, board[i][j]%10);
					//theBallManeger.addBall(topx+i*bsize, topy+bsize*12-j*bsize, false, board[i][j]%10);
					//theExplosionManeger.addExplosion(topx-10+i*bsize, topy+bsize*12-10-j*bsize);
					board[i][j].type = SingleBlock::TempState;
				}
			}
		}
	}
	for (int i=0; i<coloms; i++) {
		bool setChain=false;
		for (int j=0; j<rows; j++) {
			if (board[i][j].type==SingleBlock::Blank) {
				setChain=false;
			}
			if (board[i][j].type==SingleBlock::TempState) {
				board[i][j].type=SingleBlock::Blank;
				setChain=true;
				//if (SoundEnabled)Mix_PlayChannel(0, boing, 0);
			}
			if (board[i][j].type !=SingleBlock::Blank) {
				if ( setChain && board[i][j].type != SingleBlock::GarbageColor && board[i][j].type != SingleBlock::GarbageGray) {
					board[i][j].chainId = chain;
					//somethingsGottaFall = true;
				}
			}
		}
	}
	
	combo=0;
	for (int j=0; j<rows; j++)
	{
		previus=SingleBlock::Blank;
		combo=0;
		for (int i=0; i<coloms; i++)
		{
			if ((board[i][j].type != SingleBlock::Blank)&&(board[i][j].type <= SingleBlock::GarbageGray && !board[i][j].clearing && !board[i][j].clearing && !board[i][j].hanging))	{
				if (board[i][j].type == previus) {
					combo++;
				}
				else {
					if (combo>2) {
						for (int k = i-combo; k<i; k++)
						{
							toBeCleared[k][j] = true;
						}
					}
					combo=1;
					previus = board[i][j].type;
				}
			} //if board
			else
			{
				if (combo>2)
					for (int k = i-combo; k<i; k++)
					{
						toBeCleared[k][j] = true;
					}
				combo = 0;
				previus = SingleBlock::Blank;
			}

		} //for j
	} //for i
	
	combo = 0;
	chain = 0;
	int grey = 0;
	for (int i=0; i<coloms; i++) {
		for (int j=0; j<rows; j++) {
			if (toBeCleared[i][j]) {
				//see if any garbage is around:
				FirstGarbageMarker(i-1, j);
				FirstGarbageMarker(i+1, j);
				FirstGarbageMarker(i, j-1);
				FirstGarbageMarker(i, j+1);
				//that is checked now :-)
				if (board[i][j].type == SingleBlock::Grey) {
					grey++;
				}
				//if ((vsMode) && (grey>2) && (board[j][i]%10000000==6))
				//	garbageTarget->CreateGreyGarbage();
				if ((board[i][j].type != SingleBlock::Blank)&&(board[i][j].type <= SingleBlock::GarbageGray && !board[i][j].clearing && !board[i][j].clearing && !board[i][j].hanging)) {
					board[i][j].hang = 10*fallTime;
					board[i][j].clearing = true;
				}

				if (chainSize[board[i][j].chainId]>chainSize[chain]) {
					chain=board[i][j].chainId;
				}
				combo++;
				stop+=140*combo;
				score +=10;
				if (combo>3) {
					score+=3*combo; //More points if more cleared simontanously
				}
			}
		}
	}
	score+=chainSize[chain]*100;
	if (chain==0)
	{
		chain=firstUnusedChain();
		chainSize[chain]=0;
		chainUsed[chain]=true;
	}
	chainSize[chain]++;
	for (int i=0; i<coloms; i++) {
		for (int j=0; j<rows; j++)
		{
			if (toBeCleared[i][j]) {
				board[i][j].chainId = chain;
			}
		}
	}

	{
		//This is here we add text to screen!
		bool dead = false;
		for (int i=0; i<coloms; i++) {
			for (int j=rows-1; j>=0; j--) {
				if (toBeCleared[i][j]) {
					if (!dead)
					{
						//dead=true;
						//string tempS = itoa(chainSize[chain]);
						//if (chainSize[chain]>1)
						//	theTextManeger.addText(topx-10+j*bsize, topy+12*bsize-i*bsize, tempS, 1000);
					}
				}
			}
		}
	} //This was there text was added
	
	
	//Add create garbage here!
	
	for (int i=0; i<coloms; i++) {
		for (int j=0; j<rows; j++) {
			if (garbageToBeCleared[i][j])
			{
				GarbageClearer(i, j, board[i][j].match, true, chain); //Clears the blocks and all blocks connected to it.
			}
		}
	}
	
	chain=0;

	//Break chains (if a block is stable it is reset to (chain == 0)):
	for (int i=0; i<coloms; i++) {
		bool faaling = false;  //In the beginning we are NOT falling
		for (int j=0; j<rows; j++)
		{
			if ( faaling && board[i][j].type > SingleBlock::Blank && board[i][j].type <= SingleBlock::Grey ) {
				board[i][j].falling = true;
			}
			if ( !faaling && board[i][j].falling)
				board[i][j].falling = false;
			if ((!faaling)&&(board[i][j].type != SingleBlock::Blank)&&(board[i][j].chainId > 0)&&(!board[i][j].clearing)&&(!board[i][j].hanging)) {
				if (chainSize[board[i][j].chainId]>chainSize[chain]) {
					chain=board[i][j].chainId;
				}
				board[i][j].chainId = 0;
			}
			if (!( board[i][j].type>SingleBlock::Blank && board[i][j].type<=SingleBlock::Grey && !board[i][j].clearing && !board[i][j].falling && !board[i][j].hanging )  ) {
				faaling=true;
			}
			if (board[i][j].type == SingleBlock::GarbageColor || board[i][j].hanging || board[i][j].clearing) {
				faaling = false;
			}
		}
	}
#endif
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
	if ((ticks>nrPushedPixel*1000*speed) && (status == BlockGame::Running) && (!stop)) {
		while ( (ticks>nrPushedPixel*1000*speed) && (!(puzzleMode))) {
			PushPixels();
		}
	}
	if (status == Running) {
		ReduceStuff();
		ClearBlocks();
	}
}
