#ifndef RESERVATION_H
#define RESERVATION_H

#include <vector>
#include <string>
#include "animal.h"

class Reservation {
private:
    int ID;
    std::string startDate;
    std::string endDate;
    std::vector<Animal*> animals;
    bool putTogether;

public:
    Reservation();
    Reservation(int ID, const std::string& startDate, const std::string& endDate, const std::vector<Animal*>& animals, bool putTogether);
    ~Reservation();

    int getID() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    std::vector<Animal*> getAnimals() const;
    bool getPutTogether() const;

    void setID(int id);
    void setStartDate(const std::string& date);
    void setEndDate(const std::string& date);
    void setAnimals(const std::vector<Animal*>& animalList);
    void setPutTogether(bool val);
};

#endif

