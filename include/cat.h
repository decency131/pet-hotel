#ifndef CAT_H
#define CAT_H

#include "animal.h"

class Cat : public Animal {
private:
    int weight;

public:
    Cat();
    Cat(int ID, const std::string& name, const std::string& birthDate, const std::string& breed, const std::string& careSchedule, int weight);
    ~Cat();

    int getWeight() const;
    void setWeight(int weight);

    std::string makeSound() const override;
};

#endif

