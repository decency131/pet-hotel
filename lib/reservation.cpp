#include "../include/reservation.h"
#include <chrono>
#include <cstdio>
#include <string>

Reservation::Reservation() : ID(0) {}
Reservation::Reservation(int ID, const std::string start_date, const std::string end_date, const std::vector<Animal*>& animals)
    : ID(ID), animals(animals) {
        int ys, ms, ds, ye, me, de;
        std::sscanf(start_date.c_str(), "%d-%d-%d", &ys, &ms, &ds);
        std::sscanf(end_date.c_str(), "%d-%d-%d", &ye, &me, &de);
        startDate = {std::chrono::year{ys}, std::chrono::month{(unsigned)ms},  std::chrono::day{(unsigned)ds}};
        endDate = {std::chrono::year{ye}, std::chrono::month{(unsigned)me},  std::chrono::day{(unsigned)de}};
    }

Reservation::~Reservation() {}

int Reservation::get_ID() const { return ID; }
std::chrono::year_month_day Reservation::get_startDate() const { return startDate; }
std::chrono::year_month_day Reservation::get_endDate() const { return endDate; }
std::vector<Animal*> Reservation::get_animals() const { return animals; }

void Reservation::set_ID(int id) { ID = id; }
void Reservation::set_startDate(const std::chrono::year_month_day d) { startDate = d; }
void Reservation::set_endDate(const std::chrono::year_month_day d) { endDate = d; }
void Reservation::set_animals(const std::vector<Animal*>& v) { animals = v; }
