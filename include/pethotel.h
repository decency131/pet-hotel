#ifndef PETHOTEL_H
#define PETHOTEL_H

#include <string>
#include <vector>

#include "animal.h"
#include "cat.h"
#include "dog.h"
#include "kennel.h"
#include "reservation.h"
#include "rodent.h"

class PetHotel {
   private:
    std::string name;
    std::string address;
    std::vector<Animal*> animals;
    std::vector<Kennel*> kennels;
    std::vector<Reservation*> reservations;
    std::string filename;

   public:
    PetHotel();
    PetHotel(const std::string& jsonfile);
    ~PetHotel();

    std::string get_name() const;
    std::string get_address() const;
    std::vector<Animal*> get_animals() const;
    std::vector<Kennel*> get_kennels() const;
    std::vector<Reservation*> get_reservations() const;

    void set_name(const std::string& n);
    void set_address(const std::string& a);
    void set_animals(const std::vector<Animal*>& animalList);
    void set_kennels(const std::vector<Kennel*>& kennelList);
    void set_reservations(const std::vector<Reservation*>& resList);
};

#endif
