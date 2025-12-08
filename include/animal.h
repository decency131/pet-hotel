#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>

class Animal {
   protected:
    int ID;
    int age;
    std::string name;
    std::string breed;
    std::string careSchedule;
    bool neighbours;
    int owner_id;

   public:
    Animal();
    Animal(int ID, const std::string& name, int age, const std::string& breed,
           const std::string& careSchedulei, bool neighbours, int owner_id);
    virtual ~Animal() = 0;

    int get_ID() const;
    int get_age() const;
    int get_owner_id() const;
    std::string get_name() const;
    std::string get_breed() const;
    std::string get_careSchedule() const;
    bool get_neighbours() const;

    void set_ID(int id);
    void set_age(int age);
    void set_owner_id(int owner_id);
    void set_name(const std::string& name);
    void set_breed(const std::string& breed);
    void set_careSchedule(const std::string& careSchedule);
    void set_neighbours(bool neighbours);

    virtual std::string make_sound() const = 0;
    virtual std::string get_type() const = 0;
};

#endif
