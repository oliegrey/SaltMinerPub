#pragma once
#include <dpp/dpp.h>
#include <vector>

namespace UIConfig {
	// names of components/buttons
	enum ComponentTag: int {
		left,
		down,
		right,
		portal,
		backpack,
		settings,
		maxComponentTag
	};

	struct Button {
		dpp::emoji emoji{};
		std::string_view label{};
		dpp::component_style style{dpp::cos_secondary};
		bool disabled{false};
		bool restricted{true};

		Button(dpp::emoji e): emoji{e} {}

		Button(std::string_view l): label{l} {}

		Button(std::string_view l, bool r):
			label{l},
			style{r?dpp::cos_secondary :dpp::cos_primary},
			restricted{r} {}

		Button(dpp::emoji e, bool r):
			emoji{e},
			style{r?dpp::cos_secondary :dpp::cos_primary},
			restricted{r} {}
	};

	// buttons
	inline const Button buttons[maxComponentTag]{
		{dpp::emoji{"◀️", 0}},
		{dpp::emoji{"🔽", 0}},
		{dpp::emoji{"▶️", 0}},
		{dpp::emoji{"portal", 991751523342225500}},
		{dpp::emoji{"🎒", 0}},
		{dpp::emoji{"⚙️", 0}}
	};

	// types of action rows
	enum class RowType {buttons, select};

	// maps onto UI
	enum class UIName: int {game, settings, maxUIName};

	using RowTags = std::vector<ComponentTag>;

	struct Rows {
		RowType type{};
		RowTags tags{};
	};

	// groups of rows of interactable components belonging to UIs
	const std::vector<Rows> UI[static_cast<size_t>(UIName::maxUIName)]{
		{
			{RowType::buttons, {left, down, right, portal, backpack}},
			{RowType::buttons, {settings}}
		},
		{
			{RowType::buttons, {portal, portal, backpack}},
			{RowType::buttons, {settings, settings}}
		},
	};
}