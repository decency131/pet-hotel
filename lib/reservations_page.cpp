#include "../include/reservations_page.h"

#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../include/highlighted_toggle.h"

using namespace ftxui;

Component MakeReservationAddPage(AppState& state) {
    Component client_toggle = HighlightedToggle(&state.clients, &state.client_selected);
    Component pet_type_toggle = HighlightedToggle(&state.pet_type, &state.pet_type_selected);
    Component pet_name_input = Input("", &state.pet_name);
    Component pet_breed_input = Input("", &state.pet_breed);
    Component pet_care_schedule_input = Input("", &state.pet_care_schedule);
    Component pet_rodent_type_input = Input("", &state.pet_rodent_type);
    Component pet_weight_input = Input("", &state.pet_weight);
    Component pet_neighbours_check =
        Checkbox("Can your pet have neighbours?", &state.pet_neighbours);

    Component submit_button = Button("Submit", [&] {
        // TODO: create or update reservation + possibly new pet
        // All form values are in state.*
    });

    Component container = Container::Vertical({
        client_toggle,
        pet_type_toggle,
        pet_name_input,
        pet_breed_input,
        pet_neighbours_check,
        pet_care_schedule_input,
        pet_weight_input,
        pet_rodent_type_input,
        submit_button,
    });

    return Renderer(container, [&, container, client_toggle, pet_type_toggle, pet_name_input,
                                pet_breed_input, pet_neighbours_check, pet_care_schedule_input,
                                pet_weight_input, pet_rodent_type_input, submit_button] {
        Element title;
        Elements body;

        if (state.current_user >= 0) {
            title = text("Reservation") | bold;

            if (state.current_user == 0) body.push_back(client_toggle->Render());

            body.push_back(pet_type_toggle->Render());
            body.push_back(hbox({text("Pet name: "), pet_name_input->Render()}));
            body.push_back(hbox({text("Pet breed: "), pet_breed_input->Render()}));
            body.push_back(pet_neighbours_check->Render());
            body.push_back(hbox({text("Pet care schedule: "), pet_care_schedule_input->Render()}));

            if (state.pet_type_selected == 0 || state.pet_type_selected == 1) {
                body.push_back(hbox({text("Pet weight: "), pet_weight_input->Render()}));
            }
            if (state.pet_type_selected == 2) {
                body.push_back(hbox({text("Rodent type: "), pet_rodent_type_input->Render()}));
            }

            body.push_back(submit_button->Render());
        } else {
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
}

Component MakeReservationDeletePage(AppState& state) {
    state.reservation_delete_selected.clear();
    Components reservation_delete_checks;

    for (Reservation* res : state.hotel.get_reservations()) {
        if (!res) continue;

        auto selected = std::make_shared<bool>(false);
        state.reservation_delete_selected.push_back(selected);

        std::string label = "Reservation " + std::to_string(res->get_ID());
        reservation_delete_checks.push_back(Checkbox(label, selected.get()));
    }

    Component checks_container = Container::Vertical(reservation_delete_checks);

    Component submit_button = Button("Submit", [&] {
        // TODO
    });

    Component container = Container::Vertical({
        checks_container,
        submit_button,
    });

    return Renderer(container,
                    [&, container, reservation_delete_checks, checks_container, submit_button] {
                        Elements body;

                        if (state.current_user == 0) {
                            if (reservation_delete_checks.empty()) {
                                body.push_back(text("There are no reservations to remove.") | dim);
                            } else {
                                body.push_back(text("Select reservations to remove:"));
                                body.push_back(checks_container->Render());
                            }

                            body.push_back(separator());
                            body.push_back(submit_button->Render());
                        } else {
                            body.push_back(text("Only employees can remove reservations.") | dim);
                        }

                        return vbox({
                                   text("Remove reservations") | bold,
                                   separator(),
                                   vbox(std::move(body)),
                               }) |
                               border;
                    });
}

static std::string FormatDate(const std::chrono::year_month_day& d) {
    std::ostringstream oss;
    oss << int(d.year()) << "-" << unsigned(d.month()) << "-" << unsigned(d.day());
    return oss.str();
}
Component MakeReservationListPage(AppState& state) {
    Component add_button = Button("Make a reservation", [&] { navigate_to(state, 5); });

    Component delete_button = Button("Delete reservations", [&] { navigate_to(state, 10); });

    Component add_remove = Container::Horizontal({add_button, delete_button});

    struct Cache {
        std::vector<int> ids;
        Components collapsibles;
    };
    static Cache cache;

    auto rebuild_if_needed = [&] {
        std::vector<int> new_ids;
        for (Reservation* r : state.hotel.get_reservations())
            if (r) new_ids.push_back(r->get_ID());

        if (new_ids == cache.ids) return;

        cache.ids = std::move(new_ids);
        cache.collapsibles.clear();

        for (Reservation* res : state.hotel.get_reservations()) {
            if (!res) continue;
            Component child = Renderer([res] {
                Elements lines;
                lines.push_back(text("Reservation ID: " + std::to_string(res->get_ID())) | bold);
                lines.push_back(separator());

                lines.push_back(text("Start date: " + FormatDate(res->get_startDate())));
                lines.push_back(text("End date:   " + FormatDate(res->get_endDate())));

                if (res->get_animals().empty()) {
                    lines.push_back(text("Animals: none") | dim);
                } else {
                    lines.push_back(text("Animals:"));
                    for (Animal* a : res->get_animals()) {
                        if (!a) continue;
                        lines.push_back(text("  - " + a->get_name() + " (ID " +
                                             std::to_string(a->get_ID()) + ")"));
                    }
                }

                return vbox(std::move(lines)) | border;
            });

            std::string label = "Reservation " + std::to_string(res->get_ID());
            cache.collapsibles.push_back(Collapsible(label, child));
        }
    };

    Component root = Container::Vertical({
        add_remove, Renderer([&] { return filler(); })
    });

    return Renderer(root, [&, root, add_remove] {
        rebuild_if_needed();

        Elements tiles;
        if (cache.collapsibles.empty()) {
            tiles.push_back(text("There are no reservations.") | dim);
        } else {
            for (auto& c : cache.collapsibles) tiles.push_back(c->Render() | flex);
        }

        return vbox({
                   text("Reservations") | bold,
                   separator(),
                   add_remove->Render(),
                   separator(),
                   vbox(std::move(tiles)),
               }) |
               border | flex;
    });
}
