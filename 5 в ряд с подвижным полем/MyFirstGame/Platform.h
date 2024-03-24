#pragma once

const int COLOR_PLATFORM[3] = { 0, 255, 51 };
const int QUANTITY_PLATFORM = 4;
/*расположение платформ на ИП (игровое поле)
1, 2
3, 4*/
class Platform
{
public:
	Platform();
	Platform(double client_screen_length, double client_screen_widh, const int color[3], int number_platform);
	~Platform();

	POINT* GetPoints() { return _coordinates_peaks; };
	int GetSize() { return sizeof(_coordinates_peaks); };

	//двумя точками можно определить прямоугольник
	POINT GetP1() { return _coordinates_peaks[0]; }
	POINT GetP2() { return _coordinates_peaks[2]; }

	void RecalculationCoordinates(double center, double side, int coordinate_quarter, int push_from_the_center);

	void MovePlatform(int coordinate_quarter, bool direction, int move_step); //direction = true - движение от центра 

	void RotatePlatform(int coordinate_quarter, bool direction, POINT center, double step_move); //direction = true - по часовой

	void ErasePlarform(HDC hdc);

	int _number_platform;
	POINT _coordinates_peaks[4];
};