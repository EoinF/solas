#pragma once

#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>

class TileLightState
{
private:
	int _brightness = 0;

public:
	uint8_t brightness()
	{
		return (uint8_t)glm::min(_brightness, 255);
	}
	const void addLighting(int _brightness)
	{
		this->_brightness += _brightness;
	}
	void subtractLighting(int _brightness)
	{
		this->_brightness -= _brightness;
	}
	void setLighting(int _brightness)
	{
		this->_brightness = _brightness;
	}
	bool isWall = false;
};