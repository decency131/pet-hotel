#include "../include/main_menu_page.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

Component MakeMainMenuPage(AppState& state) {
    Component client_button = Button("Client", [&] {
        navigate_to(state, 1);  // client selection
    });

    Component employee_button = Button("Employee", [&] {
        state.current_user = 0;
        navigate_to(state, 3);  // employee page
    });

    Component main_menu_buttons = Container::Vertical({
        client_button,
        employee_button,
    });

    return Renderer(main_menu_buttons, [&, main_menu_buttons] {
        return vbox({
                   text("Welcome to " + state.hotel.get_name() + " at " +
                        state.hotel.get_address()) |
                       bold,
                   separator(),
                   text("Are you a client or an employee?"),
                   separator(),
                   main_menu_buttons->Render(),
               }) |
               border;
    });
}
