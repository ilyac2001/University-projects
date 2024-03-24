#include "HeaderGraphicsClass.h"

Platform::Platform(double client_screen_length, double client_screen_widh, const int color[3], int number_platform)
{
	const double CENTER_COORDINATES = min(client_screen_length, client_screen_widh) / 2;
	const double SIDE_PLATFORM = min(client_screen_length, client_screen_widh) / (2 * sqrt(2)) - PUSH_FROM_THE_CENTER;//-40 если не задевать текст подсказки

	RecalculationCoordinates(CENTER_COORDINATES, SIDE_PLATFORM, number_platform, PUSH_FROM_THE_CENTER);
}

void Platform::RecalculationCoordinates(double center, double side, int coordinate_quarter, int PUSH_FROM_THE_CENTER) {
	if (coordinate_quarter < 0 or coordinate_quarter > 3) { throw "parameter coordinate_quarter implies values { 0, 1, 2, 3}"; };
	//платформы пронумерованы и расположена, как четверти в привычной координатной плоскости (оси x, y положительно направлены в право и вверх соответственно)
	POINT p1, p2, p3, p4;

	p1.x = center + PUSH_FROM_THE_CENTER * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0]; //левая верхняя вершина платформы
	p1.y = center + PUSH_FROM_THE_CENTER * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1];
	//далее обход по часовой
	p2.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * side + PUSH_FROM_THE_CENTER * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0];
	p2.y = center + PUSH_FROM_THE_CENTER * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1];

	p3.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * side + PUSH_FROM_THE_CENTER * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0];
	p3.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * side + PUSH_FROM_THE_CENTER * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1];

	p4.x = center + PUSH_FROM_THE_CENTER * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0];
	p4.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * side + PUSH_FROM_THE_CENTER * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1];

	_coordinates_peaks[0] = p1;
	_coordinates_peaks[1] = p2;
	_coordinates_peaks[2] = p3;
	_coordinates_peaks[3] = p4;
};

void Platform::ErasePlarform(HDC hdc) {
	HBRUSH hBrush = CreateSolidBrush(RGB(COLOR_BACKGROUND_WINDOW[0], COLOR_BACKGROUND_WINDOW[1], COLOR_BACKGROUND_WINDOW[2]));
	SelectObject(hdc, hBrush);
	Polygon(hdc, GetPoints(), QUANTITY_PLATFORM);
	DeleteObject(hBrush);

	HPEN hPen = CreatePen(PS_INSIDEFRAME, 0, RGB(COLOR_BACKGROUND_WINDOW[0], COLOR_BACKGROUND_WINDOW[1], COLOR_BACKGROUND_WINDOW[2]));
	SelectObject(hdc, hPen);
	Polygon(hdc, GetPoints(), QUANTITY_PLATFORM);
	DeleteObject(hPen);

	hBrush = CreateSolidBrush(RGB(COLOR_PLATFORM[0], COLOR_PLATFORM[1], COLOR_PLATFORM[2]));
	hPen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
};

void Platform::MovePlatform(int coordinate_quarter, bool direction, int move_step) {
	if (coordinate_quarter < 0 or coordinate_quarter > 3) { throw "parameter coordinate_quarter implies values { 0, 1, 2, 3}"; };
	int coefficient = -1;
	if (direction) { coefficient *= -1; }; //true - перемещение от центра

	for (int i = 0; i < QUANTITY_PLATFORM; i++) {
		_coordinates_peaks[i].x += COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * move_step * coefficient + 0.5; //+ 0.5 обуславливается погрешностью вычислений, потерей данных
		_coordinates_peaks[i].y += COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * move_step * coefficient + 0.5;
	}

}
void Platform::RotatePlatform(int coordinate_quarter, bool direction, POINT center, double step_move)
{
	if (coordinate_quarter < 0 || coordinate_quarter > 3) { throw "parameter number_platform implies values { 0, 1, 2, 3}"; };
	int c_direction = 1;
	if (!direction) { c_direction *= -1; };
	double x0 = center.x;
	double y0 = center.y;
	double new_x;
	double new_y;

	for (int i = 0; i < 4; i++)
	{
		new_x = x0 + (_coordinates_peaks[i].x - x0) * cos(c_direction * step_move) - (_coordinates_peaks[i].y - y0) * sin(c_direction * step_move);
		new_y = y0 + (_coordinates_peaks[i].y - y0) * cos(c_direction * step_move) + (_coordinates_peaks[i].x - x0) * sin(c_direction * step_move);
		_coordinates_peaks[i].x = new_x;
		_coordinates_peaks[i].y = new_y;
	}
}

