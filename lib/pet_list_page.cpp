#include "../include/pet_list_page.h"

#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <sstream>

using namespace ftxui;

static std::string FormatDate(const std::chrono::year_month_day& d) {
    std::ostringstream oss;
    oss << int(d.year()) << "-" << unsigned(d.month()) << "-" << unsigned(d.day());
    return oss.str();
}

static std::string IsAnimalStaying(AppState& state, const Animal* animal) {
    using namespace std::chrono;
    if (!animal) return "";

    for (Reservation* res : state.hotel.get_reservations()) {
        if (!res) continue;

        for (Animal* a : res->get_animals()) {
            if (!a) continue;
            if (a->get_ID() == animal->get_ID()) {
                return FormatDate(res->get_startDate()) + " to " + FormatDate(res->get_endDate());
            }
        }
    }

    return "";
}

Component MakePetListPage(AppState& state) {
    return Renderer([&] {
        Element title;
        Elements tiles;

        if (state.current_user >= 0) {
            title = text("Pets of Client " + std::to_string(state.current_user)) | bold;

            for (Animal* animal : state.hotel.get_animals()) {
                if (!animal) continue;
                if (state.current_user != 0 && animal->get_owner_id() != state.current_user)
                    continue;

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

                std::string staying = IsAnimalStaying(state, animal);
                std::string staying_str = !staying.empty() ? staying : "No";
                lines.push_back(text("Staying at the hotel: " + staying_str));

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
}
