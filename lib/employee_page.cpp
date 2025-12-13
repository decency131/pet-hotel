#include "../include/employee_page.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

Component MakeEmployeePage(AppState& state) {
    Component pet_list_button = Button("List pets", [&] {
        navigate_to(state, 4);
    });

    Component reservation_add_button = Button("Make a reservation", [&] {
        navigate_to(state, 5);
    });

    Component reservation_list_button = Button("List reservations", [&] {
        navigate_to(state, 9);
    });

    Component kennel_button = Button("Kennels", [&] {
        navigate_to(state, 6);
    });

    Component employee_page_container = Container::Vertical({
        pet_list_button,
        reservation_add_button,
        reservation_list_button,
        kennel_button,
    });

    return Renderer(employee_page_container, [&, employee_page_container] {
        Element title;
        Elements body;

        if (state.current_user == 0) {
            title = text("Employee") | bold;
            body.push_back(employee_page_container->Render());
        } else {
            title = text("Employee") | bold;
            body.push_back(text("Wrong user selected."));
        }

        return vbox({
                   title,
                   separator(),
                   vbox(std::move(body)),
               }) |
               border;
    });
}
