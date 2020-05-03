#pragma once

#include <iostream>
#include <stdio.h>
//#include <glm/ext.hpp>
#include <glm/glm.hpp>

using namespace std;

class TileLightState
{
private:
	int _r = 0;
	int _g = 0;
	int _b = 0;
	int _brightness = 0;
public:
	uint8_t r()
	{
		//return (uint8_t)((glm::min(_r , 255) * glm::min(_brightness, 255)) / 255);
		return (uint8_t)glm::min(_brightness, 255);
	}
	uint8_t g()
	{
		//return (uint8_t)((glm::min(_g, 255) * glm::min(_brightness, 255)) / 255);
		return (uint8_t)glm::min(_brightness, 255);
	}
	uint8_t b()
	{
		//return (uint8_t)((glm::min(_b, 255) * glm::min(_brightness, 255)) / 255);
		return (uint8_t)glm::min(_brightness, 255);
	}
	uint8_t a()
	{
		return (uint8_t)(255 - glm::min(_brightness, 255));
	}
	void addLighting(int _r, int _g, int _b, int _brightness) {
		this->_r += _r;
		this->_g += _g;
		this->_b += _b;
		this->_brightness += _brightness;
	}
	void subtractLighting(int _r, int _g, int _b, int _brightness) {
		this->_r -= _r;
		this->_g -= _g;
		this->_b -= _b;
		this->_brightness -= _brightness;
	}
	void setLighting(int _r, int _g, int _b, int _brightness) {
		this->_r = _r;
		this->_g = _g;
		this->_b = _b;
		this->_brightness = _brightness;
	}
	bool isWall = false;
};