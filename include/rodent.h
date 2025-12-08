#ifndef RODENT_H
#define RODENT_H

#include "animal.h"

class Rodent : public Animal {
   private:
    std::string type;

   public:
    Rodent();
    Rodent(int ID, const std::string& name, int age, const std::string& breed,
           const std::string& careSchedule, bool neighbours, int owner_id, const std::string& type);
    ~Rodent();

    std::string get_rod_type() const;
    void set_rod_type(const std::string& type);

    std::string make_sound() const override;
    std::string get_type() const override;
};

#endif