Platform::~Platform()
{
}

Counter::Counter(double client_screen_length, double client_screen_widh, const int color[3], int number_platform, int number_couter) {

	const double CENTER_COORDINATES = min(client_screen_length, client_screen_widh) / 2;
	const double SIDE_PLATFORM = min(client_screen_length, client_screen_widh) / (2 * sqrt(2)) - PUSH_FROM_THE_CENTER;//-40 если не задевать текст подсказки
	const double RADIUS_CIRCLE = (SIDE_PLATFORM) / (3 * 2) - REDUCE_RADIUS;

	_number_platform = number_platform;
	for (int i = 0; i < 3; i++)
	{
		_counter_color[i] = color[i];
	}
	RecalculationCoordinates(CENTER_COORDINATES, RADIUS_CIRCLE, number_platform, number_couter, PUSH_FROM_THE_CENTER);
}

void Counter::RecalculationCoordinates(double center, double RADIUS_CIRCLE, int coordinate_quarter, int number_counter, int PUSH_FROM_THE_CENTER)
{
	if (coordinate_quarter < 0 || coordinate_quarter > 3) { throw "parameter number_platform implies values { 0, 1, 2, 3}"; };
	if (number_counter < 0 || number_counter > 9) { throw "parameter number_couter implies values { 0, 1, 2, 3, 4, 5, 6, 7, 8}"; };

	switch (coordinate_quarter + 1)
	{
	case 1:
		_center_circle.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * COEFFICIENTS_NUMBER_COUNTER[8 - number_counter][0] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * RADIUS_CIRCLE;
		_center_circle.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * COEFFICIENTS_NUMBER_COUNTER[8 - number_counter][1] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * RADIUS_CIRCLE;
		break;
	case 2:
		if (number_counter < 3)
		{
			_center_circle.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * COEFFICIENTS_NUMBER_COUNTER[number_counter + 6][0] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * RADIUS_CIRCLE;
			_center_circle.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * COEFFICIENTS_NUMBER_COUNTER[number_counter + 6][1] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * RADIUS_CIRCLE;
		}
		else if (number_counter > 5)
		{
			_center_circle.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * COEFFICIENTS_NUMBER_COUNTER[number_counter - 6][0] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * RADIUS_CIRCLE;
			_center_circle.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * COEFFICIENTS_NUMBER_COUNTER[number_counter - 6][1] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * RADIUS_CIRCLE;
		}
		else
		{
			_center_circle.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * COEFFICIENTS_NUMBER_COUNTER[number_counter][0] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * RADIUS_CIRCLE;
			_center_circle.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * COEFFICIENTS_NUMBER_COUNTER[number_counter][1] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * RADIUS_CIRCLE;
		};
		break;
	case 3:
		if (number_counter == 0 || number_counter == 3 || number_counter == 6)
		{
			_center_circle.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * COEFFICIENTS_NUMBER_COUNTER[number_counter + 2][0] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * RADIUS_CIRCLE;
			_center_circle.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * COEFFICIENTS_NUMBER_COUNTER[number_counter + 2][1] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * RADIUS_CIRCLE;
		}
		else if (number_counter == 2 || number_counter == 5 || number_counter == 8)
		{
			_center_circle.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * COEFFICIENTS_NUMBER_COUNTER[number_counter - 2][0] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * RADIUS_CIRCLE;
			_center_circle.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * COEFFICIENTS_NUMBER_COUNTER[number_counter - 2][1] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * RADIUS_CIRCLE;
		}
		else
		{
			_center_circle.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * COEFFICIENTS_NUMBER_COUNTER[number_counter][0] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * RADIUS_CIRCLE;
			_center_circle.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * COEFFICIENTS_NUMBER_COUNTER[number_counter][1] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * RADIUS_CIRCLE;

		};
		break;
	default:
		_center_circle.x = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * COEFFICIENTS_NUMBER_COUNTER[number_counter][0] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * RADIUS_CIRCLE;
		_center_circle.y = center + COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * COEFFICIENTS_NUMBER_COUNTER[number_counter][1] * 2 * (RADIUS_CIRCLE + REDUCE_RADIUS) - COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * RADIUS_CIRCLE;
		break;
	}
	POINT new_point;
	POINT start_point = _center_circle;
	start_point.x += RADIUS_CIRCLE;
	_on_circle.push_back(start_point);
	for (double angle = 0; angle < 2 * PI; angle += PI / 512)
	{
		new_point.x = _center_circle.x + (start_point.x - _center_circle.x) * cos(angle) - (start_point.y - _center_circle.y) * sin(angle);
		new_point.y = _center_circle.y + (start_point.y - _center_circle.y) * cos(angle) + (start_point.x - _center_circle.x) * sin(angle);
		_on_circle.push_back(new_point);
	}
	_on_circle.push_back(start_point);
}

