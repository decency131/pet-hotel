#ifndef DOG_H
#define DOG_H

#include "animal.h"

class Dog : public Animal {
   private:
    int weight;

   public:
    Dog();
    Dog(int ID, const std::string& name, int age, const std::string& breed,
        const std::string& careSchedule, bool neighbours, int owner_id, int weight);
    ~Dog();

    int get_weight() const;
    void set_weight(int weight);

    std::string make_sound() const override;
};

#endif
