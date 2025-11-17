#include "../include/reservation.h"

Reservation::Reservation() : ID(0), putTogether(false) {}
Reservation::Reservation(int ID, const std::string& startDate, const std::string& endDate, const std::vector<Animal*>& animals, bool putTogether)
    : ID(ID), startDate(startDate), endDate(endDate), animals(animals), putTogether(putTogether) {}

Reservation::~Reservation() {}

int Reservation::getID() const { return ID; }
std::string Reservation::getStartDate() const { return startDate; }
std::string Reservation::getEndDate() const { return endDate; }
std::vector<Animal*> Reservation::getAnimals() const { return animals; }
bool Reservation::getPutTogether() const { return putTogether; }

void Reservation::setID(int id) { ID = id; }
void Reservation::setStartDate(const std::string& d) { startDate = d; }
void Reservation::setEndDate(const std::string& d) { endDate = d; }
void Reservation::setAnimals(const std::vector<Animal*>& v) { animals = v; }
void Reservation::setPutTogether(bool val) { putTogether = val; }

