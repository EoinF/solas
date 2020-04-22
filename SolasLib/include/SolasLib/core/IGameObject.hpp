#pragma once

class IGameObject
{
public:
	float x = 0;
	float y = 0;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	void addLighting(float r, float g, float b) {
		this->r += r;
		this->g += g;
		this->b += b;
	}
	void subtractLighting(float r, float g, float b) {
		this->r -= r;
		this->g -= g;
		this->b -= b;
	}
	void setLighting(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	bool isWall = false;
};