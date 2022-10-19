#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <random>
#include <cassert>
#include <chrono>
#include <conio.h>
#include "entities.h"
#include "map.h"
#include "utilities.h"

class Game
{
	class Model
	{
		Map map;
		std::vector<Entity*> entities;

	public:
		Model(int mapWidth, int mapHeight) : map(Map(mapWidth, mapHeight)) {}
		std::vector<Entity*>& getEntities() { return entities; }

		void addPlantEating(int xPos, int yPos)
		{
			Entity* ent = new PlantEating(xPos, yPos);

			entities.push_back(ent);
		}

		void addPlantEating(Position pos) { addPlantEating(pos.getX(), pos.getY()); }

		void addPredator(int xPos, int yPos)
		{
			Entity* ent = new Predator(xPos, yPos);

			entities.push_back(ent);
		}

		void addPredator(Position pos) { addPredator(pos.getX(), pos.getY()); }

		void addPlantEatingFood(int xPos, int yPos)
		{
			Entity* ent = new PlantEatingFood(xPos, yPos);

			entities.push_back(ent);
		}

		void addPlantEatingFood(Position pos) { addPlantEatingFood(pos.getX(), pos.getY()); }

		void addPredatorFood(int xPos, int yPos)
		{
			Entity* ent = new PredatorFood(xPos, yPos);

			entities.push_back(ent);
		}

		void addPredatorFood(Position pos) { addPredatorFood(pos.getX(), pos.getY()); }

		void addPlant(int xPos, int yPos)
		{
			Entity* ent = new Plant(xPos, yPos);

			entities.push_back(ent);
		}

		void addPlant(Position pos) { addPlant(pos.getX(), pos.getY()); }

		Map& getMap() { return map; }

		bool isPositionFree(int xPos, int yPos)
		{
			bool free = true;

			for (auto item : entities)
			{
				if (item->getX() == xPos && item->getY() == yPos)
				{
					free = false;
					break;
				}
			}

			return free;
		}
		bool isPositionFree(Position pos) { return isPositionFree(pos.getX(), pos.getY()); }

		bool getFreePositionAround(int xPos, int yPos, int& freeX, int& freeY)
		{
			for (int i = 0; i < 2; i++)
			{
				freeX = xPos + std::pow(-1, i);
				freeY = yPos;

				if (isPositionFree(freeX, freeY))
					return true;
			}

			for (int i = 0; i < 2; i++)
			{
				freeX = xPos;
				freeY = yPos + std::pow(-1, i);

				if (isPositionFree(freeX, freeY))
					return true;
			}

			return false;
		}
		bool getFreePositionAround(int xPos, int yPos, Position& pos)
		{
			int tmpX;
			int tmpY;

			bool result = getFreePositionAround(xPos, yPos, tmpX, tmpY);

			pos.setX(tmpX);
			pos.setY(tmpY);

			return result;
		}

