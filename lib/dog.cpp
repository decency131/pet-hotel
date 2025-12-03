#include "../include/dog.h"

Dog::Dog() : weight(0) {}
Dog::Dog(int ID, const std::string& name, int age, const std::string& breed,
         const std::string& careSchedule, bool neighbours, int owner_id, int weight)
    : Animal(ID, name, age, breed, careSchedule, neighbours, owner_id), weight(weight) {}

Dog::~Dog() {}

int Dog::get_weight() const { return weight; }
void Dog::set_weight(int w) { weight = w; }

std::string Dog::make_sound() const { return "Woof!"; }
