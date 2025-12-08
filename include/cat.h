#ifndef CAT_H
#define CAT_H

#include "animal.h"

class Cat : public Animal {
   private:
    int weight;

   public:
    Cat();
    Cat(int ID, const std::string& name, int age, const std::string& breed,
        const std::string& careSchedule, bool neighbours, int owner_id, int weight);
    ~Cat();

    int get_weight() const;
    void set_weight(int weight);

    std::string make_sound() const override;
    std::string get_type() const override;
};

#endif
