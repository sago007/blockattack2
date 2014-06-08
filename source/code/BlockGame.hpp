/* 
 * File:   BlockGame.hpp
 * Author: poul
 *
 * Created on 6. juni 2014, 15:49
 */

#ifndef BLOCKGAME_HPP
#define	BLOCKGAME_HPP

#include <string>
#include <SFML/Config.hpp>

struct SingleBlock {
	enum BlockType { Blank, Red, Blue, Yellow, Purple, Turkish, Green, Grey = 7, GarbageColor = 10, GarbageGray = 20 };
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
	static const int bsize = 50; //Block size in pixels
public:
	BlockGame();
	enum ActionType { AdcanceTime, Move, PlaceBlock, Switch, Push, UpdateNextRow};
	void Action(ActionType type, int param_int1, int param_int2, int param_int3, const std::string &param_s);
    const SingleBlock (&GetBoard() const)[coloms][rows] ;
	enum GameState { NotStarted, Running, GameOver, Won, Draw };
	GameState GetStatus() const;
	void GetCursor(int &x, int &y) const;
private:
	sf::Uint32 nextRandomNumber = 0;
	sf::Uint16 rand2();
	void AdvanceTo(int time2advance);
	void SwitchAtCursor();
	void PushLine();
	void PushPixels();
	void SetNextLine();
	void FindTowerHeight();
	bool BoardEmpty() const;
	unsigned int score = 0;
	unsigned int ticks = 0;
	bool timetrial = false;
	SingleBlock board[coloms][rows];
	SingleBlock nextRow[coloms];
	GameState status = NotStarted;
	int cursorx = 2;
	int cursory = 2;
	int TowerHeight = 0;
	int prevTowerHeight = 0;
	int pixels = 0;
	bool puzzleMode = false;
	bool bGameOver = false;
	bool bNearDeath = true;
	int stop = 0;
	unsigned int nrStops = 0; 
	int speedLevel = 0;
	int nrPushedPixel = 0;
	double speed = 0.0;
	double baseSpeed = 0.0;
};

#endif	/* BLOCKGAME_HPP */

