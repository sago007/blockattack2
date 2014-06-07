/* 
 * File:   BlockGame.hpp
 * Author: poul
 *
 * Created on 6. juni 2014, 15:49
 */

#ifndef BLOCKGAME_HPP
#define	BLOCKGAME_HPP

#include <string>

struct SingleBlock {
	enum BlockType { Blank, Red, Blue, Yellow, Purple, LightBlue, Green, Gray = 7, GarbageColor = 10, GarbageGray = 20 };
	BlockType type = BlockType::Blank; 
	unsigned int chainId = 0;  //What chain group does we blong to?
	unsigned int match = 0;  //tells what garbage belongs together.
	bool falling = false;  //Is the block falling?
	bool clearing = false;  //Is the block being cleared?
};

class BlockGame {
public:
	static const int coloms = 6;
	static const int rows = 30;
public:
	BlockGame();
	enum ActionType { AdcanceTime, Move, PlaceBlock};
	void Action(ActionType type, int param_int1, int param_int2, int param_int3, const std::string &param_s);
    const SingleBlock (&GetBoard() const)[coloms][rows] ;
	enum GameState { NotStarted, Started, GameOver, Won, Draw };
	GameState GetStatus() const;
	void GetCursor(int &x, int &y) const;
private:
	void AdvanceTo(int time2adcance);
	unsigned int score = 0;
	unsigned int ticks = 0;
	bool timetrial = false;
	SingleBlock board[6][30];
	GameState status = NotStarted;
	int cursorx = 2;
	int cursory = 2;
};

#endif	/* BLOCKGAME_HPP */

