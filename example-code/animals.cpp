// Include iostream so we can print stuff into the terminal.
#include <iostream>

//===============================================================================================
// Animal Class
//===============================================================================================
// This is our base class which defines the interface for an Animal.
// We put stuff in here that every Animal can do, in this case eat and be hungry.
class Animal
{
public:
    // This constructor is called whenever an Animal is created.
    Animal()
        : isHungry (true) // Member variables should be initialised in the initialisation list.
    {
        // This will show us when the constructor of Animal gets called.
        std::cout << "Animal's constructor called!\n";
    }

    // This destructor is called whenever an Animal is destroyed.
    // If our class has any virtual functions we also need to make the destructor virtual.
    // Otherwise the destructors of classes which inherit from this class might not get
    // called.
    virtual ~Animal()
    {
        // This will show us when the destructor of Animal gets called.
        std::cout << "Animal's destructor called!\n";
    }

    void eat()
    {
        // Here we call the virtual function enjoyMeal(). 
        // If a class which inherits from Animal overrides this function it's version of
        // the function will get called. This allows different types of Animal to exhibit
        // different behavior when they are fed.
        enjoyMeal();
        
        // Animals will automatically become non-hungry when they eat.
        isHungry = false;
    }

    // Very simple member function which queries the isHungry variable.
    void areYouHungry()
    {
        if (isHungry)
            std::cout << "Yeah, I'm famished dawg!\n";
        else
            std::cout << "Nah Fam!\n";
    }

    // The enjoyMeal() function is marked as virtual.
    // This tells the computer that it needs to see if an inheriting class has overridden
    // the function, and to run the correct implementation if it has.
    virtual void enjoyMeal()
    {
        std::cout << "Yum!\n";
    }

private:
    // isHungry is a private variable because we want to keep it encapsulated.
    // This way nobody else can change it and it will behave as expected, when the Animal
    // eats it will no longer be hungry. If people had access to the isHungry variable
    // they could alter it and overfeed the Animal.
    bool isHungry;
}; // Always remember the semicolon after a class definition.

//===============================================================================================
// Dog Class
//===============================================================================================
// Dog inherits publicly from Animal.
// Inheritance models the `is a' principle. By making Dog inherit Animal we are stating
// that "A Dog `is an' Animal". If we made a class called Toaster we would not have it
// inherit Animal because a Toaster is not an Animal.
class Dog : public Animal
{
public:
    // This constructor is called whenever a Dog is created.
    Dog (float fluffinessInit) // This initial fluffiness is passed as an argument to the constructor.
        : fluffiness (fluffinessInit) // Again, variables should be initialised in the initialisation list.
    {
        // This will show us when the constructor of Dog gets called.
        std::cout << "Dog's constructor called!\n";
    }

    // This destructor is called whenever a Dog is destroyed.
    ~Dog()
    {
        // This will show us when the destructor of Dog gets called.
        std::cout << "Dog's destructor called!\n";
    }

    // Here we override the enjoyMeal() function from the Animal class.
    // Marking the function with the `override' keyword makes the compiler check that the
    // function exists in the base class. If we made a typo here and put 
    // `void enjoyMeat() override' the compiler would tell us that our function is not
    // overriding anything so we have probably made a mistake.
    void enjoyMeal() override
    {
        std::cout << "Woof!\n";
    }

    // A setter for the Dog's fluffiness variable.
    void setFluffiness (float newFluffiness)
    {
        fluffiness = newFluffiness;
    }

    // A getter for the Dog's fluffiness variable.
    float getFluffiness()
    {
        return fluffiness;
    }

private:
    // Dogs have their own properties which not all Animals exhibit. In this case
    // fluffiness.
    float fluffiness;
}; // Always remember the semicolon after a class definition.

//===============================================================================================
// Animal Class
//===============================================================================================
// Cat inherits publicly from Animal. Again, we are stating that a Cat `is an' Animal.
class Cat : public Animal
{
public:
    // This constructor is called whenever a Cat is created.
    Cat (float hairinessInit) // The initial hairiness is passed as an argument to the constructor.
        : hairiness (hairinessInit) // Again, variables should be initialised in the initialisation list.
    {
        // This will show us when the constructor of Cat gets called.
        std::cout << "Cat's constructor called!\n";
    }

    // This destructor is called whenever a Cat is destroyed.
    ~Cat()
    {
        // This will show us when the destructor of Cat gets called.
        std::cout << "Cat's destructor called!\n";
    }

    // Here we override the enjoyMeal() function from the Animal class.
    void enjoyMeal() override
    {
        std::cout << "Meow!\n";
    }

    // A setter for the Cat's hairiness variable.
    void setHairiness (float newHairiness)
    {
        hairiness = newHairiness;
    }

    // A getter for the Cat's hairiness variable.
    float getHairiness()
    {
        return hairiness;
    }

private:
    // Cats have their own properties which not all Animals exhibit. In this case
    // hairiness.
    float hairiness;
}; // Always remember the semicolon after a class definition.

