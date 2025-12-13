#include "../include/client_page.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

Component MakeClientPage(AppState& state) {
    Component pet_list_button = Button("List pets", [&] { navigate_to(state, 4); });

    Component reservation_add_button = Button("Make a reservation", [&] { navigate_to(state, 5); });

    Component client_page_container = Container::Vertical({
        pet_list_button,
        reservation_add_button,
    });

    return Renderer(client_page_container, [&, client_page_container] {
        Element title;
        Elements body;

        if (state.current_user > 0) {
            title = text("Client " + std::to_string(state.current_user)) | bold;
            body.push_back(text("You are client " + std::to_string(state.current_user)));
            body.push_back(client_page_container->Render());
        } else {
            title = text("Client") | bold;
            body.push_back(text("No client selected."));
        }

        return vbox({
                   title,
                   separator(),
                   vbox(std::move(body)),
               }) |
               border;
    });
}
