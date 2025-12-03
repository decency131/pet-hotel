#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

#include "../include/pethotel.h"

using nlohmann::json;

Animal* make_animal(json& ja) {
    int id = ja.value("id", 0);
    int age = ja.value("age", 0);
    int owner_id = ja.value("owner_id", 0);
    std::string name = ja.value("name", "name");
    std::string type = ja.value("type", "unicorn");
    std::string breed = ja.value("breed", "unicorn");
    std::string care = ja.value("care", "feed(optional)");
    bool neighbours = ja.value("neighbours", true);

    if (type == "unicorn") {
        std::cerr << "something is wrong with json\n";
        std::exit(1);
    }
    if (type == "dog") {
        return new Dog(id, name, age, breed, care, neighbours, owner_id, ja.value("weight", 0));
    }
    if (type == "cat") {
        return new Cat(id, name, age, breed, care, neighbours, owner_id, ja.value("weight", 0));
    }
    if (type == "rodent") {
        return new Rodent(id, name, age, breed, care, neighbours, owner_id,
                          ja.value("rodent_type", "capybara"));
    }
    // default return
    return new Rodent(id, name, age, breed, care, neighbours, owner_id,
                      ja.value("rodent_type", "capybara"));
}

PetHotel::PetHotel() {}
PetHotel::PetHotel(const std::string& jsonfile) : filename(jsonfile) {
    json j;
    {  // opening the file and sending
        std::ifstream f(jsonfile);
        if (!f.is_open()) {
            std::cerr << "could not open " << jsonfile << "\n";
            std::exit(0);
        }
        try {
            f >> j;
        } catch (const std::exception& e) {
            std::cerr << e.what();
            std::exit(0);
        }
    }

    std::unordered_map<int, Animal*> animals_by_id;
    // filling the animals vector
    for (json& ja : j["animals"]) {
        Animal* a = make_animal(ja);
        animals.push_back(a);
        animals_by_id[a->get_ID()] = a;
    }

    for (json& jk : j["kennels"]) {
        int id = jk.value("id", 0);
        std::string size = jk.value("size", "small");
        int capacity;
        std::string type = "cat/dog";

        // rodents have their own size, for cats and dogs capacity means that,
        //   for example, a big kennel can host 1 big dog(4 points), or 2 medium sized(2 points
        //   each), or 4 small ones(1 point each)
        if (size == "rodent") {
            capacity = 4;
            type = "rodent";
        }
        if (size == "small") capacity = 1;
        if (size == "medium") capacity = 2;
        if (size == "big") capacity = 4;

        std::vector<Animal*> inside;
        for (int aid : jk["animals"]) {
            if (animals_by_id.count(aid)) inside.push_back(animals_by_id[aid]);
        }
        bool empty = inside.empty();

        kennels.push_back(new Kennel(id, size, capacity, type, inside, empty));
    }

    for (json& jr : j["reservations"]) {
        int id = jr.value("id", 0);
        std::string startDate = jr.value("start", "0000-00-00");
        std::string endDate = jr.value("end", "0000-00-00");

        std::vector<Animal*> pets;
        for (int aid : jr["animals"]) {
            if (animals_by_id.count(aid)) pets.push_back(animals_by_id[aid]);
        }

        reservations.push_back(new Reservation(id, startDate, endDate, pets));
    }
}

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
