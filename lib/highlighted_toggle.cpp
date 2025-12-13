#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "../include/highlighted_toggle.h"

using namespace ftxui;

Component HighlightedToggle(std::vector<std::string>* entries, int* selected) {
    MenuOption option = MenuOption::Toggle();

    option.entries = entries;
    option.selected = selected;

    option.entries_option.transform = [](const EntryState& state) {
        Element e = text(state.label);

        if (state.active) {
            e = e | bold | color(Color::Yellow);
        } else {
            e = e | color(Color::GrayDark);
        }

        if (state.focused) {
            e = e | underlined;
        }

        return e;
    };

    return Menu(option);
}
