#ifndef RODENT_H
#define RODENT_H

#include "animal.h"

class Rodent : public Animal {
private:
    std::string type;

public:
    Rodent();
    Rodent(int ID, const std::string& name, const std::string& birthDate, const std::string& breed, const std::string& careSchedule, const std::string& type);
    ~Rodent();

    std::string getType() const;
    void setType(const std::string& type);

    std::string makeSound() const override;
};

#endif