		Entity* getClosestPlant(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPlant())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestPlantEatingFood(Entity& ent) 
		{ 
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPlantEatingFood())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestPredatorFood(Entity& ent)
		{ 
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPredatorFood())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestPlantEating(Entity& ent)
		{ 
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPlantEating())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestPredator(Entity& ent) 
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPredator())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}

		Entity* getClosestLivingPlant(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPlant() || other->isDying())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestLivingPlantEatingFood(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPlantEatingFood() || other->isDying())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestLivingPredatorFood(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPredatorFood() || other->isDying())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestLivingPlantEating(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPlantEating() || other->isDying())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestLivingPredator(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPredator() || other->isDying())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}

		Entity* getClosestLivingReproducablePlantEating(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPlantEating() || other->isDying() || !other->isReproducable())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
		Entity* getClosestLivingReproducablePredator(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!other->isPredator() || other->isDying() || !other->isReproducable())
					continue;

				if (!closest)
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
					continue;
				}

				int distance = map.getDistance(*other, ent);

				if (distance < closestDistance)
				{
					closest = other;
					closestDistance = distance;
				}
			}

			if (closest == &ent)
				return nullptr;

			return closest;
		}
	};

	class View
	{
		Model& model;

	public:
		View(Model& model) : model(model) {}

		Model& getModel() { return model; }

		virtual void render() {}
	};

	class ConsoleView : public View
	{
	public:
		static const char PLANT_EATING_SYM = '@';
		static const char PREDATOR_SYM = '&';
		static const char PLANT_EATING_FOOD_SYM = '*';
		static const char PREDATOR_FOOD_SYM = '%';
		static const char PLANT_SYM = 'X';
		static const char EMPTY_FIELD_SYM = '-';

		ConsoleView(Model& model) : View(model) {}
		
		virtual void render()
		{
			system("cls");

			std::vector<std::vector<char>> representation(getModel().getMap().getHeight());
			for (int i = 0; i < representation.size(); i++)
			{
				representation[i] = std::vector<char>(getModel().getMap().getWidth());

				for (int j = 0; j < representation[i].size(); j++)
					representation[i][j] = EMPTY_FIELD_SYM;
			}

			for (int i = 0; i < getModel().getEntities().size(); i++)
			{
				Entity* ent = getModel().getEntities()[i];

				int yPos = ent->getPos().getY() - 1;
				int xPos = ent->getPos().getX() - 1;

				if (ent->isPlantEating())
					representation[yPos][xPos] = PLANT_EATING_SYM;

				else if (ent->isPredator())
					representation[yPos][xPos] = PREDATOR_SYM;

				else if (ent->isPlantEatingFood())
					representation[yPos][xPos] = PLANT_EATING_FOOD_SYM;

				else if (ent->isPredatorFood())
					representation[yPos][xPos] = PREDATOR_FOOD_SYM;

				else if (ent->isPlant())
					representation[yPos][xPos] = PLANT_SYM;

				//std::cout << ent->isPlantEating() << " " << ent->isPredator() << " " << ent->isPlantEatingFood() << " " << ent->isPredatorFood() << " " << ent->isPlant() << "\n";

				//std::cout << representation[ent->getPos().getY() - 1][ent->getPos().getX() - 1] << "\n";
			}

			for (int i = 0; i < representation.size(); i++)
			{
				for (int j = 0; j < representation[0].size(); j++)
					std::cout << representation[i][j] << " ";
				
				std::cout << "\n";
			}
		}
	};

	class Controller
	{
		Model& model;
		Entity* self;
	public:
		Controller(Model& model, Entity* self) : model(model), self(self)
		{

		}

		Entity* getSelf() { return self; }
		Model& getModel() { return model; }

		virtual void move(int xPos, int yPos) { ; }
		virtual void eat(Entity* other) { ; }
		virtual void reproduce(Entity* other) { ; }
		virtual void die() 
		{ 
			std::vector<Entity*>& entities = getModel().getEntities();

			int pos;
			for (pos = 0; pos < entities.size(); pos++)
				if (entities[pos]->getId() == getSelf()->getId())
					break;

			entities.erase(entities.begin() + pos);
		}
		virtual void actReflexively()
		{ ; }
	};

	class AnimalController : public Controller
	{
	public:
		AnimalController(Model& model, Entity* animal) : Controller(model, animal)
		{
		}

		virtual void move(int xPos, int yPos)
		{ 
			getSelf()->setX(xPos);
			getSelf()->setY(yPos);
		}
	};

	class PlantEatingController : public AnimalController
	{
	public:
		PlantEatingController(Model& model, Entity* plantEatingAnimal) : AnimalController(model, plantEatingAnimal) {}

		virtual void eat(Entity* other)
		{ 
			if (other->isPlant())
			{ 
				getSelf()->addHealth(Utility::limit(0, other->getHealth(), getSelf()->getPlantEatingHealthAddition()));
				getSelf()->addStarvation(Utility::limit(0, other->getHealth(), -getSelf()->getPlantEatingHealthAddition()));

				other->addHealth(-getSelf()->getPlantEatingHealthAddition());
				other->setAttacked();

				if (other->getHealth() == 0)
					other->setDying();
			}

			else if (other->isPlantEatingFood())
			{
				getSelf()->addHealth(Utility::limit(0, other->getHealth(), getSelf()->getFoodEatingHealthAddition()));
				getSelf()->addStarvation(Utility::limit(0, other->getHealth(), -getSelf()->getFoodEatingHealthAddition()));
				other->addHealth(-getSelf()->getFoodEatingHealthAddition());
				other->setAttacked();

				if (other->getHealth() == 0)
					other->setDying();
			}
		}
		virtual void reproduce(Entity* other)
		{ 
			Entity* self = getSelf();

			Position freePos;

			if (getModel().getFreePositionAround(self->getX(), self->getY(), freePos))
			{
				if (getModel().getMap().isValidPosition(freePos))
					getModel().addPlantEating(freePos);
			}
		}

		virtual void actReflexively()
		{
			Entity* animal = getSelf();

			int health = animal->getHealth();
			int starvation_level = animal->getStarvation();
			int old_level = animal->getOld();

			if (old_level == animal->getMaxOld())
				animal->setDying();

			else if (animal->isAttacked())
			{
				Entity* predator = getModel().getClosestLivingPredator(*getSelf());

				int predatorDistance = -1;

				Entity* target = nullptr;

				if (predator)
				{
					predatorDistance = getModel().getMap().getDistance(predator, getSelf());
					target = predator;
				}

				if (target)
				{
					int xDistance = Utility::limit(-1, 1, -(target->getX() - getSelf()->getX()));
					int yDistance = Utility::limit(-1, 1, -(target->getY() - getSelf()->getY()));

					if (Utility::abs(xDistance) > Utility::abs(yDistance))
						getSelf()->setX(getSelf()->getX() + Utility::signum(xDistance));

					else
						getSelf()->setY(getSelf()->getY() + Utility::signum(yDistance));
				}
			}

			else if (starvation_level > animal->getDangerousStarvation() || health < animal->getSafeHealthLevel()) // If animal is starving then find food
			{
				Entity* food = getModel().getClosestLivingPlantEatingFood(*getSelf());
				Entity* plant = getModel().getClosestLivingPlant(*getSelf());

				int foodDistance = -1;

				if (food)
					foodDistance = getModel().getMap().getDistance(food, getSelf());

				int plantDistance = -1;

				if (plant)
					plantDistance = getModel().getMap().getDistance(plant, getSelf());

				Entity* target = nullptr;

				if (!food)
					target = plant;

				else if (!plant)
					target = food;

				else if (foodDistance < plantDistance)
					target = food;

				else
					target = plant;

				if (target)
				{
					int xDistance = target->getX() - getSelf()->getX();
					int yDistance = target->getY() - getSelf()->getY();

					if (xDistance == 0 && Utility::abs(yDistance) == 1 || Utility::abs(xDistance) == 1 && yDistance == 0) // If animal adjacent to target then eat it
						eat(target);

					else if (Utility::abs(xDistance) > Utility::abs(yDistance))
						getSelf()->setX(getSelf()->getX() + Utility::signum(xDistance));

					else
						getSelf()->setY(getSelf()->getY() + Utility::signum(yDistance));
				}
			} 

			else if (old_level >= animal->getMinReproductionOld() && old_level <= animal->getMaxReproductionOld()) // If all is ok then reproduce
			{
				Entity* pair = getModel().getClosestLivingReproducablePlantEating(*getSelf());

				int pairDistance = -1;

				Entity* target = nullptr;

				if (pair)
				{
					pairDistance = getModel().getMap().getDistance(pair, getSelf());
					target = pair;
				}

				if (target)
				{
					int xDistance = target->getX() - getSelf()->getX();
					int yDistance = target->getY() - getSelf()->getY();

					if (xDistance == 0 && Utility::abs(yDistance) == 1 || Utility::abs(xDistance) == 1 && yDistance == 0) // If animal adjacent to target then eat it
						reproduce(target);

					else if (Utility::abs(xDistance) > Utility::abs(yDistance))
						getSelf()->setX(getSelf()->getX() + Utility::signum(xDistance));

					else
						getSelf()->setY(getSelf()->getY() + Utility::signum(yDistance));
				}
			}

			animal->incOld();
			animal->incStarvation();
		}
	};

	class PredatorController : public AnimalController
	{
	public:
		PredatorController(Model& model, Entity* predatorAnimal) : AnimalController(model, predatorAnimal) {}

		virtual void eat(Entity* other)
		{
			if (other->isPlantEating())
			{
				getSelf()->addHealth(Utility::limit(0, other->getHealth(), getSelf()->getPlantEatingEatingHealthAddition()));
				getSelf()->addStarvation(Utility::limit(0, other->getHealth(), -getSelf()->getPlantEatingEatingHealthAddition()));

				other->addHealth(-getSelf()->getPlantEatingEatingHealthAddition());
				other->setAttacked();

				if (other->getHealth() == 0)
					other->setDying();
			}

			else if (other->isPredatorFood())
			{
				getSelf()->addHealth(Utility::limit(0, other->getHealth(), getSelf()->getFoodEatingHealthAddition()));
				getSelf()->addStarvation(Utility::limit(0, other->getHealth(), -getSelf()->getFoodEatingHealthAddition()));
				getSelf()->addStarvation(-getSelf()->getFoodEatingHealthAddition());

				other->addHealth(-getSelf()->getFoodEatingHealthAddition());
				other->setAttacked();

				if (other->getHealth() == 0)
					other->setDying();
			}
		}

		virtual void reproduce(Entity* right)
		{
			Entity* self = getSelf();

			Position freePos;

			if (getModel().getFreePositionAround(self->getX(), self->getY(), freePos))
			{
				if (getModel().getMap().isValidPosition(freePos))
					getModel().addPredator(freePos);
			}
		}
		//virtual void die() { ; }

		virtual void actReflexively()
		{ 
			Predator* animal = static_cast<Predator*>(getSelf());

			int health = animal->getHealth();
			int starvation_level = animal->getStarvation();
			int old_level = animal->getOld();

			if (old_level == animal->getMaxOld())
				animal->setDying();
			
			// if (state is attacked) then run away

			else if (starvation_level > animal->getDangerousStarvation() || health < animal->getSafeHealthLevel()) // If animal is starving then find food
			{
				Entity* food = getModel().getClosestLivingPlantEatingFood(*getSelf());
				Entity* plantEating = getModel().getClosestLivingPlantEating(*getSelf());

				int foodDistance = -1;

				if (food)
					foodDistance = getModel().getMap().getDistance(food, getSelf());

				int plantEatingDistance = -1;

				if (plantEating)
					plantEatingDistance = getModel().getMap().getDistance(plantEating, getSelf());

				Entity* target = nullptr;

				if (!food)
					target = plantEating;

				else if (!plantEating)
					target = food;

				else if (foodDistance < plantEatingDistance)
					target = food;

				else
					target = plantEating;

				if (target)
				{
					int xDistance = target->getX() - getSelf()->getX();
					int yDistance = target->getY() - getSelf()->getY();

					if (xDistance == 0 && Utility::abs(yDistance) == 1 || Utility::abs(xDistance) == 1 && yDistance == 0) // If animal adjacent to target then eat it
						eat(target);

					else if (Utility::abs(xDistance) > Utility::abs(yDistance))
					{
						getSelf()->setX(getSelf()->getX() + Utility::signum(xDistance));

						//std::cout << "x offset\n";
					}

					else
					{
						getSelf()->setY(getSelf()->getY() + Utility::signum(yDistance));

						//std::cout << "y offset\n";
					}

					/*std::cout << "id: " << getSelf()->getId() << " | Diff: " << xDistance << " " << yDistance << "\n";

					std::cout << "target id: " << target->getId() << "\n";*/
				}
			}

			else if (old_level >= animal->getMinReproductionOld() && old_level <= animal->getMaxReproductionOld()) // If all is ok then reproduce
			{
				Entity* pair = getModel().getClosestLivingReproducablePredator(*getSelf());

				int pairDistance = -1;

				Entity* target = nullptr;

				if (pair)
				{
					pairDistance = getModel().getMap().getDistance(pair, getSelf());
					target = pair;
				}

				if (target)
				{
					int xDistance = target->getX() - getSelf()->getX();
					int yDistance = target->getY() - getSelf()->getY();

					if (xDistance == 0 && Utility::abs(yDistance) == 1 || Utility::abs(xDistance) == 1 && yDistance == 0) // If animal adjacent to target then eat it
						reproduce(target);

					else if (Utility::abs(xDistance) > Utility::abs(yDistance))
						getSelf()->setX(getSelf()->getX() + Utility::signum(xDistance));

					else
						getSelf()->setY(getSelf()->getY() + Utility::signum(yDistance));
				}
			}

			animal->incOld();
			animal->incStarvation();
		}
	};

	class FoodController : public Controller
	{
	public:
		FoodController(Model& model, Entity* food) : Controller(model, food) {}

		virtual void actReflexively() 
		{
			Entity* ent = getSelf();

			if (ent->getOld() == ent->getMaxOld())
				ent->setDying();

			ent->incOld();
		}
	};

	class PlantController : public Controller
	{
	public:
		PlantController(Model& model, Entity* plant) : Controller(model, plant) {}

		virtual void reproduce(Entity* right)
		{
			Entity* self = getSelf();

			Position freePos;

			if (getModel().getFreePositionAround(self->getX(), self->getY(), freePos))
			{
				if (getModel().getMap().isValidPosition(freePos))
					getModel().addPlant(freePos);
			}
		}
		//virtual void die() { ; }

		virtual void actReflexively()
		{ 
			Entity* ent = getSelf();

			int oldLevel = ent->getOld();

			if (ent->isBorn())
			{
				ent->setMoving();
				return;
			}

			if (oldLevel == ent->getMaxOld())
				ent->setDying();

			else if (oldLevel >= ent->getMinReproductionOld() && oldLevel <= ent->getMaxReproductionOld())
			{
				if (Utility::getRandomInt(0, 3) != 3)
					reproduce(getSelf());
			}

			ent->incOld();
		}
	};

