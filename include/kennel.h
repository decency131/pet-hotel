#ifndef KENNEL_H
#define KENNEL_H

#include <string>
#include <vector>
#include "animal.h"

class Kennel {
private:
    int ID;
    std::string size;
    int capacity;
    std::string type;
    std::vector<Animal*> animals;
    bool isEmpty;

public:
    Kennel();
    Kennel(int ID, const std::string& size, int capacity, const std::string& type, const std::vector<Animal*>& animals, bool isEmpty);
    ~Kennel();

    int getID() const;
    std::string getSize() const;
    int getCapacity() const;
    std::string getType() const;
    std::vector<Animal*> getAnimals() const;
    bool getIsEmpty() const;

    void setID(int id);
    void setSize(const std::string& size);
    void setCapacity(int cap);
    void setType(const std::string& type);
    void setAnimals(const std::vector<Animal*>& animalList);
    void setIsEmpty(bool val);
};

#endif

