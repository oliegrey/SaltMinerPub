#pragma once
class Vector2d {
public:
	int x{0};
	int y{0};

	constexpr Vector2d(const int xi, const int yi) : x{xi}, y{yi} {}

	Vector2d() = default;

	friend std::ostream& operator<<(std::ostream& out, Vector2d pos) {
		out << "(" << pos.x << ", " << pos.y << ")";
		return out;
	}

	Vector2d operator*(const Vector2d& scalar) {
		x *= scalar.x;
		y *= scalar.y;
		return *this;
	}

	constexpr Vector2d operator*(const int scalar) const {
		return {x * scalar, y * scalar};
	}

	constexpr Vector2d operator*(const Vector2d other) const {
		return {x * other.x, y * other.y};
	}

	// careful to specify width in tiles or pixels or RGB
	int flatten(int width) const { return (y * width) + x; }

	constexpr int area() const { return x * y; }
};