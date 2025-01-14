#pragma once

#include <plog/Log.h>
#include <sqlite3.h>

class Player;
class World;
class Vector2d;

class FileManager {
private:
    sqlite3* m_db{};
    const char* m_dir{"../SaltMiner/data/data.db"};
    sqlite3_stmt* statement{};

public:
    FileManager() {
        sqlite3_open(m_dir, &m_db);

        if (!m_db) { PLOGD << "Failed to load database."; }

        try {
            createTable(
                "CREATE TABLE IF NOT EXISTS player("
                "id             INT PRIMARY KEY,"      
                "x              INT NOT NULL,"
                "y              INT NOT NULL"
                ");"
            );

            createTable(
                "CREATE TABLE IF NOT EXISTS world("
                "seed           INT PRIMARY KEY,"
                "owner          INT UNIQUE NOT NULL,"
                "creationTime   DEFAULT CURRENT_TIMESTAMP"
                ");"
            );

            createTable(
                "CREATE TABLE IF NOT EXISTS destroyed("
                "id             INTEGER PRIMARY KEY AUTOINCREMENT,"
                "seed           INT UNIQUE NOT NULL,"
                "x              INT NOT NULL,"
                "y              INT NOT NULL,"
                "FOREIGN KEY(seed) REFERENCES worlds(seed)"
                ");"
                "CREATE INDEX idx_coordinates ON destroyed (seed, x, y)"
            );
        }
        catch (const std::exception& e) {
            PLOGD << e.what(); 
        }
    }

    void createTable(const char* table) {
        char* errorMessage{nullptr};
        sqlite3_exec(m_db, table, nullptr, 0, &errorMessage);
        if (errorMessage) { PLOGD << errorMessage; }
    }

    void finalize(const char* msg);
    bool finalizeLoad(const char* msg);
    bool prepareStatement(const char* sql);

    void insertPlayer(const Player& player);
    void updatePlayer(const Player& player);
    bool loadPlayer(Player* player);

    void insertWorld(const World& world);
    void updateWorld(const World& world);
    bool loadSeed(World* world);

    uint32_t getFirstEmptySeed();
    void insertDestroyed(const World& world, const Vector2d& pos);
}; 