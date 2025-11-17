#ifndef DOG_H
#define DOG_H

#include "animal.h"

class Dog : public Animal {
private:
    int weight;

public:
    Dog();
    Dog(int ID, const std::string& name, const std::string& birthDate, const std::string& breed, const std::string& careSchedule, int weight);
    ~Dog();

    int getWeight() const;
    void setWeight(int weight);

    std::string makeSound() const override;
};

#endif

