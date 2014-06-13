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
	enum BlockType { Blank, Red, Blue, Yellow, Purple, Turkish, Green, Grey = 7, GarbageColor = 10, GarbageGray = 20, TempState = 100 };
	BlockType type = BlockType::Blank; 
	unsigned int chainId = 0;  //What chain group does we blong to?
	unsigned int match = 0;  //tells what garbage belongs together.
	unsigned int hang = 0;
	bool falling = false;  //Is the block falling?
	bool clearing = false;  //Is the block being cleared?
	bool hanging = false;  //Is the block hanging (after harbage clear)
};

class BlockGame {
public:
	static const int coloms = 6;
	static const int rows = 30;
	static const int bsize = 50; //Block size in pixels
	static const int maxNumberOfChains = 100;
	static const int garbageStackSize = 10;
	static const int frameLength = 50;
	static const int hangTime = 40;
	static const int fallTime = 20;
public:
	BlockGame();
	enum ActionType { AdcanceTime, Move, PlaceBlock, Switch, Push, UpdateNextRow};
	void Action(ActionType type, int param_int1, int param_int2, int param_int3, const std::string &param_s);
	const SingleBlock &GetNextLine(int x) const;
	const SingleBlock &GetBoard(int x, int y) const;
	enum GameState { NotStarted, Running, GameOver, Won, Draw };
	GameState GetStatus() const;
	void GetCursor(int &x, int &y) const;
    int GetPixels() const;
private:
	sf::Uint32 nextRandomNumber = 0;
	sf::Uint16 rand2();
	SingleBlock &GetNextLine(int x);
	SingleBlock &GetBoard(int x, int y);
	int firstUnusedChain();
	void setGameSpeed(int globalSpeedLevel);
	void emptyGarbageStack();
	bool pushGarbage(int width, int height, int type);
	bool popGarbage(int &width, int &height, int &type);
	void setPlayerWon();
	void setDraw();
	bool LineEmpty(int lineNr);
	bool BoardEmpty();
	bool hasStaticContent();
	bool CreateGarbage(int wide, int height);
	bool CreateGreyGarbage();
	int GarbageClearer(int x, int y, int number, bool aLineToClear, int chain);
	int GarbageMarker(int x, int y);
	int FirstGarbageMarker(int x, int y);
	void AdvanceTo(int time2advance);
	void ClearBlocks();
	void SetGameOver();
	void SwitchAtCursor();
	void PushLine();
	void PushPixels();
	void ReduceStuff();
	void SetNextLine();
	void FallDown();
	void FindTowerHeight();
	bool BoardEmpty() const;
	unsigned int score = 0;
	unsigned int ticks = 0;
	unsigned int gameStatedAt = 3000;
	bool timetrial = false;
	SingleBlock board[coloms][rows];
	SingleBlock nextRow[coloms];
	bool garbageToBeCleared[coloms][rows];
	GameState status = NotStarted;
	int cursorx = 2;
	int cursory = 2;
	int TowerHeight = 0;
	int prevTowerHeight = 0;
	int pixels = 0;
	bool puzzleMode = false;
	bool bGameOver = false;
	bool bNearDeath = true;
	bool bGarbageFallLeft = false;
	int stop = 0;
	unsigned int nrStops = 0;
	unsigned int nrFellDown = 0;
	unsigned int speedLevel = 1;
	unsigned int hangTicks = 0;
	int nrPushedPixel = 0;
	int nextGarbageNumber = 1;
	double speed = 1.0;
	double baseSpeed = 1.0;
	int chain = 0; 
	int chainSize[maxNumberOfChains]{}; //Contains the chains
	bool chainUsed[maxNumberOfChains]{};   //True if the chain is used
	int garbageStack[garbageStackSize][3]{}; //A garbage stack with space for 10 garbage blocks. 0=x,1=y,2=type
	int garbageStackUsed = 0;
};

#endif	/* BLOCKGAME_HPP */

