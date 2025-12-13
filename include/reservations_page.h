#pragma once

#include <ftxui/component/component.hpp>

#include "app_state.h"

ftxui::Component MakeReservationAddPage(AppState& state);     // 5
ftxui::Component MakeReservationListPage(AppState& state);    // 9
ftxui::Component MakeReservationDeletePage(AppState& state);  // 10
