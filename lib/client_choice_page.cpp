#include "../include/client_choice_page.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

Component MakeClientChoicePage(AppState& state) {
    Component client1_button = Button("Client 1", [&] {
        state.current_user = 1;
        navigate_to(state, 2);
    });

    Component client2_button = Button("Client 2", [&] {
        state.current_user = 2;
        navigate_to(state, 2);
    });

    Component client_choice_container = Container::Vertical({
        client1_button,
        client2_button,
    });

    return Renderer(client_choice_container, [&, client_choice_container] {
        return vbox({
                   text("Client selection") | bold,
                   separator(),
                   text("Please choose which client you are:"),
                   separator(),
                   client_choice_container->Render(),
               }) |
               border;
    });
}
