#pragma once
#include <utility>

#include <dpp/dpp.h>

#include "World.h"
#include "Vector2d.h"
#include "UIManager.h"

class Player {
private:
	dpp::guild_member m_member{};
	World m_world{};
	Vector2d m_pos{};
	UIManager UI;
	
public:
	Player() = delete;

	Player(dpp::guild_member member): 
		m_member{member}, UI{member.user_id}
	{}

	Player& operator=(const Player& other) noexcept {
		if (&other != this) {
			m_member = other.m_member;
		}
		return *this;
	}

	Vector2d pos() const { return m_pos; }

	const uint8_t* worldImage() const { return m_world.image(); }

	void addUI(dpp::message* msg, UIConfig::UIName name) {
		UI.addUI(msg, name);
	}

	const std::string_view compressedWorldImage() const {
		return m_world.compressedImage();
	}

	void move(UIConfig::ComponentTag tag) {
		switch (tag) {
		case UIConfig::left:	
			m_pos.x -= 1;
			break;
		case UIConfig::right:	
			m_pos.x += 1;
			break;
		case UIConfig::down:
			m_pos.y += 1;
			break;
		}
		m_world.clear();
		m_world.drawGuy(m_pos);
	}
};

