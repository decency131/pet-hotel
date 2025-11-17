#include "../include/pethotel.h"

PetHotel::PetHotel() {}
PetHotel::PetHotel(const std::string& n, const std::string& a, const std::vector<Animal*>& ani, const std::vector<Kennel*>& ken, const std::vector<Reservation*>& res)
    : name(n), address(a), animals(ani), kennels(ken), reservations(res) {}

PetHotel::~PetHotel() {}

std::string PetHotel::getName() const { return name; }
std::string PetHotel::getAddress() const { return address; }
std::vector<Animal*> PetHotel::getAnimals() const { return animals; }
std::vector<Kennel*> PetHotel::getKennels() const { return kennels; }
std::vector<Reservation*> PetHotel::getReservations() const { return reservations; }

void PetHotel::setName(const std::string& n) { name = n; }
void PetHotel::setAddress(const std::string& a) { address = a; }
void PetHotel::setAnimals(const std::vector<Animal*>& v) { animals = v; }
void PetHotel::setKennels(const std::vector<Kennel*>& v) { kennels = v; }
void PetHotel::setReservations(const std::vector<Reservation*>& v) { reservations = v; }

