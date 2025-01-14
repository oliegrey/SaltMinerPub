#include "FileManager.h"
#include "Player.h"
#include "World.h"

void FileManager::finalize(const char* msg) {
    if (sqlite3_step(statement) != SQLITE_DONE) {
        PLOGD << "not " << msg << ": " << sqlite3_errmsg(m_db);
    }
    else { sqlite3_finalize(statement); }
}

bool FileManager::finalizeLoad(const char* msg) {
    if (sqlite3_step(statement) == SQLITE_ROW) {
        PLOGD << msg  << " loaded.";
        return true;
    }
    else {
        PLOGD << msg << " data not found.";
        return false;
    }
}

bool FileManager::prepareStatement(const char* sql) {
    auto result = sqlite3_prepare(m_db, sql, -1, &statement, nullptr);
    if (result != SQLITE_OK) {
        PLOGD << "failed to prepare statement: " << sqlite3_errmsg(m_db);
        return false;
    }
    else { return true; }
}

// PLAYER
void FileManager::insertPlayer(const Player& player) {
    const char* insert{"INSERT INTO player(id, x, y) VALUES(?, ?, ?)"};
    if (!prepareStatement(insert)) { return; }
    sqlite3_bind_int64(statement, 1, player.userID());
    sqlite3_bind_int(statement, 2, player.pos().x);
    sqlite3_bind_int(statement, 3, player.pos().y);
    finalize("inserted");
}

void FileManager::updatePlayer(const Player& player) {
    const char* update{"UPDATE player SET x = ?, y = ? WHERE id = ?"};
    if (!prepareStatement(update)) { return; }
    sqlite3_bind_int(statement, 1, player.pos().x);
    sqlite3_bind_int(statement, 2, player.pos().y);
    sqlite3_bind_int64(statement, 3, player.userID());
    finalize("updated");
}

bool FileManager::loadPlayer(Player* player) {
    const char* select{"SELECT id, x, y FROM player WHERE id = ?"};
    if (!prepareStatement(select)) { return false; }
    sqlite3_bind_int64(statement, 1, player->userID());
    if (finalizeLoad("player")) {
        player->pos({
            sqlite3_column_int(statement, 1), 
            sqlite3_column_int(statement, 2)
        });
    }
    else { insertPlayer(*player); }
}

// WORLD
void FileManager::insertWorld(const World& world) {
    const char* insert{"INSERT INTO world(seed, owner) VALUES(?, ?)"};
    if (!prepareStatement(insert)) { return; }
    sqlite3_bind_int(statement, 1, getFirstEmptySeed());
    sqlite3_bind_int64(statement, 2, world.getOwner()->userID());
    finalize("inserted");
}

void FileManager::updateWorld(const World& world) {
    const char* update{"UPDATE world SET owner = ? WHERE seed = ?"};
    if (!prepareStatement(update)) { return; }
    sqlite3_bind_int64(statement, 1, world.getOwner()->userID());
    sqlite3_bind_int(statement, 2, world.getSeed());
    finalize("updated");
}

bool FileManager::loadSeed(World* world) {
    const char* select{"SELECT seed FROM world WHERE owner = ?"};
    if (!prepareStatement(select)) { return false; }
    sqlite3_bind_int64(statement, 1, world->getOwner()->userID());

    if (finalizeLoad("seed")) {
        world->setSeed(sqlite3_column_int(statement, 1));   
    }
    else { insertWorld(*world); }

    return true;
}

// DESTROYED 
uint32_t FileManager::getFirstEmptySeed() {
    const char* select{"SELECT MAX(seed) FROM destroyed;"};
    prepareStatement(select);
    uint32_t result{static_cast<uint32_t>(sqlite3_column_int(statement, 1))};
    finalizeLoad("first seed");
    if (result > 2000000) {
        const char* select{"SELECT MIN(created_at) FROM destroyed;"};
        prepareStatement(select);
        return sqlite3_column_int(statement, 1);
    }
    else { return result; }
}

void FileManager::insertDestroyed(const World& world, const Vector2d& pos) {
    const char* insert{"INSERT INTO destroyed(seed, x, y) VALUES(?, ?, ?)"};
    if (!prepareStatement(insert)) { return; }
    sqlite3_bind_int(statement, 1, world.getSeed());
    sqlite3_bind_int(statement, 2, pos.x);
    sqlite3_bind_int(statement, 3, pos.y);
    finalize("inserted");
}