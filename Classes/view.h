#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//�������� sfml ������ "���", ������� � �������� �������
void getPlayerCoordinateForView(float x, float y, bool camp) { //������� ��� ���������� ��������� ������
	float tempX = x; float tempY = y;//��������� ����� ������ � ��������� ��, ����� ������ ����
	if (camp) {
		//��������
		if (x < 685) tempX = 685;//������� �� ���� ����� �������
		if (x > 3925) tempX = 3925;//������� �� ���� ������ �������
		if (y < 380) tempY = 380;//������� �������
		if (y > 3250) tempY = 3250;//������ �������	
	}
	else {
		//���������
		if (x < 685) tempX = 685;//������� �� ���� ����� �������
		if (x > 1325) tempX = 1325;//������� �� ���� ������ �������
		if (y < 380) tempY = 380;//������� �������
		if (y > 1650) tempY = 1650;//������ �������	
	}
	view.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 
	
}