//===============================================================================================
// Our Program
//===============================================================================================
// Anything we use in the program needs to be defined beforehand. Hence the class
// definitions being above the main() function.
int main()
{
    // Some output formatting.
    std::cout << "======================================================\n"
              << " Creating Variables\n"
              << "======================================================\n";

    std::cout << "Creating a Dog:\n";
    // First we create a Dog object with 14% fluffiness.
    // When we create an object of a derived type (one which inherits another class) the
    // bass class' constructor is called first followed by the derived class' constructor.
    // So the line `Dog dog (14.0f);' should call Animal's constructor followed by Dog's
    // constructor. You should be able to see this in the programs output because we print things in
    // the constructors.
    Dog dog (14.0f); // Dog's constructor takes an argument to set the initial fluffiness.
                     // This is passed in some round brackets.

    std::cout << "\nCreating a Cat:\n";
    // Now we create a Cat object with 99.9% hairiness. 
    // This should call Animal's constructor followed by Cat's constructor.
    Cat cat (99.9f); // Cat's constructor takes an argument to set the initial hairiness.

    // Some output formatting.
    std::cout << "\n======================================================\n"
              << " Setting and Getting\n"
              << "======================================================\n";

    // We can query the respective fluffiness and hairiness of our Dog and Cat using the
    // getters we defined.
    std::cout << "The dog is " << dog.getFluffiness() << "% fluffy.\n";
    std::cout << "The cat is " << cat.getHairiness() << "% hairy.\n";

    // We can also alter them using the setters we defined.
    // Let's give the dog a blow dry.
    std::cout << "\nBlow drying the dog's hair!\n";
    dog.setFluffiness (88.4f);
    std::cout << "The dog is now " << dog.getFluffiness() << "% fluffy.\n";

    // And give the cat a hair cut.
    std::cout << "\nCutting the cat's hair!\n";
    cat.setHairiness (0.0f);
    std::cout << "The cat is now " << cat.getHairiness() << "% hairy.\n";

    // Some output formatting.
    std::cout << "\n======================================================\n"
              << " Polymorphism\n"
              << "======================================================\n";

    // Because both Dog and Cat are Animals, we can treat them as such and interact with
    // them through the Animal interface. This allows us to have variables which appear to
    // be the same type (Animal) but behave differently. This is known as polymorphism.
    // One useful use for this is to have arrays in which each element behaves
    // differently. In C++ arrays can only contain one type of variable, but because both
    // Dog and Cat can be treated as Animals we can put them in an Array of Animals.
    // To do this we need to make an array of pointers to Animals and make the elements
    // point at a our Dog and Cat.
    int nAnimals = 2;
    Animal *animals[] = {&dog, &cat};

    // Now we can loop through our array of Animals and ask if they are hungry and feed
    // them. Each Animal should enjoy its meal in a different way like we defined in the
    // classes. If the enjoyMeal() function in Animal was not virtual, Animal's
    // implementation of enjoyMeal() would be called here and both the Dog and Cat would
    // simply say `Yum!'.
    for (int i = 0; i < nAnimals; ++i)
    {
        Animal *currentAnimal = animals [i]; // Get the current animal from the array.

        std::cout << "   Owner: Animal number " << i << ", are you hungry?\n"
                  << "Animal " << i << ": ";

        // When calling a member function on a pointer to an object the . becomes  an ->
        // `pointerToObject->function();' instead of `object.function();'.
        currentAnimal->areYouHungry();

        std::cout << "   Owner: Oh my, I'll have to feed you then.\n"
                  << "Animal " << i << ": ";
        currentAnimal->eat();
        std::cout << "   Owner: Animal number " << i << ", are you still hungry?\n"
                  << "Animal " << i << ": ";
        currentAnimal->areYouHungry();
        std::cout << "\n";
    } 

    // When we treat derived classes (Dog and Cat) as though they were part of the base
    // class (Animal) we cannot use the functions and variables unique to the derived
    // classes. The next line of code will produce an error if you uncomment it.

    // animals [0]->setFluffiness (12.3f); // Even though we know that the 0th element of 
                                           // animals is a Dog we cannot call its
                                           // setFluffiness() function. This is because we are
                                           // treating it as an Animal and Animal has no
                                           // setFluffiness() function.


    // Some output formatting.
    std::cout << "======================================================\n"
              << " Destroying Variables\n"
              << "======================================================\n";

    // When our dog and cat variable go out of scope (we exit the function in which they
    // were created) they are automatically destroyed in the reverse order of which they
    // were created.
    // When an object of a derived type is destroyed, its destructor is called followed by
    // its base class' destructor. 
    // We created a Dog first and then a Cat. When running the program we should see Cat's
    // destructor get called followed by the Animal destructor, then the Dog's destructor
    // followed again by the Animal destructor.
    std::cout << "Destroying a Cat then a Dog:\n";

    return 0;
}
