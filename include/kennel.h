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

    int get_ID() const;
    std::string get_size() const;
    int get_capacity() const;
    std::string get_type() const;
    std::vector<Animal*> get_animals() const;
    bool get_isEmpty() const;

    void set_ID(int id);
    void set_size(const std::string& size);
    void set_capacity(int cap);
    void set_type(const std::string& type);
    void set_animals(const std::vector<Animal*>& animalList);
    void set_isEmpty(bool val);
};

#endif
