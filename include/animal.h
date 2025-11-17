#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>

class Animal {
protected:
    int ID;
    std::string name;
    std::string birthDate;
    std::string breed;
    std::string careSchedule;

public:
    Animal();
    Animal(int ID, const std::string& name, const std::string& birthDate, const std::string& breed, const std::string& careSchedule);
    virtual ~Animal() = 0;

    int getID() const;
    std::string getName() const;
    std::string getBirthDate() const;
    std::string getBreed() const;
    std::string getCareSchedule() const;

    void setID(int id);
    void setName(const std::string& name);
    void setBirthDate(const std::string& birthDate);
    void setBreed(const std::string& breed);
    void setCareSchedule(const std::string& careSchedule);

    virtual std::string makeSound() const = 0;
};

#endif

