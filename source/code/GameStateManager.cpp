/* 
 * File:   GameStateManager.cpp
 * Author: poul
 * 
 * Created on 4. juni 2014, 21:41
 */

#include "GameStateManager.hpp"

using namespace std;

struct GameStateManager::GameStateManagerData {
	vector<shared_ptr<GameState>  > states;  
};

GameStateManager::GameStateManager() {
	this->data = new GameStateManager::GameStateManagerData();
}

GameStateManager::~GameStateManager() {
	delete this->data;
}

void GameStateManager::PushState(std::shared_ptr<GameState> s) {
	data->states.push_back(s);
}

void GameStateManager::PopInactive() {
	if (data->states.size() == 0) return;
	if (!data->states.back()->IsActive()) {
		data->states.pop_back();
		PopInactive();
	}
}

void GameStateManager::Draw(sf::RenderWindow &target) {
	if (data->states.size() == 0) return;
	size_t firstState = 0;
	for (size_t i = data->states.size(); i > firstState; i--) {
		if (data->states.at(i-1)->IsBlockingDraw() ) {
			firstState = i-1;
		}
	}
	for (size_t i = firstState; i < data->states.size(); i++) {
		data->states.at(i)->Draw(target);
	}
}

void GameStateManager::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	if (data->states.size() == 0) return;
	PopInactive();
	size_t firstState = 0;
	for (size_t i = data->states.size(); i > firstState; i--) {
		if (data->states.at(i-1)->IsBlockingUpdate() ) {
			firstState = i-1;
		}
	}
	for (size_t i = firstState; i < data->states.size(); i++) {
		data->states.at(i)->Update(fDeltaTime,input);
	}
}

void GameStateManager::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	if (data->states.size() == 0) return;
	size_t firstState = 0;
	for (size_t i = data->states.size(); i > firstState; i--) {
		if (data->states.at(i-1)->IsBlockingUpdate() ) {
			firstState = i-1;
		}
	}
	for (size_t i = firstState; i < data->states.size(); i++) {
		data->states.at(i)->UpdateCommandQueue(inout);
	}
}