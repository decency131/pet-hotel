#include "../include/rodent.h"

Rodent::Rodent() {}
Rodent::Rodent(int ID, const std::string& name, const std::string& birthDate, const std::string& breed, const std::string& careSchedule, const std::string& type)
    : Animal(ID, name, birthDate, breed, careSchedule), type(type) {}

Rodent::~Rodent() {}

std::string Rodent::getType() const { return type; }
void Rodent::setType(const std::string& t) { type = t; }

std::string Rodent::makeSound() const { return "Squeak!"; }

