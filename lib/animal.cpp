#include "../include/animal.h"

Animal::Animal() : ID(0) {}
Animal::Animal(int ID, const std::string& name, int age, const std::string& breed,
               const std::string& careSchedule, bool neighbours, int owner_id)
    : ID(ID),
      name(name),
      age(age),
      breed(breed),
      careSchedule(careSchedule),
      neighbours(neighbours),
      owner_id(owner_id) {}

Animal::~Animal() {}

int Animal::get_ID() const { return ID; }
int Animal::get_age() const { return age; }
int Animal::get_owner_id() const { return owner_id; }
std::string Animal::get_name() const { return name; }
std::string Animal::get_breed() const { return breed; }
std::string Animal::get_careSchedule() const { return careSchedule; }
bool Animal::get_neighbours() const { return neighbours; }

void Animal::set_ID(int id) { ID = id; }
void Animal::set_age(int a) { age = a; }
void Animal::set_owner_id(int oid) { owner_id = oid; }
void Animal::set_name(const std::string& n) { name = n; }
void Animal::set_breed(const std::string& br) { breed = br; }
void Animal::set_careSchedule(const std::string& cs) { careSchedule = cs; }
void Animal::set_neighbours(const bool n) { neighbours = n; }
