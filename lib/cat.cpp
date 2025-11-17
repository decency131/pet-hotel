#include "../include/cat.h"

Cat::Cat() : weight(0) {}
Cat::Cat(int ID, const std::string& name, const std::string& birthDate, const std::string& breed, const std::string& careSchedule, int weight)
    : Animal(ID, name, birthDate, breed, careSchedule), weight(weight) {}

Cat::~Cat() {}

int Cat::getWeight() const { return weight; }
void Cat::setWeight(int w) { weight = w; }

std::string Cat::makeSound() const { return "Meow!"; }

