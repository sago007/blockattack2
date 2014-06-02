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

#ifndef SAGOCOMMANDQUEUE_HPP
#define	SAGOCOMMANDQUEUE_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace sago {

class SagoCommandQueue {
public:
	SagoCommandQueue();
	~SagoCommandQueue();
	void ReadKeysAndAddCommands();
	void PushCommand(const std::string &cmd);
	void AddHandler(const std::string &cmd,std::function<void(std::string)> callback);
	void ProcessAllHandlers();
	void ClearCommands();
	void BindKey(const sf::Keyboard::Key &key, const std::string &bindname);
	void BindKeyCommand(const std::string &bindname, const std::string &cmd);
	bool IsPressed(const std::string &bindname) const;
	const std::vector<std::string> &GetCommandQueue() const;
private:
	SagoCommandQueue(const SagoCommandQueue& base) = delete;
    SagoCommandQueue& operator=(const SagoCommandQueue& base) = delete;
	struct SagoCommandQueueData;
	SagoCommandQueueData *data;
};

}

#endif	/* SAGOCOMMANDQUEUE_HPP */

