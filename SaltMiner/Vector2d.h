#pragma once
class Vector2d {
public:
	int x{0};
	int y{0};

	Vector2d() = default;
	Vector2d(int xi, int yi): x{xi}, y{yi} {}

	friend std::ostream& operator<<(std::ostream& out, Vector2d pos) {
		out << "(" << pos.x << ", " << pos.y << ")";
		return out;
	}
};