#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <vector>

#include "../include/pethotel.h"

using namespace ftxui;

int main() {
    std::string data = "../data/hotel.json";
    PetHotel hotel(data);

    // -1 = no value
    // 0 = employee
    // 1 = client 1
    // 2 = client 2
    int current_user = -1;

    auto screen = ScreenInteractive::Fullscreen();

    // 0 = main page
    // 1 = client choice page
    // 2 = client page (shows client 1 or 2 depending on current_user)
    // 3 = employee page
    // 4 = pet list page
    // 5 = reservation page
    int page_index = 0;

    // nav history for the back button
    std::vector<int> history;

    // nav functions, used for switching pages
    auto navigate_to = [&](int new_page) {
        if (new_page == page_index) return;
        history.push_back(page_index);
        page_index = new_page;
    };
    auto go_back = [&] {
        if (history.empty()) return;
        if (page_index == 2 or page_index == 3) current_user = -1;
        page_index = history.back();
        history.pop_back();
    };

    Component back_button = Button("Back", go_back);
    Component exit_button = Button("Exit", [&] { screen.Exit(); });

    Component client_button = Button("Client", [&] {
        navigate_to(1);  // go to client selection
    });

    Component employee_button = Button("Employee", [&] {
        // set current_user to employee when going to employee page
        current_user = 0;
        navigate_to(3);  // go to employee page
    });

    Component main_menu_buttons = Container::Vertical({
        client_button,
        employee_button,
    });

    Component main_menu = Renderer(main_menu_buttons, [&] {
        return vbox({
                   text("Welcome to " + hotel.get_name() + " at " + hotel.get_address()) | bold,
                   separator(),
                   text("Are you a client or an employee?"),
                   separator(),
                   main_menu_buttons->Render(),
               }) |
               border;
    });

    Component client1_button = Button("Client 1", [&] {
        current_user = 1;  // set selected client
        navigate_to(2);    // go to unified client page
    });

    Component client2_button = Button("Client 2", [&] {
        current_user = 2;  // set selected client
        navigate_to(2);    // go to unified client page
    });

    Component pet_list_button = Button("List pets", [&] { navigate_to(4); });

    Component reservation_button = Button("Make a reservation", [&] { navigate_to(5); });

    Component client_choice_container = Container::Vertical({
        client1_button,
        client2_button,
    });

    Component client_choice_page = Renderer(client_choice_container, [&] {
        return vbox({
                   text("Client selection") | bold,
                   separator(),
                   text("Please choose which client you are:"),
                   separator(),
                   client1_button->Render(),
                   client2_button->Render(),
               }) |
               border;
    });

    Component client_page_container = Container::Vertical({
        pet_list_button,
        reservation_button,
    });

    Component client_page = Renderer(client_page_container, [&] {
        Element title;
        Elements body;

        if (current_user > 0) {
            title = text("Client " + std::to_string(current_user)) | bold;
            body.push_back(text("You are client " + std::to_string(current_user)));
            body.push_back(client_page_container->Render());

        } else {
            // Fallback if somehow no client is set
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

    // employee page
    Component employee_page = Renderer([&] {
        return vbox({
                   text("Employee") | bold,
                   separator(),
                   text("This is the page for an employee."),
               }) |
               border;
    });

    Component pet_list_page = Renderer([&] {
        Element title;
        Elements tiles;

        if (current_user > 0) {
            title = text("Pets of Client " + std::to_string(current_user)) | bold;

            for (Animal* animal : hotel.get_animals()) {
                if (!animal) continue;
                if (animal->get_owner_id() != current_user) continue;

                Elements lines;

                lines.push_back(text(animal->get_name()) | bold);
                lines.push_back(separator());

                lines.push_back(text("ID: " + std::to_string(animal->get_ID())));
                lines.push_back(text("Type: " + animal->get_type()));
                lines.push_back(text("Age: " + std::to_string(animal->get_age()) + " years"));
                lines.push_back(text("Breed: " + animal->get_breed()));
                lines.push_back(text("Care schedule: " + animal->get_careSchedule()));

                std::string neighbours_str = animal->get_neighbours() ? "Yes" : "No";
                lines.push_back(text("Can have neighbours: " + neighbours_str));

                Element tile = vbox(std::move(lines)) | border | flex;

                tiles.push_back(tile);
            }

            if (tiles.empty()) {
                tiles.push_back(text("You have no pets registered.") | dim);
            }
        } else {
            title = text("Pets") | bold;
            tiles.push_back(text("No client selected.") | dim);
        }

        Elements rows;
        for (size_t i = 0; i < tiles.size(); i += 2) {
            if (i + 1 < tiles.size()) {
                rows.push_back(hbox({
                                   tiles[i] | flex,
                                   tiles[i + 1] | flex,
                               }) |
                               flex);
            } else {
                rows.push_back(hbox({
                                   tiles[i] | flex,
                                   filler(),
                               }) |
                               flex);
            }
        }

        if (rows.empty()) {
            rows.push_back(text("No pets to show.") | dim);
        }

        Element grid = vbox(std::move(rows)) | flex;

        return vbox({
                   title,
                   separator(),
                   grid,
               }) |
               border | flex;
    });

    // tab container
    Component pages = Container::Tab(
        {
            main_menu,           // 0
            client_choice_page,  // 1
            client_page,         // 2
            employee_page,       // 3
            pet_list_page,       // 4
            // reservation_page     // 5
        },
        &page_index);

    // main container
    Component main_container = Container::Vertical({
        pages,
        back_button,
        exit_button,
    });

    // app renderer
    Component app = CatchEvent(Renderer(main_container,
                                        [&] {
                                            // only show the back button when not on main page
                                            Element back_element;
                                            if (page_index == 0) {
                                                back_element = filler();
                                            } else {
                                                back_element = back_button->Render() | border;
                                            }

                                            return vbox({
                                                       pages->Render() | flex,
                                                       separator(),
                                                       hbox({
                                                           back_element,
                                                           filler(),
                                                           exit_button->Render() | border,
                                                       }),
                                                   }) |
                                                   flex;
                                        }),
                               [&](Event event) {
                                   if (event == Event::Escape) {
                                       screen.Exit();
                                       return true;
                                   }
                                   return false;
                               });

    screen.Loop(app);
    return 0;
}
