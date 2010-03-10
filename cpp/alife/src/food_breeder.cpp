#include "food_breeder.h"
#include "world.h"

FoodBreeder::FoodBreeder( int _numFood)
{
    minFoodItems = _numFood;
}
void FoodBreeder::onIdle( World& w)
{
    if (w.getNumFood() < minFoodItems ){
	int nNewFoodItems = minFoodItems - w.getNumFood();
	while( nNewFoodItems -- > 0 ){
	    w.addFood( FoodPtr( new Food( vec2(sqr(frnd())*w.getSize().x,
					       sqr(frnd())*w.getSize().y) ) ) );
	}
    }
}
