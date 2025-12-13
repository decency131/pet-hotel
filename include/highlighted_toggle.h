#pragma once

#include <ftxui/component/component.hpp>
#include <string>
#include <vector>

// A toggle menu with highlighted active/selected entries.
ftxui::Component HighlightedToggle(std::vector<std::string>* entries, int* selected);
