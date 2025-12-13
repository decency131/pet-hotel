#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "../include/app_state.h"
#include "../include/client_choice_page.h"
#include "../include/client_page.h"
#include "../include/employee_page.h"
#include "../include/kennels_page.h"
#include "../include/main_menu_page.h"
#include "../include/pet_list_page.h"
#include "../include/reservations_page.h"

using namespace ftxui;

int main() {
    AppState state("../data/hotel.json");
    auto screen = ScreenInteractive::Fullscreen();

    Component back_button = Button("Back", [&] {
        if (state.history.empty()) return;

        // Reset user when going back from client/employee pages
        if (state.page_index == 2 || state.page_index == 3) state.current_user = -1;

        state.page_index = state.history.back();
        state.history.pop_back();
    });

    Component exit_button = Button("Exit", [&] { screen.Exit(); });

    Component pages = Container::Tab(
        {
            MakeMainMenuPage(state),               // 0
            MakeClientChoicePage(state),       // 1
            MakeClientPage(state),             // 2
            MakeEmployeePage(state),           // 3
            MakePetListPage(state),            // 4
            MakeReservationAddPage(state),     // 5
            MakeKennelPage(state),             // 6
            MakeKennelAddPage(state),          // 7
            MakeKennelRemovePage(state),       // 8
            MakeReservationListPage(state),    // 9
            MakeReservationDeletePage(state),  // 10
        },
        &state.page_index);

    Component main_container = Container::Vertical({
        pages,
        back_button,
        exit_button,
    });

    Component app = CatchEvent(Renderer(main_container,
                                        [&] {
                                            Element back_element;
                                            if (state.page_index == 0) {
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
