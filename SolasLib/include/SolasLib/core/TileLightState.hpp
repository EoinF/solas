#pragma once

#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>

class TileLightState {
  private:
	int _brightness = 0;

  public:
	int brightness() const { return _brightness; }
	void addLighting(int _brightness) { this->_brightness += _brightness; }
	void subtractLighting(int _brightness) { this->_brightness -= _brightness; }
	void setLighting(int _brightness) { this->_brightness = _brightness; }
	bool isWall = false;
};
