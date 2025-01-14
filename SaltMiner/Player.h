#pragma once

#include <utility>
#include <memory>

#include <dpp/dpp.h>

#include "World.h"
#include "Vector2d.h"
#include "UIManager.h"
#include "FileManager.h"

class Player {
public:
	FileManager fileManager{};

private:
	dpp::guild_member m_member{};
	World* m_world{};
	Vector2d m_pos{};
	UIManager m_UI;
	
public:
	Player() = delete;

	Player(dpp::guild_member member): 
		m_member{member}, m_UI{member.user_id}
	{ fileManager.loadPlayer(this); }

	~Player() { 
		if (m_world) { disconnect(); } 
	}

	Player& operator=(const Player& other) noexcept {
		if (&other != this) { m_member = other.m_member; }
		return *this;
	}

	const Vector2d& pos() const { return m_pos; }

	void pos(const Vector2d& pos) { m_pos = pos; }

	const dpp::guild_member& member(){ return m_member; }

	const uint64_t userID() const { return m_member.user_id; }

	World* getWorld() const { return m_world; }

	void setWorld(World* world) { m_world = world; }

	const uint8_t* worldImage() const { return m_world->image(); }

	void log(std::string_view msg) const {
		PLOGD << "[user: " << m_member.user_id << "]" 
			"[world: " << m_world->getSeed() << "] " << msg;
	}

	// owner called: remove all world pointers from players & deallocate world memory 
	// otherwise: remove world pointer from caller, remove player pointer from world
	void disconnect() { 
		if (!m_world) { 
			log("no world to disconnect from."); 
		}
		else if (m_world->isOwner(this)) {
			m_world->clearConnections();
			delete m_world;
			log("owner disconnected.");
		}
		else {
			log("guest disconnected.");
			m_world->disconnect(this);
		}
	}

	void connect(Player* owner) {
		if (owner->m_world == m_world) { log("player already connected."); }
		else {
			disconnect();
			m_world->connect(this);
		}
	}

	void addUI(dpp::message* msg, UIConfig::UIName name) {
		m_UI.addUI(msg, name);
	}

	const std::string_view compressedWorldImage() const {
		return m_world->compressedImage();
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
		m_world->clear();
		m_world->drawGuy(m_pos);

		fileManager.updatePlayer(*this);
	}
};
