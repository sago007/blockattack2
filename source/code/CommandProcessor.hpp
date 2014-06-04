/* 
 * File:   CommandProcessor.hpp
 * Author: poul
 *
 * Created on 4. juni 2014, 22:21
 */

#ifndef COMMANDPROCESSOR_HPP
#define	COMMANDPROCESSOR_HPP

#include "GameStateManager.hpp"
#include "sago/SagoCommandQueue.hpp"
#include "sago/SagoDataHolder.hpp"

void ProcessCommands(sago::SagoCommandQueue &queue, sago::SagoDataHolder &dh, GameStateManager &m);

#endif	/* COMMANDPROCESSOR_HPP */

