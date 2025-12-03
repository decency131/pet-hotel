#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::Fullscreen();

    // 0 = main page
    // 1 = client choice page
    // 2 = client 1
    // 3 = client 2
    // 4 = employee page
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
        navigate_to(4);  // go to employee page
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
    Component client1_button = Button("Client 1", [&] { navigate_to(2); });
    Component client2_button = Button("Client 2", [&] { navigate_to(3); });

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

    // client 1 page
    Component client1_page = Renderer([&] {
        return vbox({
                   text("Client 1") | bold,
                   separator(),
                   text("This is the page for Client 1."),
               }) |
               border;
    });

    // client 2 page
    Component client2_page = Renderer([&] {
        return vbox({
                   text("Client 2") | bold,
                   separator(),
                   text("This is the page for Client 2."),
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
            client1_page,        // 2
            client2_page,        // 3
            employee_page,       // 4
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
