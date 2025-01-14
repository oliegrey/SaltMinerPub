#include <iostream>
#include <fstream>
#include <memory>

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

void SaltMiner::on_slashcommand(const dpp::slashcommand_t& event) {
    dpp::snowflake id{event.command.member.user_id};

    if (event.command.get_command_name() == "mine") {

        Player* player{
            getPlayer(event.command.member, event.command.member.user_id)
        };

        dpp::message msg(event.command.channel_id, "");

        player->addUI(&msg, UIConfig::UIName::game);

        msg.add_file("world.jpg", player->compressedWorldImage());

        event.reply(msg);
    }
}

void SaltMiner::on_button_click(const dpp::button_click_t& event) {

    UIConfig::ComponentTag tag{UIManager::getComponentTag(event.custom_id)};

    Player* player{getPlayer(event.command.member, event.custom_id, tag)};
    PLOGD << player->pos();
    dpp::message msg(event.command.channel_id, "");

    if (isRestricted(event.custom_id) &&
        event.command.member.user_id != player->member().user_id) 
    { event.reply("This is not your button!"); }

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
}

void SaltMiner::joinGame(Player* player, const dpp::button_click_t& event)
{
    Player* owningPlayer{getCachedPlayer(event.custom_id)};
    if (!owningPlayer) {
        event.reply(
            dpp::message("This player is not currently online.")
            .set_flags(dpp::m_ephemeral)
        );
        return;
    }
    //owningPlayer->getWorld().connect(player);
}