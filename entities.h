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
	static enum STATES { EATING, MOVING, REPRODUCING, DYING, ATTACKED, BORN};

	State(STATES number) : state_number(number) { }

	void setState(STATES number) { state_number = number; }
	STATES getState() { return STATES(state_number); }

	bool isEating() 
	{
		return state_number == EATING;
	}

	bool isMoving()
	{
		return state_number == MOVING;
	}

	bool isReproducing() 
	{
		return state_number == REPRODUCING;
	}

	bool isDying() 
	{
		return state_number == DYING;
	}

	bool isAttacked() 
	{
		return state_number == ATTACKED;
	}

	bool isBorn()
	{
		return state_number == BORN;
	}

	void setEating() { state_number = EATING; }
	void setMoving() { state_number = MOVING; }
	void setReproducing() { state_number = REPRODUCING; }
	void setDying() { state_number = DYING; }
	void setAttacked() { state_number = ATTACKED; }
	void setBorn() { state_number = BORN; }
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

	static const int MAX_STARVATION = 0;
	static const int MAX_OLD = 0;
	static const int MAX_HEALTH = 0;

	static const int DANGEROUS_STARVATION = 0;
	static const int SAFE_HEALTH_LEVEL = 0;
	static const int MAX_REPRODUCTION_OLD = 0;
	static const int MIN_REPRODUCTION_OLD = 0;

	static const int PLANTEATING_EATING_HEALTH_ADDITION = 0;
	static const int PLANT_EATING_HEALTH_ADDITION = 0;
	static const int FOOD_EATING_HEALTH_ADDITION = 0;

public:
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
	void addOld(int value) { old = Utility::limit(1, MAX_OLD, old + value); }
	void addStarvation(int value) { old = Utility::limit(1, MAX_STARVATION, starvation + value); }

	void incHealth() { health += 1; }
	void incOld() { old += 1; }
	void incStarvation() { starvation += 1; }

	State& getState() { return state; }
	/*void setState(State state) { state.setState(state.getState()); }
	void setState(State::STATES state) { this->state.setState(state); }*/
	bool isEating() {
		return state.isEating();
	}

	bool isMoving() {
		return state.isMoving();
	}

	bool isReproducing() {
		return state.isReproducing();
	}

	bool isDying() {
		return state.isDying();
	}

	bool isAttacked() {
		return state.isAttacked();
	}

	bool isBorn() { return state.isBorn(); }

	void setEating() { state.setEating(); }
	void setMoving() { state.setMoving(); }
	void setReproducing() { state.setReproducing(); }
	void setDying() { state.setDying(); }
	void setAttacked() { state.setAttacked(); }

	virtual bool isAnimal() { return 0; }
	virtual bool isPlantEating() { return 0; }
	virtual bool isPredator() { return 0; }
	virtual bool isFood() { return 0; }
	virtual bool isPlantEatingFood() { return 0; }
	virtual bool isPredatorFood() { return 0; }
	virtual bool isPlant() { return 0; }

	virtual int getMaxStarvation() { return MAX_STARVATION; }
	virtual int getMaxOld() { return MAX_OLD; }
	virtual int getMaxHealth() { return MAX_HEALTH; }

	virtual int getDangerousStarvation() { return DANGEROUS_STARVATION; }
	virtual int getSafeHealthLevel() { return SAFE_HEALTH_LEVEL; }
	virtual int getMinReproductionOld() { return MIN_REPRODUCTION_OLD; }
	virtual int getMaxReproductionOld() { return MAX_REPRODUCTION_OLD; }

	virtual int getPlantEatingEatingHealthAddition() { return PLANTEATING_EATING_HEALTH_ADDITION; }
	virtual int getPlantEatingHealthAddition() { return PLANT_EATING_HEALTH_ADDITION; }
	virtual int getFoodEatingHealthAddition() { return FOOD_EATING_HEALTH_ADDITION; }

	bool isReproducable() { return getOld() >= getMinReproductionOld() && getOld() <= getMaxReproductionOld(); }
};

int Entity::currentId = 0;

class Animal : public Entity
{
	int attack_force;

public:
	static const int MIN_ATTACK_FORCE = 1;

	Animal(int old_level, int starvation_level, int health_level, int attack, int xPos, int yPos) : 
		Entity(old_level, starvation_level, health_level, xPos, yPos), attack_force(attack) {}
	Animal(int xPos, int yPos, int health) : Entity(1, 1, health, xPos, yPos), attack_force(MIN_ATTACK_FORCE) {}

	virtual bool isAnimal() { return 1; }
};

class PlantEating : public Animal
{
	static const int MAX_HEALTH = 10;
	static const int MAX_OLD = 20;
	static const int MAX_STARVATION = 15;

	static const int DANGEROUS_STARVATION = 5;
	static const int SAFE_HEALTH_LEVEL = 7;

	static const int MIN_REPRODUCTION_OLD = 3;
	static const int MAX_REPRODUCTION_OLD = 9;

