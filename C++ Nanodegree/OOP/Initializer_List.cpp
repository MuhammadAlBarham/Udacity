#include <assert.h>
#include <string>

// TODO: Define class Person

class Person{
    
    public:
    std::string name ; 
    Person(std::string nameIn); 
}; 

Person::Person(std::string nameIn): name(nameIn) { }
  // TODO: Define a public constructor with an initialization list
  // TODO: Define a public member variable: name

// Test
int main() {
  Person alice("Alice");
  Person bob("Bob");
  assert(alice.name != bob.name);
}