#include <iostream>
#include <vector>
#include "entities.h"
#include "map.h"
#include "utilities.h"

class Game
{
	class Model
	{
		Map map;
		std::vector<Entity> entities;

	public:
		Model(int mapWidth, int mapHeight) : map(Map(mapWidth, mapHeight)) {}
		std::vector<Entity>& get_entities() { return entities; }

		Map& getMap() { return map; }

		Entity* getClosestPlant(Entity& ent)
		{
			Entity* closest = nullptr;
			int closestDistance;

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* other = &entities[i];

				if (other->isPlant())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
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
				Entity* other = &entities[i];

				if (other->isPlantEatingFood())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
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
				Entity* other = &entities[i];

				if (other->isPredatorFood())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
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
				Entity* other = &entities[i];

				if (other->isPlantEating())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
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
				Entity* other = &entities[i];

				if (other->isPredator())
				{
					closest = other;
					closestDistance = map.getDistance(*closest, ent);
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

		void render(Model& model)
		{
			// ...
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

			if (starvation_level > Entity::DANGEROUS_STARVATION) // If animal is starving then find food
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

			else if (health < Entity::SAFE_HEALTH_LEVEL) // If health less than it's normal then find food
			{
				PlantEating* animal = static_cast<PlantEating*>(getSelf());

				int health = animal->getHealth();
				int starvation_level = animal->getStarvation();
				int old_level = animal->getOld();

				// if (state is attacked) then run away

				if (starvation_level > Entity::DANGEROUS_STARVATION) // If animal is starving then find food
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
			}
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
		FoodController(Model& model, Entity* controller) : Controller(model, controller) {}

		virtual void die(PlantEating* ent) { ; }

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
	View view;

public:
	Game(int mapWidth, int mapHeight) : model(Model(mapWidth, mapHeight)), view(model)
	{
		
	}

	void run()
	{
		while (true) // Game loop
		{
			view.render(model); // rendering map to console

			for (int i = 0; i < model.get_entities().size(); i++)
			{
				Controller* controller = nullptr;
				Entity ent = model.get_entities()[i];

				if (ent.isPlantEating())
					controller = new PlantEatingController(model, &ent);

				else if (ent.isPredator())
					controller = new PredatorController(model, &ent);
				
				else if (ent.isPlantEatingFood() || ent.isPredatorFood())
					controller = new FoodController(model, &ent);
				
				else if (ent.isPlant())
					controller = new PredatorController(model, &ent);

				controller->actReflexively();
			}
		}
	}
};

int main()
{

	return 0;
}