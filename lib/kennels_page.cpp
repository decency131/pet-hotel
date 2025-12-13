#include "../include/kennels_page.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <string>
#include <vector>

#include "../include/highlighted_toggle.h"

using namespace ftxui;

Component MakeKennelPage(AppState& state) {
    Component kennel_add_button = Button("Add kennel", [&] { navigate_to(state, 7); });
    Component kennel_remove_button = Button("Remove kennels", [&] { navigate_to(state, 8); });
    Component kennel_add_remove = Container::Horizontal({kennel_add_button, kennel_remove_button});

    struct Cache {
        std::vector<int> ids;
        Components collapsibles;
        Component list_container;
    };
    static Cache cache;

    auto rebuild_if_needed = [&] {
        std::vector<int> new_ids;
        for (Kennel* k : state.hotel.get_kennels())
            if (k) new_ids.push_back(k->get_ID());

        if (new_ids == cache.ids && cache.list_container) return;

        cache.ids = std::move(new_ids);
        cache.collapsibles.clear();

        for (Kennel* kennel : state.hotel.get_kennels()) {
            if (!kennel) continue;

            Component child = Renderer([kennel] {
                Elements lines;
                lines.push_back(text("Type: " + kennel->get_type()));
                if (kennel->get_type() != "rodent")
                    lines.push_back(text("Size: " + kennel->get_size()));

                if (kennel->is_empty()) {
                    lines.push_back(text("Empty"));
                } else {
                    std::string pets = "Pets inside:";
                    for (auto animal : kennel->get_animals()) {
                        if (!animal) continue;
                        pets += " " + animal->get_name() + " (ID " +
                                std::to_string(animal->get_ID()) + ")";
                    }
                    lines.push_back(text(pets));
                }
                return vbox(std::move(lines)) | border;
            });

            std::string label = std::to_string(kennel->get_ID());
            cache.collapsibles.push_back(Collapsible(label, child));
        }

        cache.list_container = Container::Vertical(cache.collapsibles);
    };

    Component root = Container::Vertical({kennel_add_remove, Renderer([&] { return filler(); })});

    return Renderer(root, [&, root, kennel_add_remove] {
        Element title = text("Kennels") | bold;

        if (state.current_user != 0) {
            return vbox({
                       title,
                       separator(),
                       text("Wrong user selected.") | dim,
                   }) |
                   border | flex;
        }

        rebuild_if_needed();

        Elements tiles;
        if (!cache.list_container || cache.collapsibles.empty()) {
            tiles.push_back(text("You have no kennels.") | dim);
        } else {
            for (auto& c : cache.collapsibles) {
                tiles.push_back(c->Render() | flex);
            }
        }

        Elements rows;
        for (size_t i = 0; i < tiles.size(); i += 2) {
            if (i + 1 < tiles.size()) {
                rows.push_back(hbox({tiles[i] | flex, tiles[i + 1] | flex}) | flex);
            } else {
                rows.push_back(hbox({tiles[i] | flex, filler()}) | flex);
            }
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
}

Component MakeKennelAddPage(AppState& state) {
    Component kennel_type_toggle =
        HighlightedToggle(&state.kennel_type, &state.kennel_type_selected);
    Component kennel_size_toggle =
        HighlightedToggle(&state.kennel_size, &state.kennel_size_selected);

    Component submit_button = Button("Submit", [&] {
        // TODO
    });

    Component form = Container::Vertical({
        kennel_type_toggle,
        kennel_size_toggle,
        submit_button,
    });

    return Renderer(form, [&, form, kennel_type_toggle, kennel_size_toggle, submit_button] {
        Elements body;

        if (state.current_user == 0) {
            body.push_back(text("Select kennel type:"));
            body.push_back(kennel_type_toggle->Render());

            if (state.kennel_type_selected == 0) {  // cat/dog
                body.push_back(separator());
                body.push_back(text("Select kennel size:"));
                body.push_back(kennel_size_toggle->Render());
            }

            body.push_back(separator());
            body.push_back(submit_button->Render());
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
}

Component MakeKennelRemovePage(AppState& state) {
    state.kennel_remove_selected.clear();
    Components kennel_remove_checks;

    for (Kennel* kennel : state.hotel.get_kennels()) {
        if (!kennel) continue;

        auto selected = std::make_shared<bool>(false);
        state.kennel_remove_selected.push_back(selected);

        std::string label = "Kennel " + std::to_string(kennel->get_ID());
        kennel_remove_checks.push_back(Checkbox(label, selected.get()));
    }

    Component checks_container = Container::Vertical(kennel_remove_checks);

    Component submit_button = Button("Submit", [&] {
        // TODO
    });

    Component container = Container::Vertical({
        checks_container,
        submit_button,
    });

    return Renderer(container,
                    [&, container, kennel_remove_checks, checks_container, submit_button] {
                        Elements body;

                        if (state.current_user == 0) {
                            if (kennel_remove_checks.empty()) {
                                body.push_back(text("There are no kennels to remove.") | dim);
                            } else {
                                body.push_back(text("Select kennels to remove:"));
                                body.push_back(checks_container->Render());
                            }

                            body.push_back(separator());
                            body.push_back(submit_button->Render());
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
}
