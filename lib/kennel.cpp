#include "../include/kennel.h"

Kennel::Kennel() : ID(0), capacity(0), empty(true) {}
Kennel::Kennel(int ID, const std::string& size, int capacity, const std::string& type, const std::vector<Animal*>& animals, bool isEmpty)
    : ID(ID), size(size), capacity(capacity), type(type), animals(animals), empty(isEmpty) {}

Kennel::~Kennel() {}

int Kennel::get_ID() const { return ID; }
std::string Kennel::get_size() const { return size; }
int Kennel::get_capacity() const { return capacity; }
std::string Kennel::get_type() const { return type; }
std::vector<Animal*> Kennel::get_animals() const { return animals; }
bool Kennel::is_empty() const { return empty; }

void Kennel::set_ID(int id) { ID = id; }
void Kennel::set_size(const std::string& s) { size = s; }
void Kennel::set_capacity(int cap) { capacity = cap; }
void Kennel::set_type(const std::string& t) { type = t; }
void Kennel::set_animals(const std::vector<Animal*>& a) { animals = a; }
void Kennel::is_empty(bool val) { empty = val; }
