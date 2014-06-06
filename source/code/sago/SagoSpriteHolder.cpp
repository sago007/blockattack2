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

#include "SagoSpriteHolder.hpp"
#include "SagoMisc.hpp"
#include <memory>
#include <unordered_map>
#include <json-c/json.h>
#include <iostream>
#include <string.h>
#include <boost/algorithm/string/predicate.hpp>


//I truely hate the C way of checking for equal. Usually read: "if not X compares to Y then they must be equal"
#define strequal(X,Y) strcmp(X,Y)==0

using namespace std;

namespace sago {

struct SagoSpriteHolder::SagoSpriteHolderData {
	const sago::SagoDataHolder* tex;
	std::unordered_map<std::string,std::shared_ptr<sago::SagoSprite>> sprites;
	const sago::SagoSprite* defaultSprite;
};
	
SagoSpriteHolder::SagoSpriteHolder(const SagoDataHolder &texHolder) {
	data = new SagoSpriteHolderData();
	try {
		data->tex = &texHolder;
		ReadSprites();
		data->defaultSprite = new sago::SagoSprite(texHolder,"fallback",sf::IntRect(0,0,64,64),1,100);
	} catch (...) {
		delete data;
	}
}

SagoSpriteHolder::~SagoSpriteHolder() {
	delete data;
}

void SagoSpriteHolder::ReadSpriteFile(const std::string &filename) {
	string fullfile = "sprites/"+filename;
	string content = sago::GetFileContent(fullfile.c_str());
	json_object * jobj = json_tokener_parse(content.c_str());
	json_object_object_foreach(jobj, key, val) {
		string spriteName = key;
		string textureName;
		int topx = 0;
		int topy = 0;
		int height = 0;
		int width = 0;
		int number_of_frames = 1;
		int frame_time = 1;
		enum json_type type = json_object_get_type(val);
		if (type == json_type_object) {
			json_object *innerobject = json_object_object_get(jobj, key);
			json_object_object_foreach(innerobject, key2, val2) {
				if (strequal(key2,"texture")) {
					type = json_object_get_type(val2);
					if (type == json_type_string) {
						textureName = json_object_get_string(val2);
					}
					else {
						cerr << "Error passing sprite! File: " << filename << ", sprite: " << spriteName << ", key: " << key2 << ". Value was expected to be a string!" << endl; 
					}
				}
				if (strequal(key2,"topx")) {
					type = json_object_get_type(val2);
					if (type == json_type_int) {
						topx = json_object_get_int(val2);
					}
					else {
						cerr << "Error passing sprite! File: " << filename << ", sprite: " << spriteName << ", key: " << key2 << ". Value was expected to be an integer!" << endl; 
					}
				}
				if (strequal(key2,"topy")) {
					type = json_object_get_type(val2);
					if (type == json_type_int) {
						topy = json_object_get_int(val2);
					}
					else {
						cerr << "Error passing sprite! File: " << filename << ", sprite: " << spriteName << ", key: " << key2 << ". Value was expected to be an integer!" << endl; 
					}
				}
				if (strequal(key2,"height")) {
					type = json_object_get_type(val2);
					if (type == json_type_int) {
						height = json_object_get_int(val2);
					}
					else {
						cerr << "Error passing sprite! File: " << filename << ", sprite: " << spriteName << ", key: " << key2 << ". Value was expected to be an integer!" << endl; 
					}
				}
				if (strequal(key2,"width")) {
					type = json_object_get_type(val2);
					if (type == json_type_int) {
						width = json_object_get_int(val2);
					}
					else {
						cerr << "Error passing sprite! File: " << filename << ", sprite: " << spriteName << ", key: " << key2 << ". Value was expected to be an integer!" << endl; 
					}
				}
				if (strequal(key2,"number_of_frames")) {
					type = json_object_get_type(val2);
					if (type == json_type_int) {
						number_of_frames = json_object_get_int(val2);
					}
					else {
						cerr << "Error passing sprite! File: " << filename << ", sprite: " << spriteName << ", key: " << key2 << ". Value was expected to be an integer!" << endl; 
					}
				}
				if (strequal(key2,"frame_time")) {
					type = json_object_get_type(val2);
					if (type == json_type_int) {
						frame_time = json_object_get_int(val2);
					}
					else {
						cerr << "Error passing sprite! File: " << filename << ", sprite: " << spriteName << ", key: " << key2 << ". Value was expected to be an integer!" << endl; 
					}
				}
			}
			if (number_of_frames < 1) {
				number_of_frames = 1;
			}
			if (frame_time < 1) {
				frame_time = 1;
			}
			std::shared_ptr<sago::SagoSprite> ptr(new SagoSprite(*(data->tex),textureName,sf::IntRect(topx,topy,width,height),number_of_frames,frame_time));
			this->data->sprites[std::string(spriteName)] = ptr;
		}
		else {
			cerr << "Error passing sprite! File: " << filename << ", key: " << key << ". Value was expected to be an object!" << endl; 
		}
	}
	json_object_put(jobj);
}

void SagoSpriteHolder::ReadSprites() {
	std::vector<std::string> spritefiles = GetFileList("sprites");
	for (std::string &item : spritefiles  ) {
		if (boost::algorithm::ends_with(item,".sprite")) {
			cout << "Found " << item << endl;
			ReadSpriteFile(item);
		}
		else {
			cout << "Ignoreing " << item << endl;
		}
	}
}

const sago::SagoSprite& SagoSpriteHolder::GetSprite(const std::string &spritename) const {
	std::unordered_map<std::string,std::shared_ptr<sago::SagoSprite>>::const_iterator got = data->sprites.find (spritename);
	if ( got == data->sprites.end() ) {
		return *data->defaultSprite;
	}
	else {
		return *(got->second);
	}
}

}