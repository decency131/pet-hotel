#include <chrono>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
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
    name = j.value("name", "hotel");
    address = j.value("address", "middle of nowhere");

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

PetHotel::~PetHotel() {
    json j;
    
    j["name"] = name;
    j["address"] = address;
    
    for (auto* a : animals){
        json ja;
        ja["id"] = a->get_ID();
        ja["age"] = a->get_age();
        ja["owner_id"] = a->get_owner_id();
        ja["name"] = a->get_name();
        ja["breed"] = a->get_breed();
        ja["care"] = a->get_careSchedule();
        ja["neighbours"] = a->get_neighbours();
        
        if(dynamic_cast<Dog*>(a)){
            auto* d = (Dog*)a;
            ja["type"] = "dog";
            ja["weight"] = d->get_weight();
        }
        if(dynamic_cast<Cat*>(a)){
            auto* c = (Cat*)a;
            ja["type"] = "cat";
            ja["weight"] = c->get_weight();
        }
        if(dynamic_cast<Rodent*>(a)){
            auto* r = (Rodent*)a;
            ja["type"] = "dog";
            ja["rodent_type"] = r->get_type();
        }
        
        j["animals"].push_back(ja);
    }
    
    for (auto* k : kennels){
        json jk;
        
        jk["id"] = k->get_ID();
        jk["size"] = k->get_size();
        for (auto* a : k->get_animals()){
            jk["animals"].push_back(a->get_ID());
        }
        
        j["kennels"].push_back(jk);
    }
    
    for (auto* r : reservations){
        json jr;
        
        jr["id"] = r->get_ID();
        
        std::chrono::year_month_day start_date = r->get_startDate();
        std::chrono::year_month_day end_date = r->get_endDate();
        std::ostringstream start_date_s;
        std::ostringstream end_date_s;
        
        start_date_s << int(start_date.year()) << "-" << unsigned(start_date.month()) << "-" << unsigned(start_date.day());
        end_date_s << int(end_date.year()) << "-" << unsigned(end_date.month()) << "-" << unsigned(end_date.day());

        for (auto* a : r->get_animals()){
            jr["animals"].push_back(a->get_ID());
        }
        
        j["reservations"].push_back(jr);
    }
    
    {
        std::ofstream out(filename);
        out << j.dump(4);
    }
}

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
