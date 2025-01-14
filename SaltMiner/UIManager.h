#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include <dpp/dpp.h>

#include "UIConfig.h"

// componentID = <4char tag><0/1 restricted><18char discord userID>
// creates all interactable UI and processes UI input 
class UIManager {
public:

private:
	std::string m_memberID{};

	// to limit the character length of the component id
	constexpr static int tagStartID{1000};

	std::string getComponentID(
		UIConfig::ComponentTag tag, bool restricted=true
	) const { 
		return std::to_string(tag + tagStartID) + 
			std::to_string(restricted) + m_memberID;
	}

	const UIConfig::Button& getButton(UIConfig::ComponentTag tag) const {
		return UIConfig::buttons[tag]; 
	}

	const std::vector<UIConfig::Rows>& getRows(UIConfig::UIName name) 
		const { return UIConfig::UI[static_cast<size_t>(name)]; }

	dpp::component addButton(UIConfig::ComponentTag tag) const;

	void addButtonRow(dpp::message* msg, const UIConfig::RowTags& tags) const;

public:
	UIManager() = delete;

	UIManager(dpp::snowflake memberID): m_memberID{std::to_string(memberID)} {}

	static const UIConfig::ComponentTag getComponentTag(
		std::string componentID
	) {
		return static_cast<UIConfig::ComponentTag>
			(std::stoi(componentID.substr(0, 4)) - tagStartID);
	}

	void addUI(dpp::message* msg, UIConfig::UIName name) const;
};