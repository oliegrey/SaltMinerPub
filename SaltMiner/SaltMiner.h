#pragma once

#include <string>
#include <unordered_map>

#include <dpp/dpp.h>

#include "Player.h"
#include "UIManager.h"

class SaltMiner {
private:
    dpp::cluster* m_bot{};
    std::unordered_map<dpp::snowflake, Player> m_players{};

    std::vector<dpp::slashcommand> m_commands{
        {"Mine", "Play Salt Miner.", m_bot->me.id},
        {"test", "test command", m_bot->me.id}
    };

    std::string getToken();

    void on_slashcommand_callback(const dpp::slashcommand_t& event);

    dpp::snowflake getPlayerID(std::string componentID) {
        return static_cast<dpp::snowflake>(std::stoll(componentID.substr(4)));
    }

    Player* getPlayer(dpp::guild_member member, std::string componentID) {
        dpp::snowflake id{getPlayerID(componentID)};
        return getPlayer(member, id);
    }

    Player* getPlayer(dpp::guild_member member, dpp::snowflake id) {
        auto iterator{m_players.find(id)};

        if (iterator == m_players.end()) { 
            return &m_players.emplace(id, Player{member}).first->second;
        }

        PLOGD << "Player found!";
        return &iterator->second;
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
            this->on_slashcommand_callback(event);
        });

        m_bot->on_button_click([this] (const dpp::button_click_t& event) {
            
            Player* player{
                getPlayer(event.command.member, event.custom_id)
            };
            PLOGD << player->pos();
            dpp::message msg(event.command.channel_id,  "");

            auto tag{UIManager::getComponentTag(event.custom_id)};
            switch (tag) {
            case UIConfig::left:
            case UIConfig::right:
            case UIConfig::down:
                player->move(tag);

                player->addUI(&msg, UIConfig::UIName::game);

                msg.add_file("world.jpg", player->compressedWorldImage());

                event.reply(dpp::ir_update_message, msg);
                break;
            case UIConfig::portal:
                break;
            case UIConfig::backpack:
                break;
            }
        });
    }

    void start() { m_bot->start(dpp::st_wait); }
};