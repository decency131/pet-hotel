#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
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
        page_index = history.back();
        history.pop_back();
    };

    // back and exit buttons
    Component back_button = Button("Back", go_back);
    Component exit_button = Button("Exit", [&] { screen.Exit(); });

    // main page
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
                   text("Welcome") | bold,
                   separator(),
                   text("Are you a client or an employee?"),
                   separator(),
                   main_menu_buttons->Render(),
               }) |
               border;
    });

    // client choice page
    Component client1_button = Button("Client 1", [&] {
        current_user = 1;  // set selected client
        navigate_to(2);    // go to unified client page
    });

    Component client2_button = Button("Client 2", [&] {
        current_user = 2;  // set selected client
        navigate_to(2);    // go to unified client page
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

    // unified client page – content depends on current_user
    Component client_page = Renderer([&] {
        Element title;
        Element body;

        if (current_user == 1) {
            title = text("Client 1") | bold;
            body = text("This is the page for Client 1.");
        } else if (current_user == 2) {
            title = text("Client 2") | bold;
            body = text("This is the page for Client 2.");
        } else {
            // Fallback if somehow no client is set
            title = text("Client") | bold;
            body = text("No client selected.");
        }

        return vbox({
                   title,
                   separator(),
                   body,
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

    // tab container
    Component pages = Container::Tab(
        {
            main_menu,           // 0
            client_choice_page,  // 1
            client_page,         // 2
            employee_page,       // 3
        },
        &page_index);

    // main container
    Component main_container = Container::Vertical({
        pages,
        back_button,
        exit_button,
    });

    // app renderer
    Component app = Renderer(main_container, [&] {
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
    });

    screen.Loop(app);
    return 0;
}