	static const int PLANT_EATING_HEALTH_ADDITION = 5;
	static const int FOOD_EATING_HEALTH_ADDITION = 2;

public:
	PlantEating(int old_level, int starvation_level, int attack, int health_level, int xPos, int yPos) :
		Animal(old_level, starvation_level, health_level, attack, xPos, yPos) {}
	PlantEating(int xPos, int yPos) : Animal(xPos, yPos, MAX_HEALTH) {}

	virtual bool isPlantEating() { return true; }

	virtual int getMaxStarvation() { return MAX_STARVATION; }
	virtual int getMaxOld() { return MAX_OLD; }
	virtual int getMaxHealth() { return MAX_HEALTH; }

	virtual int getDangerousStarvation() { return DANGEROUS_STARVATION; }
	virtual int getSafeHealthLevel() { return SAFE_HEALTH_LEVEL; }
	virtual int getMinReproductionOld() { return MIN_REPRODUCTION_OLD; }
	virtual int getMaxReproductionOld() { return MAX_REPRODUCTION_OLD; }

	virtual int getPlantEatingHealthAddition() { return PLANT_EATING_HEALTH_ADDITION; }
	virtual int getFoodEatingHealthAddition() { return FOOD_EATING_HEALTH_ADDITION; }
};

class Predator : public Animal
{
	static const int MAX_HEALTH = 15;
	static const int MAX_OLD = 15;
	static const int MAX_STARVATION = 10;

	static const int DANGEROUS_STARVATION = 4;
	static const int SAFE_HEALTH_LEVEL = 11;

	static const int MIN_REPRODUCTION_OLD = 4;
	static const int MAX_REPRODUCTION_OLD = 8;

	static const int PLANTEATING_EATING_HEALTH_ADDITION = 4;
	static const int FOOD_EATING_HEALTH_ADDITION = 1;

public:
	Predator(int old_level, int starvation_level, int health, int attack, int attack_force_magnitude, int xPos, int yPos) :
		Animal(old_level, starvation_level, health, attack, xPos, yPos)
	{

	}
	Predator(int xPos, int yPos) : Animal(xPos, yPos, MAX_HEALTH) {}

	virtual bool isPredator() { return true; }

	virtual int getMaxStarvation() { return MAX_STARVATION; }
	virtual int getMaxOld() { return MAX_OLD; }
	virtual int getMaxHealth() { return MAX_HEALTH; }

	virtual int getDangerousStarvation() { return DANGEROUS_STARVATION; }
	virtual int getSafeHealthLevel() { return SAFE_HEALTH_LEVEL; }
	virtual int getMinReproductionOld() { return MIN_REPRODUCTION_OLD; }
	virtual int getMaxReproductionOld() { return MAX_REPRODUCTION_OLD; }

	virtual int getPlantEatingEatingHealthAddition() { return PLANTEATING_EATING_HEALTH_ADDITION; }
	virtual int getFoodEatingHealthAddition() { return FOOD_EATING_HEALTH_ADDITION; }
};

class Food : public Entity
{
public:
	Food(int old_level, int starvation_level, int health_level, int xPos, int yPos) : Entity(old_level, starvation_level, health_level, xPos, yPos) {}
	Food(int xPos, int yPos, int health) : Entity(1, 1, health, xPos, yPos) {}

	virtual bool isFood() { return true; }
};

class PlantEatingFood : public Food
{
	static const int MAX_OLD = 9;
	static const int MAX_HEALTH = 6;

public:
	PlantEatingFood(int old_level, int starvation_level, int health_level, int xPos, int yPos) : Food(old_level, starvation_level, health_level, xPos, yPos) {}
	PlantEatingFood(int xPos, int yPos) : Food(xPos, yPos, MAX_HEALTH) {}

	virtual bool isPlantEatingFood() { return true; }

	virtual int getMaxOld() { return MAX_OLD; }
	virtual int getMaxHealth() { return MAX_HEALTH; }
};

class PredatorFood : public Food
{
	static const int MAX_OLD = 6;
	static const int MAX_HEALTH = 6;

public:
	PredatorFood(int old_level, int starvation_level, int health_level, int xPos, int yPos) : Food(old_level, starvation_level, health_level, xPos, yPos) {}
	PredatorFood(int xPos, int yPos) : Food(xPos, yPos, MAX_HEALTH) {}

	virtual bool isPredatorFood() { return true; }

	virtual int getMaxOld() { return MAX_OLD; }
	virtual int getMaxHealth() { return MAX_HEALTH; }
};

class Plant : public Entity
{
	static const int MAX_OLD = 60;
	static const int MAX_HEALTH = 30;

	static const int MAX_REPRODUCTION_OLD = 8;
	static const int MIN_REPRODUCTION_OLD = 4;

public:
	Plant(int old_level, int starvation_level, int health_level, int xPos, int yPos) : Entity(old_level, starvation_level, health_level, xPos, yPos) {}
	Plant(int xPos, int yPos) : Entity(1, 1, MAX_HEALTH, xPos, yPos) {}

	virtual bool isPlant() { return true; }

	virtual int getMaxOld() { return MAX_OLD; }
	virtual int getMaxHealth() { return MAX_HEALTH; }

	virtual int getMinReproductionOld() { return MIN_REPRODUCTION_OLD; }
	virtual int getMaxReproductionOld() { return MAX_REPRODUCTION_OLD; }
};
