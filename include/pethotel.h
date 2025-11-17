#ifndef PETHOTEL_H
#define PETHOTEL_H

#include <string>
#include <vector>
#include "animal.h"
#include "kennel.h"
#include "reservation.h"

class PetHotel {
private:
    std::string name;
    std::string address;
    std::vector<Animal*> animals;
    std::vector<Kennel*> kennels;
    std::vector<Reservation*> reservations;

public:
    PetHotel();
    PetHotel(const std::string& name, const std::string& address, const std::vector<Animal*>& animals, const std::vector<Kennel*>& kennels, const std::vector<Reservation*>& reservations);
    ~PetHotel();

    std::string getName() const;
    std::string getAddress() const;
    std::vector<Animal*> getAnimals() const;
    std::vector<Kennel*> getKennels() const;
    std::vector<Reservation*> getReservations() const;

    void setName(const std::string& n);
    void setAddress(const std::string& a);
    void setAnimals(const std::vector<Animal*>& animalList);
    void setKennels(const std::vector<Kennel*>& kennelList);
    void setReservations(const std::vector<Reservation*>& resList);
};

#endif

