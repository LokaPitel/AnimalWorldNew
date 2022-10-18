#include <iostream>
#include <vector>
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

		Entity* getClosestPlant(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = entities[i];

				if (!closest && other->isPlant())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
				}

				if (!closest)
					continue;

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

				if (!closest && other->isPlantEatingFood())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
				}

				if (!closest)
					continue;

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

				if (!closest && other->isPredatorFood())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
				}

				if (!closest)
					continue;

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

				if (!closest && other->isPlantEating())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
				}

				if (!closest)
					continue;

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

				if (!closest && other->isPredator())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
				}

				if (!closest)
					continue;

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
			//system("cls");

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
		virtual void die() { ; }
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
				getSelf()->addHealth(PlantEating::PLANT_EATING_HEALTH_ADDITION);
				other->addHealth(-PlantEating::PLANT_EATING_HEALTH_ADDITION);

				other->setState(State::STATES::ATTACKED);

				if (other->getHealth() < 0)
					other->setState(State::STATES::DYING);
			}

			else if (other->isPlantEatingFood())
			{
				getSelf()->addHealth(PlantEating::FOOD_EATING_HEALTH_ADDITION);
				other->addHealth(-PlantEating::FOOD_EATING_HEALTH_ADDITION);

				other->setState(State::STATES::ATTACKED);

				if (other->getHealth() < 0)
					other->setState(State::STATES::DYING);
			}
		}

		virtual void reproduce(Entity* other)
		{ 
			;
		}
		virtual void die() { ; }

		virtual void actReflexively()
		{
			PlantEating* animal = static_cast<PlantEating*>(getSelf());

			int health = animal->getHealth();
			int starvation_level = animal->getStarvation();
			int old_level = animal->getOld();

			// if (state is attacked) then run away

			if (starvation_level > Entity::DANGEROUS_STARVATION || health < Entity::SAFE_HEALTH_LEVEL) // If animal is starving then find food
			{
				Entity* food = getModel().getClosestPlantEatingFood(*getSelf());
				Entity* plant = getModel().getClosestPlant(*getSelf());

				int foodDistance = getModel().getMap().getDistance(food, getSelf());
				int plantDistance = getModel().getMap().getDistance(plant, getSelf());

				Entity* target = nullptr;

				if (foodDistance < plantDistance)
					target = food;

				else
					target = plant;

				int xDistance = target->getX() - getSelf()->getX();
				int yDistance = target->getY() - getSelf()->getY();

				if (xDistance == 0 || yDistance == 0) // If animal adjacent to target then eat it
					eat(target);

				else if (xDistance > yDistance)
					getSelf()->setX(getSelf()->getX() + Utility::signum(xDistance));

				else
					getSelf()->setY(getSelf()->getY() + Utility::signum(xDistance));
			}

			else if (old_level > Entity::MAX_OLD) // If old is maximum level then animal has to die
			{
				// state equals dying
			}

			else if (old_level >= Entity::MIN_REPRODUCTION_OLD && old_level <= Entity::MAX_REPRODUCTION_OLD) // If all is ok then reproduce
			{

			}

			animal->addOld(1);
			animal->addStarvation(1);
		}
	};

	class PredatorController : public AnimalController
	{
	public:
		PredatorController(Model& model, Entity* predatorAnimal) : AnimalController(model, predatorAnimal) {}

		virtual void eat(Entity* other)
		{
			;
		}

		virtual void reproduce(Entity* right)
		{
			;
		}
		virtual void die() { ; }

		virtual void actReflexively() { ; }

	};

	class FoodController : public Controller
	{
	public:
		FoodController(Model& model, Entity* food) : Controller(model, food) {}

		virtual void die() { ; }

		virtual void actReflexively() { ; }
	};

	class PlantController : public Controller
	{
	public:
		PlantController(Model& model, Entity* plant) : Controller(model, plant) {}

		virtual void reproduce(Entity* right)
		{
			;
		}
		virtual void die() { ; }

		virtual void actReflexively() { ; }
	};

private:
	Model model;
	ConsoleView view;

public:
	static const int INITIAL_PREDATOR_COUNT = 4;
	static const int INITIAL_PLANTEATING_COUNT = 7;
	static const int INITIAL_PLANT = 10;
	static const int INITIAL_PLANT_FOOD_COUNT = 3;
	static const int INITIAL_PREDATOR_FOOD_COUNT = 2;

	Game(int mapWidth, int mapHeight) : model(Model(mapWidth, mapHeight)), view(model)
	{
		assert(mapWidth * mapHeight >= INITIAL_PREDATOR_COUNT + INITIAL_PLANTEATING_COUNT +
			INITIAL_PLANT + INITIAL_PLANT_FOOD_COUNT + INITIAL_PREDATOR_FOOD_COUNT);

		std::vector<Position> freePositions(mapWidth * mapHeight);

		for (int i = 0; i < mapHeight * mapWidth; i++)
				freePositions[i] = Position(i % mapWidth + 1, i / mapWidth + 1);

		std::random_device rd;
		std::mt19937 gen(rd());

		for (int i = 0; i < INITIAL_PREDATOR_COUNT; i++)
		{
			std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);

			Position pos = freePositions[random_index];

			model.addPredator(pos);

			freePositions.erase(freePositions.begin() + random_index);
		}

		for (int i = 0; i < INITIAL_PLANTEATING_COUNT; i++)
		{
			std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);

			Position pos = freePositions[random_index];

			model.addPlantEating(pos);

			freePositions.erase(freePositions.begin() + random_index);
		}

		for (int i = 0; i < INITIAL_PLANT; i++)
		{
			std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);

			Position pos = freePositions[random_index];

			model.addPlant(pos);

			freePositions.erase(freePositions.begin() + random_index);
		}

		for (int i = 0; i < INITIAL_PLANT_FOOD_COUNT; i++)
		{
			std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);

			Position pos = freePositions[random_index];

			model.addPlantEatingFood(pos);

			freePositions.erase(freePositions.begin() + random_index);
		}

		for (int i = 0; i < INITIAL_PREDATOR_FOOD_COUNT; i++)
		{
			std::uniform_int_distribution<> distribution(0, freePositions.size() - 1);

			int random_index = distribution(gen);

			Position pos = freePositions[random_index];

			model.addPredatorFood(pos);

			freePositions.erase(freePositions.begin() + random_index);
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
					controller = new PredatorController(model, ent);

				controller->actReflexively();
			}

			Keyboard::handle_keyboard();
		}
	}
};

int main()
{
	Game game(10, 10);

	game.run();

	return 0;
}