void Counter::MoveCounter(int coordinate_quarter, bool direction, int step_move)
{
	if (coordinate_quarter < 0 || coordinate_quarter > 3) { throw "parameter coordinate_quarter implies values { 0, 1, 2, 3}"; };
	int coefficient = -1;
	if (direction) { coefficient *= -1; }; //true - перемещение от центра

	_center_circle.x += coefficient * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * step_move + 0.5;//+ 0.5 обуславливается погрешностью вычислений, потерей данных
	_center_circle.y += coefficient * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * step_move + 0.5;
	for (int i = 0; i < _on_circle.size(); i++) {
		_on_circle[i].x += coefficient * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][0] * step_move + 0.5;
		_on_circle[i].y += coefficient * COORDINATE_QUARTER_COEFFICIENTS[coordinate_quarter][1] * step_move + 0.5;

	};
}

void Counter::RotateCounter(int coordinate_quarter, bool direction, POINT center, double step_move)
{
	if (coordinate_quarter < 0 || coordinate_quarter > 3) { throw "parameter number_platform implies values { 0, 1, 2, 3}"; };
	int c_direction = 1;
	if (!direction) { c_direction *= -1; };
	double x0 = center.x;
	double y0 = center.y;
	double new_x;
	double new_y;

	for (int i = 0; i < _on_circle.size(); i++)
	{
		new_x = x0 + (_on_circle[i].x - x0) * cos(c_direction * step_move) - (_on_circle[i].y - y0) * sin(c_direction * step_move);
		new_y = y0 + (_on_circle[i].y - y0) * cos(c_direction * step_move) + (_on_circle[i].x - x0) * sin(c_direction * step_move);
		_on_circle[i].x = new_x;
		_on_circle[i].y = new_y;
	}
}

void Counter::RecolorCounter(HDC hdc, const int color[3])
{
	for (int i = 0; i < 3; i++)
	{
		_counter_color[i] = color[i];
	}
	HBRUSH hBrush;
	hBrush = CreateSolidBrush(RGB(_counter_color[0], _counter_color[1], _counter_color[2])); //фон платформы
	SelectObject(hdc, hBrush);
	Polygon(hdc, GetPoints(), GetSize());
}

void Counter::NullifyCounter()
{
	for (int i = 0; i < 3; i++)
	{
		_counter_color[i] = COLOR_PLATFORM[i];
	}
}

