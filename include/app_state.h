#pragma once

#include <memory>
#include <string>
#include <vector>

#include "pethotel.h"  // Your existing header with PetHotel, Animal, Reservation, Kennel

// Central place for all shared state used by UI pages.
struct AppState {
    PetHotel hotel;

    // -1 = no value, 0 = employee, 1 = client 1, 2 = client 2
    int current_user = -1;

    // 0 = main page
    // 1 = client choice page
    // 2 = client page
    // 3 = employee page
    // 4 = pet list page
    // 5 = reservation add page
    // 6 = kennel page
    // 7 = kennel add page
    // 8 = kennel remove page
    // 9 = reservation list page
    // 10 = reservation delete page
    int page_index = 0;

    // navigation history for back button
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

    // kennel add form state
    std::vector<std::string> kennel_type = {"cat/dog", "rodent"};
    int kennel_type_selected = 0;

    std::vector<std::string> kennel_size = {"small", "medium", "large"};
    int kennel_size_selected = 0;

    // kennel remove form state
    std::vector<std::shared_ptr<bool>> kennel_remove_selected;

    // reservation delete form state
    std::vector<std::shared_ptr<bool>> reservation_delete_selected;

    explicit AppState(const std::string& data_path) : hotel(data_path) {}
};

// Small helper: navigate and push into history.
inline void navigate_to(AppState& state, int new_page) {
    if (new_page == state.page_index) return;
    state.history.push_back(state.page_index);
    state.page_index = new_page;
}
