/* 
 * File:   GameStateManager.hpp
 * Author: poul
 *
 * Created on 4. juni 2014, 21:41
 */

#ifndef GAMESTATEMANAGER_HPP
#define	GAMESTATEMANAGER_HPP

#include "GameState.hpp"
#include "sago/SagoCommandQueue.hpp"
#include <memory>

class GameStateManager {
public:
	GameStateManager();
	virtual ~GameStateManager();
	void PushState(std::shared_ptr<GameState>);
	void PopInactive();
	void Draw(sf::RenderWindow &target);
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input);
	void UpdateCommandQueue(sago::SagoCommandQueue &inout);
private:
	GameStateManager(const GameStateManager& base) = delete;
	GameStateManager& operator=(const GameStateManager& base) = delete;
	struct GameStateManagerData;
	GameStateManagerData *data;
};

#endif	/* GAMESTATEMANAGER_HPP */

