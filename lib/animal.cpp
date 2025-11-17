#include "../include/animal.h"

Animal::Animal() : ID(0) {}
Animal::Animal(int ID, const std::string& name, const std::string& birthDate, const std::string& breed, const std::string& careSchedule)
    : ID(ID), name(name), birthDate(birthDate), breed(breed), careSchedule(careSchedule) {}

Animal::~Animal() {}

int Animal::getID() const { return ID; }
std::string Animal::getName() const { return name; }
std::string Animal::getBirthDate() const { return birthDate; }
std::string Animal::getBreed() const { return breed; }
std::string Animal::getCareSchedule() const { return careSchedule; }

void Animal::setID(int id) { ID = id; }
void Animal::setName(const std::string& n) { name = n; }
void Animal::setBirthDate(const std::string& b) { birthDate = b; }
void Animal::setBreed(const std::string& br) { breed = br; }
void Animal::setCareSchedule(const std::string& cs) { careSchedule = cs; }

