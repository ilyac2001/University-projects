#pragma once
const int QUANTITY_COUNTER = 9;
/*number_couter расположение на платформе
1, 2, 3
4, 5, 6
7, 8, 9*/
const int COEFFICIENTS_NUMBER_COUNTER[QUANTITY_COUNTER][2] = { //[QUANTITY_COUNTER][для x или y]
	{1, 1},
	{2, 1},
	{3, 1},
	{1, 2},
	{2, 2},
	{3, 2},
	{1, 3},
	{2, 3},
	{3, 3}
};

class Counter
{
public:
	Counter();
	Counter(double client_screen_length, double client_screen_widh, const int color[3], int number_platform, int number_couter);
	~Counter();

	POINT* GetPoints() {
		POINT* arr = new POINT[_on_circle.size()];
		copy(_on_circle.begin(), _on_circle.end(), arr);
		return arr;
	};

	int GetSize() { return _on_circle.size(); };

	POINT GetCinter() { return _center_circle; };

	int GetColorR() { return _counter_color[0]; };
	int GetColorG() { return _counter_color[1]; };
	int GetColorB() { return _counter_color[2]; };

	void RecalculationCoordinates(double center, double RADIUS_CIRCLE, int coordinate_quarter, int number_couter, int PUSH_FROM_THE_CENTER);

	void MoveCounter(int coordinate_quarter, bool direction, int step_move); //direction = true - движение от центра 

	void RotateCounter(int coordinate_quarter, bool direction, POINT center, double step_move); // direction = true - движение по часовой

	void RecolorCounter(HDC hdc, const int color[3]);

	void NullifyCounter();

	POINT _center_circle;
	std::vector<POINT> _on_circle; //по этим точками и будем рисовать
	int _counter_color[3];
	int _number_platform;
};