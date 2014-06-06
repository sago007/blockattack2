/*
===========================================================================
blockattack - Block Attack - Rise of the Blocks
Copyright (C) 2014 Poul Sander

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

Source information and contacts persons can be found at
http://blockattack.sf.net
===========================================================================
*/

#ifndef COMMANDPROCESSOR_HPP
#define	COMMANDPROCESSOR_HPP

#include "sago/GameStateManager.hpp"
#include "sago/SagoCommandQueue.hpp"
#include "sago/SagoDataHolder.hpp"

void ProcessCommands(sago::SagoCommandQueue &queue, sago::SagoDataHolder &dh, sago::GameStateManager &m);

#endif	/* COMMANDPROCESSOR_HPP */

