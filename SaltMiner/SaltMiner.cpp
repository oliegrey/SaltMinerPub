#include <iostream>
#include <fstream>

#include <dpp/unicode_emoji.h>
#include <plog/Log.h>

#include "SaltMiner.h"

std::string SaltMiner::getToken() {
    std::ifstream iFile{"token.cfg"};
    if (!iFile) { std::ofstream createFile{"token.cfg"}; }

    std::string token{};
    if (std::getline(iFile, token)) { PLOGD << "Token found."; }
    else {
        std::cout << "Enter Discord bot token: ";
        std::getline(std::cin, token);
        std::ofstream oFile{"token.cfg"};
        oFile << token << std::endl;
        PLOGD << "Saved token.";
    }
    return token;
}

void SaltMiner::on_slashcommand_callback(const dpp::slashcommand_t& event) {
    dpp::snowflake id{event.command.member.user_id};

    if (event.command.get_command_name() == "test") {

        if (m_players.find(id) != m_players.end()) {
            m_players.insert({id, Player{event.command.member}});
        }

        Player* player{
            getPlayer(event.command.member, event.command.member.user_id)
        };

        dpp::message msg(event.command.channel_id, "");

        player->addUI(&msg, UIConfig::UIName::game);

        msg.add_file("world.jpg", player->compressedWorldImage());

        event.reply(msg);
    }
}