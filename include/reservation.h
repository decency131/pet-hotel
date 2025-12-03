#ifndef RESERVATION_H
#define RESERVATION_H

#include <vector>
#include <chrono>
#include "animal.h"

class Reservation {
private:
    int ID;
    std::chrono::year_month_day startDate;
    std::chrono::year_month_day endDate;
    std::vector<Animal*> animals;

public:
    Reservation();
    Reservation(int ID, const std::string startDate, const std::string endDate, const std::vector<Animal*>& animals);
    ~Reservation();

    int get_ID() const;
    std::chrono::year_month_day get_startDate() const;
    std::chrono::year_month_day get_endDate() const;
    std::vector<Animal*> get_animals() const;

    void set_ID(int id);
    void set_startDate(const std::chrono::year_month_day date);
    void set_endDate(const std::chrono::year_month_day date);
    void set_animals(const std::vector<Animal*>& animalList);
};

#endif
