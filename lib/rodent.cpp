#include "../include/rodent.h"

Rodent::Rodent() {}
Rodent::Rodent(int ID, const std::string& name, int age, const std::string& breed,
               const std::string& careSchedule, bool neighbours, int owner_id,
               const std::string& type)
    : Animal(ID, name, age, breed, careSchedule, neighbours, owner_id), type(type) {}

Rodent::~Rodent() {}

std::string Rodent::get_rod_type() const { return type; }
void Rodent::set_rod_type(const std::string& t) { type = t; }

std::string Rodent::make_sound() const { return "Squeak!"; }
std::string Rodent::get_type() const { return "rodent"; }
