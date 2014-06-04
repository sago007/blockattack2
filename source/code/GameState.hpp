/* 
 * File:   GameState.hpp
 * Author: poul
 *
 * Created on 4. juni 2014, 20:59
 */

#ifndef GAMESTATE_HPP
#define	GAMESTATE_HPP

#include "sago/SagoCommandQueue.hpp"

class GameState {
public:
    /**
     * Is the state active. If this returns false then the State-manager will pop the state object
     * @return true if active
     */
    virtual bool IsActive() = 0;
    /**
     * Is the state blocking states under it?
     * @return true if blocking update
     */
    virtual bool IsBlockingDraw() = 0;
    /**
     * Tells the state to draw itself to target
     * @param target The RenderWindow to draw to
     */
    virtual void Draw(sf::RenderWindow &target) = 0;
    /**
     * Is the state blocking lower states from updating?
     * @return true if blocking
     */
    virtual bool IsBlockingUpdate() = 0;
    /**
     * Called reguarly by the game loop if not blocking
     * This is the only input given to a state!
     * @param fDeltaTime mili seconds since last update
     * @param input The input queue. Note that the state cannot push commands
     */
    virtual void Update(float fDeltaTime, const sago::SagoCommandQueue &input) = 0;
    /**
     * Allows the state to push events to the event queue
     * The state should not read from this!
     * @param inout The state may add events to this
     */
    virtual void UpdateCommandQueue(sago::SagoCommandQueue &inout) = 0;
};

#endif	/* GAMESTATE_HPP */

