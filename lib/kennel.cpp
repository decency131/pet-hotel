#include "../include/kennel.h"

Kennel::Kennel() : ID(0), capacity(0), isEmpty(true) {}
Kennel::Kennel(int ID, const std::string& size, int capacity, const std::string& type, const std::vector<Animal*>& animals, bool isEmpty)
    : ID(ID), size(size), capacity(capacity), type(type), animals(animals), isEmpty(isEmpty) {}

Kennel::~Kennel() {}

int Kennel::getID() const { return ID; }
std::string Kennel::getSize() const { return size; }
int Kennel::getCapacity() const { return capacity; }
std::string Kennel::getType() const { return type; }
std::vector<Animal*> Kennel::getAnimals() const { return animals; }
bool Kennel::getIsEmpty() const { return isEmpty; }

void Kennel::setID(int id) { ID = id; }
void Kennel::setSize(const std::string& s) { size = s; }
void Kennel::setCapacity(int cap) { capacity = cap; }
void Kennel::setType(const std::string& t) { type = t; }
void Kennel::setAnimals(const std::vector<Animal*>& a) { animals = a; }
void Kennel::setIsEmpty(bool val) { isEmpty = val; }

