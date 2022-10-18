#pragma once
#include "utilities.h"

class Position
{
	int x;
	int y;

public:
	Position() : x(0), y(0) {}

	Position(int x, int y) : x(x), y(y) {}

	int getX() { return x; }
	int getY() { return y; }

	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
};

class State
{
	int state_number;

public:
	static enum STATES { EATING, MOVING, REPRODUCING, DYING, ATTACKED};

	State(STATES number) : state_number(number) { }

	void setState(STATES number) { state_number = number; }
	STATES getState() { return STATES(state_number); }

	bool isEating() {
		return state_number == EATING;
	}

	bool isMoving() {
		return state_number == MOVING;
	}

	bool isReproducing() {
		return state_number == REPRODUCING;
	}

	bool isDying() {
		return state_number == DYING;
	}

	bool isAttacked() {
		return state_number == ATTACKED;
	}
};

class Entity
{
	static int currentId;
	int id;
	int old;
	int starvation;
	int health;

	Position pos;
	State state;

public:
	static const int MAX_STARVATION = 10;
	static const int MAX_OLD = 10;
	static const int MAX_HEALTH = 10;

	static const int DANGEROUS_STARVATION = 7;
	static const int SAFE_HEALTH_LEVEL = 8;
	static const int MAX_REPRODUCTION_OLD = 6;
	static const int MIN_REPRODUCTION_OLD = 3;

	Entity(int old_level, int starvation_level, int health_level, int xPos, int yPos) : id(currentId++), old(old_level), starvation(starvation_level),
		health(health_level), pos(Position(xPos, yPos)), state(State(State::MOVING)) {}

	int getId() { return id; }
	int getOld() { return old; }
	int getStarvation() { return starvation; }

	Position getPos() { return pos; }
	int getX() { return pos.getX(); }
	int getY() { return pos.getY(); }

	void setX(int x) { pos.setX(x); }
	void setY(int y) { pos.setY(y); }

	int getHealth() { return health; }
	void setHealth(int health_level) { health = health_level; }

	void addHealth(int value) { health = Utility::limit(0, MAX_HEALTH, health + value); }
	void addOld(int value) { old = Utility::limit(0, MAX_OLD, old + value); std::cout << "id: " << id << " old: " << old << "\n"; }
	void addStarvation(int value) { old = Utility::limit(0, MAX_STARVATION, starvation + value); }

	State getState() { return state; }
	void setState(State state) { this->state = state; }
	void setState(State::STATES state) { this->state = State(state); }

	virtual bool isAnimal() { return 0; }
	virtual bool isPlantEating() { return 0; }
	virtual bool isPredator() { return 0; }
	virtual bool isFood() { return 0; }
	virtual bool isPlantEatingFood() { return 0; }
	virtual bool isPredatorFood() { return 0; }
	virtual bool isPlant() { return 0; }
};

int Entity::currentId = 0;

class Animal : public Entity
{
	int attack_force;

public:
	static const int MIN_ATTACK_FORCE = 1;

	Animal(int old_level, int starvation_level, int health_level, int attack, int xPos, int yPos) : 
		Entity(old_level, starvation_level, health_level, xPos, yPos), attack_force(attack) {}
	Animal(int xPos, int yPos) : Entity(1, 1, MAX_HEALTH, xPos, yPos), attack_force(MIN_ATTACK_FORCE) {}

	virtual bool isAnimal() { return 1; }
};

class PlantEating : public Animal
{
public:
	static const int PLANT_EATING_HEALTH_ADDITION = 3;
	static const int FOOD_EATING_HEALTH_ADDITION = 2;

	PlantEating(int old_level, int starvation_level, int attack, int health_level, int xPos, int yPos) :
		Animal(old_level, starvation_level, health_level, attack, xPos, yPos) {}
	PlantEating(int xPos, int yPos) : Animal(xPos, yPos) {}

	virtual bool isPlantEating() { return true; }
};

class Predator : public Animal
{
public:
	static const int MIN_ATTACK_FORCE = 1;

	Predator(int old_level, int starvation_level, int health, int attack, int attack_force_magnitude, int xPos, int yPos) :
		Animal(old_level, starvation_level, health, attack, xPos, yPos)
	{

	}
	Predator(int xPos, int yPos) : Animal(xPos, yPos) {}

	virtual bool isPredator() { return true; }
};

class Food : public Entity
{
public:
	Food(int old_level, int starvation_level, int health_level, int xPos, int yPos) : Entity(old_level, starvation_level, health_level, xPos, yPos) {}
	Food(int xPos, int yPos) : Entity(1, 1, MAX_HEALTH, xPos, yPos) {}

	virtual bool isFood() { return true; }
};

class PlantEatingFood : public Food
{
public:
	PlantEatingFood(int old_level, int starvation_level, int health_level, int xPos, int yPos) : Food(old_level, starvation_level, health_level, xPos, yPos) {}
	PlantEatingFood(int xPos, int yPos) : Food(xPos, yPos) {}

	virtual bool isPlantEatingFood() { return true; }
};

class PredatorFood : public Food
{
public:
	PredatorFood(int old_level, int starvation_level, int health_level, int xPos, int yPos) : Food(old_level, starvation_level, health_level, xPos, yPos) {}
	PredatorFood(int xPos, int yPos) : Food(xPos, yPos) {}

	virtual bool isPredatorFood() { return true; }
};

class Plant : public Entity
{
public:
	Plant(int old_level, int starvation_level, int health_level, int xPos, int yPos) : Entity(old_level, starvation_level, health_level, xPos, yPos) {}
	Plant(int xPos, int yPos) : Entity(1, 1, MAX_HEALTH, xPos, yPos) {}

	virtual bool isPlant() { return true; }
};
