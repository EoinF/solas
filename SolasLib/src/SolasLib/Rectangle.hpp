
#pragma once
#include <utility>

struct Rectangle {
	std::int64_t x, y, width, height;
	Rectangle(std::int64_t x, std::int64_t y, std::int64_t width, std::int64_t height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	bool containsPoint(std::int64_t pointX, std::int64_t pointY) {
		return pointX > this->x && pointX < this->x + this->width && pointY > this->y &&
			   pointY < this->y + height;
	}
};