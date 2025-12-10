#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
#include <vector>

#include "../include/pethotel.h"

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
    // 6 = kennel page
    // 7 = kennel add page
    int page_index = 0;

    // nav history for the back button
    std::vector<int> history;

    // reservation form state
    std::vector<std::string> clients = {"Client 1", "Client 2"};
    int client_selected = 0;

    std::vector<std::string> pet_type = {"cat", "dog", "rodent"};
    int pet_type_selected = 0;

    std::string pet_name;
    std::string pet_breed;
    std::string pet_care_schedule;
    std::string pet_rodent_type;
    std::string pet_weight;

    bool pet_neighbours = false;

    // kenel add form state
    std::vector<std::string> kennel_type = {"cat/dog", "rodent"};
    int kennel_type_selected = 0;

    std::vector<std::string> kennel_size = {"small", "medium", "large"};
    int kennel_size_selected = 0;

    // kennel remove form state
    std::vector<std::shared_ptr<bool>> kennel_remove_selected;

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

    // buttons
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

    Component client1_button = Button("Client 1", [&] {
        current_user = 1;  // set selected client
        navigate_to(2);    // go to unified client page
    });

    Component client2_button = Button("Client 2", [&] {
        current_user = 2;  // set selected client
        navigate_to(2);    // go to unified client page
    });

    Component pet_list_button = Button("List pets", [&] { navigate_to(4); });

    Component reservation_add_button = Button("Make a reservation", [&] { navigate_to(5); });

    Component reservation_submit_button = Button("Submit", [&] {
        // TODO
    });

    Component kennel_button = Button("Kennels", [&] { navigate_to(6); });

    Component kennel_add_button = Button("Add kennel", [&] { navigate_to(7); });

    Component kennel_remove_button = Button("Remove kennels", [&] { navigate_to(8); });

    Component kennel_add_submit_button = Button("Submit", [&] {
        // TODO
    });

    Component kennel_remove_submit_button = Button("Submit", [&] {
        // TODO
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
        reservation_add_button,
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

    Component employee_page_container = Container::Vertical({
        pet_list_button,
        reservation_add_button,
        kennel_button,
    });

    Component employee_page = Renderer(employee_page_container, [&] {
        Element title;
        Elements body;

        if (current_user == 0) {
            title = text("Employee") | bold;
            body.push_back(employee_page_container->Render());

        } else {
            // Fallback if somehow no client is set
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

    Component pet_list_page = Renderer([&] {
        Element title;
        Elements tiles;

        if (current_user >= 0) {
            title = text("Pets of Client " + std::to_string(current_user)) | bold;

            for (Animal* animal : hotel.get_animals()) {
                if (!animal) continue;
                if (current_user != 0 and animal->get_owner_id() != current_user) continue;

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

    Components kennel_components;

    Component kennel_add_remove = Container::Horizontal({kennel_add_button, kennel_remove_button});

    for (Kennel* kennel : hotel.get_kennels()) {
        if (!kennel) continue;

        Elements lines;

        lines.push_back(text("Type: " + kennel->get_type()));
        if (kennel->get_type() != "rodent") lines.push_back(text("Size: " + kennel->get_size()));
        if (kennel->is_empty()) {
            lines.push_back(text("Empty"));
        } else {
            std::string pets = "Pets inside: ";
            for (auto animal : kennel->get_animals()) {
                pets += " " + animal->get_name();
            }
            lines.push_back(text(pets));
        }

        Component child = Renderer([lines] { return vbox(lines) | border; });

        std::string label = std::to_string(kennel->get_ID());

        kennel_components.push_back(Collapsible(label, child));
    }

    Component kennel_list_container = Container::Vertical(kennel_components);
    Component kennel_container = Container::Vertical({kennel_add_remove, kennel_list_container});

    Component kennel_page = Renderer(kennel_container, [&] {
        Element title;
        Elements tiles;

        if (current_user == 0) {
            title = text("Kennels") | bold;

            if (kennel_components.empty()) {
                tiles.push_back(text("You have no kennels.") | dim);
            } else {
                for (auto& c : kennel_components) {
                    tiles.push_back(c->Render() | flex);
                }
            }
        } else {
            title = text("Kennels") | bold;
            tiles.push_back(text("Wrong user selected.") | dim);
        }

        Elements rows;
        for (size_t i = 0; i < tiles.size(); i += 2) {
            if (i + 1 < tiles.size()) {
                rows.push_back(hbox({
                                   tiles[i],
                                   tiles[i + 1],
                               }) |
                               flex);
            } else {
                rows.push_back(hbox({
                                   tiles[i] | xflex,
                                   filler(),
                               }) |
                               flex);
            }
        }

        if (rows.empty()) {
            rows.push_back(text("No kennels to show.") | dim);
        }

        Element grid = vbox(std::move(rows));

        return vbox({
                   title,
                   separator(),
                   kennel_add_remove->Render(),
                   separator(),
                   grid,
               }) |
               border | flex;
    });

    Component client_toggle = HighlightedToggle(&clients, &client_selected);
    Component pet_type_toggle = HighlightedToggle(&pet_type, &pet_type_selected);
    Component pet_name_input = Input("", &pet_name);
    Component pet_breed_input = Input("", &pet_breed);
    Component pet_care_schedule_input = Input("", &pet_care_schedule);
    Component pet_rodent_type_input = Input("", &pet_rodent_type);
    Component pet_weight_input = Input("", &pet_weight);
    Component pet_neighbours_check = Checkbox("Can your pet have neighbours?", &pet_neighbours);

    Component reservation_container = Container::Vertical({
        client_toggle,
        pet_type_toggle,
        pet_name_input,
        pet_breed_input,
        pet_neighbours_check,
        pet_care_schedule_input,
        pet_weight_input,
        pet_rodent_type_input,
        reservation_submit_button,
    });

    Component reservation_add_page = Renderer(reservation_container, [&] {
        Element title;
        Elements body;

        if (current_user >= 0) {
            title = text("Reservation") | bold;

            if (current_user == 0) body.push_back(client_toggle->Render());
            body.push_back(pet_type_toggle->Render());
            body.push_back(hbox({text("Pet name: "), pet_name_input->Render()}));
            body.push_back(hbox({text("Pet breed: "), pet_breed_input->Render()}));
            body.push_back(pet_neighbours_check->Render());
            body.push_back(hbox({text("Pet care schedule: "), pet_care_schedule_input->Render()}));
            if (pet_type_selected == 0 or pet_type_selected == 1)
                body.push_back(hbox({text("Pet weight: "), pet_weight_input->Render()}));
            if (pet_type_selected == 2)
                body.push_back(hbox({text("Rodent type: "), pet_rodent_type_input->Render()}));

            body.push_back(reservation_submit_button->Render());
        } else {
            // Fallback if somehow no client is set
            title = text("Reservation") | bold;
            body.push_back(text("No client selected."));
        }

        return vbox({
                   title,
                   separator(),
                   vbox(std::move(body)),
               }) |
               border;
    });

    Component kennel_type_toggle = HighlightedToggle(&kennel_type, &kennel_type_selected);
    Component kennel_size_toggle = HighlightedToggle(&kennel_size, &kennel_size_selected);

    Component kennel_add_container = Container::Vertical({
        kennel_type_toggle,
        kennel_size_toggle,  // will only be shown when type is cat/dog
        kennel_add_submit_button,
    });

    Component kennel_add_page = Renderer(kennel_add_container, [&] {
        Elements body;

        if (current_user == 0) {
            body.push_back(text("Select kennel type:"));
            body.push_back(kennel_type_toggle->Render());

            // show size choice if the type is cat or dog
            if (kennel_type_selected == 0) {
                body.push_back(separator());
                body.push_back(text("Select kennel size:"));
                body.push_back(kennel_size_toggle->Render());
            }

            body.push_back(separator());
            body.push_back(kennel_add_submit_button->Render());
        } else {
            body.push_back(text("Only employees can add kennels.") | dim);
        }

        return vbox({
                   text("Add kennel") | bold,
                   separator(),
                   vbox(std::move(body)),
               }) |
               border;
    });

    Components kennel_remove_checks;

    for (Kennel* kennel : hotel.get_kennels()) {
        if (!kennel) continue;

        std::string label = std::to_string(kennel->get_ID());

        // NEW: a checkbox for this kennel
        auto selected = std::make_shared<bool>(false);
        kennel_remove_selected.push_back(selected);

        std::string checkbox_label = "Kennel " + label;
        kennel_remove_checks.push_back(Checkbox(checkbox_label, selected.get()));
    }

    Component kennel_remove_checks_container = Container::Vertical(kennel_remove_checks);

    Component kennel_remove_container = Container::Vertical({
        kennel_remove_checks_container,
        kennel_remove_submit_button,
    });

    Component kennel_remove_page = Renderer(kennel_remove_container, [&] {
        Elements body;

        if (current_user == 0) {
            if (kennel_remove_checks.empty()) {
                body.push_back(text("There are no kennels to remove.") | dim);
            } else {
                body.push_back(text("Select kennels to remove:"));
                body.push_back(kennel_remove_checks_container->Render());
            }

            body.push_back(separator());
            body.push_back(kennel_remove_submit_button->Render());
        } else {
            body.push_back(text("Only employees can remove kennels.") | dim);
        }

        return vbox({
                   text("Remove kennels") | bold,
                   separator(),
                   vbox(std::move(body)),
               }) |
               border;
    });

    // tab container
    Component pages = Container::Tab(
        {
            main_menu,             // 0
            client_choice_page,    // 1
            client_page,           // 2
            employee_page,         // 3
            pet_list_page,         // 4
            reservation_add_page,  // 5
            kennel_page,           // 6
            kennel_add_page,       // 7
            kennel_remove_page     // 8
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
