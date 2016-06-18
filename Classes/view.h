#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объявили sfml объект "вид", который и является камерой
void getPlayerCoordinateForView(float x, float y, bool camp) { //функция для считывания координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и проверяем их, чтобы убрать края
	if (camp) {
		//Кампания
		if (x < 685) tempX = 685;//убираем из вида левую сторону
		if (x > 3925) tempX = 3925;//убираем из вида правую сторону
		if (y < 380) tempY = 380;//верхнюю сторону
		if (y > 3250) tempY = 3250;//нижнюю сторону	
	}
	else {
		//Выживание
		if (x < 685) tempX = 685;//убираем из вида левую сторону
		if (x > 1325) tempX = 1325;//убираем из вида правую сторону
		if (y < 380) tempY = 380;//верхнюю сторону
		if (y > 1650) tempY = 1650;//нижнюю сторону	
	}
	view.setCenter(tempX, tempY); //следим за игроком, передавая его координаты. 
	
}