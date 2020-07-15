#pragma once

#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>

class TileLightState
{
private:
	int _brightness = 0;

public:
	int brightness()
	{
		return _brightness;
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