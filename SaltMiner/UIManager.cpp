#include <string>

#include <plog/Log.h>

#include "UIManager.h"

dpp::component UIManager::addButton(UIConfig::ComponentTag tag) const {
    const UIConfig::Button& button{getButton(tag)};
    PLOGD << button.emoji.name << ", " << button.emoji.id << ", " << button.label << ".";

    return dpp::component()
        .set_label(button.label)
        .set_emoji(button.emoji.name, button.emoji.id)
        .set_style(button.style)
        .set_id(getComponentID(tag, button.restricted))
        .set_disabled(button.disabled);
}

void UIManager::addButtonRow(
    dpp::message* msg, const UIConfig::RowTags& tags
) const {
    dpp::component component{};
    for (int i{}; i < tags.size(); ++i) {
        component.add_component(addButton(tags[i]));
    }
    msg->add_component(component);
}

void UIManager::addUI(dpp::message* msg, UIConfig::UIName name) const {
    const std::vector<UIConfig::Rows>& rows{getRows(name)};

    size_t length{rows.size()};
    assert(length <= 5);

    for (int i{}; i < length; ++i) {
        const UIConfig::Rows& row{rows[i]};
        if (row.type == UIConfig::RowType::buttons) {
            addButtonRow(msg, row.tags);
        }
    }
}

// add <specials> and <interaction> select menus

//msg.add_component(
//    dpp::component().add_component(
//        dpp::component()
//        .set_type(dpp::cot_selectmenu)
//        .set_placeholder("Pick something")
//        .add_select_option(
//            dpp::select_option("label1", "value1", "description1")
//            .set_emoji(dpp::unicode_emoji::smile)
//        )
//        .add_select_option(
//            dpp::select_option("label2", "value2", "description2")
//            .set_emoji(dpp::unicode_emoji::slight_smile)
//        )
//        .set_id("4321")
//    )
//);