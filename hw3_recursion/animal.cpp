/*#include <iostream>
#include <string>
using namespace std;
*/
//Your declarations and implementations would go here

class Animal {
public:
	Animal(string name);
	string name() const;
	virtual string moveAction() const;
	virtual void speak() const = 0;
	virtual ~Animal();

private:
	string m_name;
};

class Cat : public Animal {
public:
	Cat(string name);
	virtual void speak() const;
	virtual ~Cat();
private:

};

class Duck : public Animal {
public:
	Duck(string name);
	virtual string moveAction() const;
	virtual void speak() const;
	virtual ~Duck();
private:

};

class Pig : public Animal {
public:
	Pig(string name, int weight);
	virtual void speak() const;
	virtual ~Pig();
private:
	int m_weight;
};



/*
void animate(const Animal* a)
{
	a->speak();
	cout << "!  My name is " << a->name()
		<< ".  Watch me " << a->moveAction() << "!\n";
}

int main()
{
	Animal* animals[4];
	animals[0] = new Cat("Fluffy");
	// Pigs have a name and a weight in pounds.  Pigs under 170
	// pounds oink; pigs weighing at least 170 pounds grunt.
	animals[1] = new Pig("Napoleon", 190);
	animals[2] = new Pig("Wilbur", 50);
	animals[3] = new Duck("Daffy");

	cout << "Here are the animals." << endl;
	for (int k = 0; k < 4; k++)
		animate(animals[k]);

	// Clean up the animals before exiting
	cout << "Cleaning up." << endl;
	for (int k = 0; k < 4; k++)
		delete animals[k];
}
*/
/*Output produced:

Here are the animals.
Meow!  My name is Fluffy.  Watch me walk!
Grunt!  My name is Napoleon.  Watch me walk!
Oink!  My name is Wilbur.  Watch me walk!
Quack!  My name is Daffy.  Watch me swim!
Cleaning up.
Destroying Fluffy the cat
Destroying Napoleon the pig
Destroying Wilbur the pig
Destroying Daffy the duck*/

Animal::Animal(string name):m_name(name)
{
}

string Animal::name() const
{
	return m_name;
}

string Animal::moveAction() const
{
	return "walk";
}

Animal::~Animal()
{
}

Cat::Cat(string name):Animal(name)
{
}

void Cat::speak() const
{
	cout << "Meow";
}

Cat::~Cat()
{
	cout << "Destroying " << name() << " the cat" << endl;
}


Duck::Duck(string name) : Animal(name)
{
}

string Duck::moveAction() const
{
	return "swim";
}

void Duck::speak() const
{
	cout << "Quack";
}

Duck::~Duck()
{
	cout << "Destroying " << name() << " the duck" << endl;

}

Pig::Pig(string name, int weight) : Animal(name),m_weight(weight)
{
}

void Pig::speak() const
{
	if (m_weight >= 170) {
		cout << "Grunt";
	}
	else {
		cout << "Oink";
	}
}

Pig::~Pig()
{
	cout << "Destroying " << name() << " the pig" << endl;
}