private:
	Model model;
	ConsoleView view;

public:
	static const int INITIAL_PREDATOR_COUNT = 4;
	static const int INITIAL_PLANTEATING_COUNT = 7;
	static const int INITIAL_PLANT = 0;
	static const int INITIAL_PLANT_FOOD_COUNT = 3;
	static const int INITIAL_PREDATOR_FOOD_COUNT = 2;

	/*static const int INITIAL_PREDATOR_COUNT = 1;
	static const int INITIAL_PLANTEATING_COUNT = 1;
	static const int INITIAL_PLANT = 1;
	static const int INITIAL_PLANT_FOOD_COUNT = 0;
	static const int INITIAL_PREDATOR_FOOD_COUNT = 0;*/

	Game(int mapWidth, int mapHeight) : model(Model(mapWidth, mapHeight)), view(model)
	{
		assert(mapWidth * mapHeight >= INITIAL_PREDATOR_COUNT + INITIAL_PLANTEATING_COUNT +
			INITIAL_PLANT + INITIAL_PLANT_FOOD_COUNT + INITIAL_PREDATOR_FOOD_COUNT);

		std::vector<Position> freePositions(mapWidth * mapHeight);

		for (int i = 0; i < mapHeight * mapWidth; i++)
				freePositions[i] = Position(i % mapWidth + 1, i / mapWidth + 1);

		/*std::random_device rd;
		std::mt19937 gen(rd());*/

		for (int i = 0; i < INITIAL_PREDATOR_COUNT; i++)
		{
			//std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int randomIndex = Utility::getRandomInt(0, freePositions.size() - 1);

			Position pos = freePositions[randomIndex];

			model.addPredator(pos);

			freePositions.erase(freePositions.begin() + randomIndex);
		}

		for (int i = 0; i < INITIAL_PLANTEATING_COUNT; i++)
		{
			/*std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);*/
			int randomIndex = Utility::getRandomInt(0, freePositions.size() - 1);

			Position pos = freePositions[randomIndex];

			model.addPlantEating(pos);

			freePositions.erase(freePositions.begin() + randomIndex);
		}

		for (int i = 0; i < INITIAL_PLANT; i++)
		{
			/*std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);*/
			int randomIndex = Utility::getRandomInt(0, freePositions.size() - 1);

			Position pos = freePositions[randomIndex];

			model.addPlant(pos);

			freePositions.erase(freePositions.begin() + randomIndex);
		}

		for (int i = 0; i < INITIAL_PLANT_FOOD_COUNT; i++)
		{
			/*std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);*/

			int randomIndex = Utility::getRandomInt(0, freePositions.size() - 1);

			Position pos = freePositions[randomIndex];

			model.addPlantEatingFood(pos);

			freePositions.erase(freePositions.begin() + randomIndex);
		}

		for (int i = 0; i < INITIAL_PREDATOR_FOOD_COUNT; i++)
		{
		/*	std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);*/

			int randomIndex = Utility::getRandomInt(0, freePositions.size() - 1);


			Position pos = freePositions[randomIndex];

			model.addPredatorFood(pos);

			freePositions.erase(freePositions.begin() + randomIndex);
		}
	}

	void run()
	{
		auto last = std::chrono::system_clock::now();

		while (true) // Game loop
		{
			auto now = std::chrono::system_clock::now();

			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() < 400)
			{
				while (_kbhit())
					_getch();

				continue;
			}

			last = now;

			view.render(); // rendering map to console
			
			for (int i = 0; i < model.getEntities().size(); i++)
			{
				Controller* controller = nullptr;
				Entity* ent = model.getEntities()[i];

				if (ent->isPlantEating())
					controller = new PlantEatingController(model, ent);

				else if (ent->isPredator())
					controller = new PredatorController(model, ent);
				
				else if (ent->isPlantEatingFood() || ent->isPredatorFood())
					controller = new FoodController(model, ent);
				
				else if (ent->isPlant())
					controller = new PlantController(model, ent);

				controller->actReflexively();
			}

			auto it = model.getEntities().begin();
			while (it != model.getEntities().end())
			{
				Entity* ent = *it;

				if (ent->isDying())
					it = model.getEntities().erase(it);

				if (it != model.getEntities().end())
					it++;
			}

			if (Keyboard::handle_keyboard() == Keyboard::ESC_KEY)
				exit(0);
		}
	}
};

int main()
{
	Game game(30, 30);

	game.run();

	return 0;
}