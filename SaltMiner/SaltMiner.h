#pragma once

#include <string>
#include <unordered_map>

#include <dpp/dpp.h>

#include "Player.h"
#include "UIManager.h"

class SaltMiner {
private:
    dpp::cluster* m_bot{};
    std::unordered_map<uint64_t, Player> m_players{};
    FileManager fileManager{};

    std::vector<dpp::slashcommand> m_commands{
        {"Mine", "Play Salt Miner.", m_bot->me.id},
        {"Join", "Join a specific world.", m_bot->me.id}
    };

    std::string getToken();

    void on_slashcommand(const dpp::slashcommand_t& event);

    void on_button_click(const dpp::button_click_t& event);

    uint64_t getPlayerID(const std::string componentID) {
        return std::stoull(componentID.substr(5, 23));
    }

    Player* getCachedPlayer(const std::string componentID) {
        return getCachedPlayer(getPlayerID(componentID));
    }

    Player* getCachedPlayer(dpp::snowflake id) {
        auto iterator{m_players.find(id)};
        if (iterator == m_players.end()) { return nullptr; }
        else { return &iterator->second; }
    }

    Player* getPlayer(
        dpp::guild_member member, 
        const std::string componentID, 
        bool createWorld=true
    ) { return getPlayer(member, getPlayerID(componentID), createWorld); }

    Player* getPlayer(
        dpp::guild_member member, uint64_t id, bool createWorld=true
    ) {
        Player* player{getCachedPlayer(id)};

        if (!player) { 
            player = &m_players.emplace(id, Player{member}).first->second;
            if (createWorld) { 
                player->setWorld(new World{player});
                player->fileManager.loadSeed(player->getWorld());
                player->log("created world");
            }
            player->log("added to cache.");
        }

        // set the world in player
        // add the player to 0 element in world

        return player;
    }

    // whether only the invocator of the component can interact with it
    bool isRestricted(std::string_view componentID) {
        return componentID[4] == '1';
    }

public:
    SaltMiner(): m_bot{new dpp::cluster{getToken()}} 
    {
        m_bot->on_log(dpp::utility::cout_logger());

        m_bot->on_ready([this] (const dpp::ready_t& event) {
            if (dpp::run_once<struct register_bot_commands>()) {
                m_bot->global_bulk_command_create(m_commands);
            }
        });

        m_bot->on_slashcommand([this] (const dpp::slashcommand_t& event) {
            this->on_slashcommand(event);
        });

        m_bot->on_button_click([this] (const dpp::button_click_t& event) {
            this->on_button_click(event);
        });
    }

    void start() { m_bot->start(dpp::st_wait); }

    void joinGame(Player* player, const dpp::button_click_t& event);
};