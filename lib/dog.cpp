#include "../include/dog.h"

Dog::Dog() : weight(0) {}
Dog::Dog(int ID, const std::string& name, const std::string& birthDate, const std::string& breed, const std::string& careSchedule, int weight)
    : Animal(ID, name, birthDate, breed, careSchedule), weight(weight) {}

Dog::~Dog() {}

int Dog::getWeight() const { return weight; }
void Dog::setWeight(int w) { weight = w; }

std::string Dog::makeSound() const { return "Woof!"; }

