#include "../include/cat.h"

Cat::Cat() : weight(0) {}
Cat::Cat(int ID, const std::string& name, int age, const std::string& breed,
         const std::string& careSchedule, bool neighbours, int owner_id, int weight)
    : Animal(ID, name, age, breed, careSchedule, neighbours, owner_id), weight(weight) {}

Cat::~Cat() {}

int Cat::get_weight() const { return weight; }
void Cat::set_weight(int w) { weight = w; }

std::string Cat::make_sound() const { return "Meow!"; }
