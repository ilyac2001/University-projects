#pragma once
//тут описаны методы взаимодействия графики и логики
#include "HeaderGraphicsClass.h"
#include "HeaderLogicClass.h"

void CreatePlayingField(HDC hdc, RECT rect, Platform** platform, Counter*** counter) {

    HBRUSH hBrushPlatform;
    hBrushPlatform = CreateSolidBrush(RGB(COLOR_PLATFORM[0], COLOR_PLATFORM[1], COLOR_PLATFORM[2]));
    SelectObject(hdc, hBrushPlatform);
    for (int number_platform = 0; number_platform < QUANTITY_PLATFORM; number_platform++) { 
        platform[number_platform] = new Platform(rect.bottom, rect.right, COLOR_PLATFORM, number_platform);
        Polygon(hdc, platform[number_platform]->GetPoints(), 4);


        counter[number_platform] = new Counter * [QUANTITY_COUNTER];
        for (int number_counter = 0; number_counter < QUANTITY_COUNTER; number_counter++)
        {
            counter[number_platform][number_counter] = new Counter(rect.bottom, rect.right, COLOR_PLATFORM, number_platform, number_counter);
            Polyline(hdc, counter[number_platform][number_counter]->GetPoints(), counter[number_platform][number_counter]->GetSize());
        }

    }
    DeleteObject(hBrushPlatform);
    TextOut(hdc, 0, 0, TEXT("ход белых_"), 10);
    TextOut(hdc, 0, 15, TEXT("закраисить фишку____________"), 28);

};

void MoveObject(HDC hdc, RECT rect, Platform** platform, Counter*** counter, int number_platform, bool direction)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(COLOR_PLATFORM[0], COLOR_PLATFORM[1], COLOR_PLATFORM[2])); 
    SelectObject(hdc, hBrush);

    const double MOVE_PLATFORM = min(rect.right, rect.bottom) * (2 * sqrt(2) - 1) / 24;
    int step = MOVE_PLATFORM / 10;
    int counter_move = step;
    while (counter_move <= MOVE_PLATFORM)
    {
        platform[number_platform]->ErasePlarform(hdc); 

        platform[number_platform]->MovePlatform(number_platform, direction, step); 
        Polygon(hdc, platform[number_platform]->GetPoints(), 4); 

        for (int i = 0; i < QUANTITY_COUNTER; i++)
        {
            counter[number_platform][i]->MoveCounter(number_platform, direction, step); 
            hBrush = CreateSolidBrush(RGB(counter[number_platform][i]->GetColorR(), counter[number_platform][i]->GetColorG(), counter[number_platform][i]->GetColorB()));
            SelectObject(hdc, hBrush);
            Polygon(hdc, counter[number_platform][i]->GetPoints(), counter[number_platform][i]->GetSize()); 
        }

        Sleep(35);
        counter_move += step;
    }
    DeleteObject(hBrush);
};

void RotateObject(HDC hdc, RECT rect, Platform** platform, Counter*** counter, int number_platform, bool direction)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(COLOR_PLATFORM[0], COLOR_PLATFORM[1], COLOR_PLATFORM[2]));
    SelectObject(hdc, hBrush);

    HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));; 

    int px[4];
    int py[4];
    for (int i = 0; i < 4; i++)
    {
        px[i] = platform[number_platform]->_coordinates_peaks[i].x;
        py[i] = platform[number_platform]->_coordinates_peaks[i].y;
    }

    const double ROTATE_ANGLE = PI / 2;
    double step = ROTATE_ANGLE;// / 12;
    //double move = 0;
    //while (move < ROTATE_ANGLE)
    //{
    platform[number_platform]->ErasePlarform(hdc); 

    //platform[number_platform]->RotatePlatform(number_platform, direction, counter[number_platform][4]->GetCinter(), step);
    Polygon(hdc, platform[number_platform]->GetPoints(), 4); 

    for (int i = 0; i < QUANTITY_COUNTER; i++) 
    {
        counter[number_platform][i]->RotateCounter(number_platform, direction, counter[number_platform][4]->GetCinter(), step);
    }
    

    for (int i = 0; i < QUANTITY_COUNTER; i++)
    {
        hBrush = CreateSolidBrush(RGB(counter[number_platform][i]->GetColorR(), counter[number_platform][i]->GetColorG(), counter[number_platform][i]->GetColorB()));
        SelectObject(hdc, hBrush);
        Polygon(hdc, counter[number_platform][i]->GetPoints(), counter[number_platform][i]->GetSize());
    }

    /*Sleep(50);
    move += step;
    }*/

    /*тепер фишки у нас будут отрисовываться в следующем порядке
    7 4 1       3 6 9
    8 5 2       2 5 8
    9 6 3 или   1 4 7 в зависимости от direction
    перевишем каждый элемент (точки окружности и её цвет) на своё место, чтобы сохранить их порядок номерации
    центр их сотавим не изменным, да бы по ним определяем номерацию фишек
    */

    std::vector<POINT> new_on_circle[QUANTITY_COUNTER];
    int buf_color[9][3];
    for (int i = 0; i < QUANTITY_COUNTER; i++) {
        for (int j = 0; j < counter[number_platform][i]->_on_circle.size(); j++) {
            new_on_circle[i].push_back(counter[number_platform][i]->_on_circle[j]);
        }
        buf_color[i][0] = counter[number_platform][i]->GetColorR();
        buf_color[i][1] = counter[number_platform][i]->GetColorG();
        buf_color[i][2] = counter[number_platform][i]->GetColorB();
    }

    int i1, j1;
    int k = 6;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++) {
            if (direction) {
                i1 = i + j * 3;
                j1 = k - i * 3;
            }
            else
            {
                i1 = k - i * 3;
                j1 = i + j * 3;
            }
            counter[number_platform][i1]->_on_circle = new_on_circle[j1];
            for (int c = 0; c < 3; c++)
            {
                counter[number_platform][i1]->_counter_color[c] = buf_color[j1][c];
            }
            k++;
        }
        k = 6;
    }
    //теперь для точек платформы, простое решение - вернуть их стартовые координаты
    
    for (int i = 0; i < 4; i++)
    {
        platform[number_platform]->_coordinates_peaks[i].x = px[i];
        platform[number_platform]->_coordinates_peaks[i].y = py[i];
    }

    DeleteObject(hBrush);
    DeleteObject(hPen);
};

void MoveAndRotateObject(HDC hdc, RECT rect, Platform** platform, Counter*** counter, int number_platform, bool direction) //direction == true - вращение по часовой
{
    //MoveObject(hdc, rect, platform, counter, number_platform, true); //перемещаем от центра

    RotateObject(hdc, rect, platform, counter, number_platform, direction);

    //MoveObject(hdc, rect, platform, counter, number_platform, false); //перемещаем к центру, обратно

};

void CheckWin(Player* p1, Player* p2) {
    if (p1->CheckFiveInRow() && p2->CheckFiveInRow()) { MessageBox(NULL, TEXT("НИЧЬЯ!"), TEXT("Игра окончена!"), MB_OK); }
   
            if (p1->CheckFiveInRow()) { MessageBox(NULL, TEXT("БЕЛЫЕ победили!"), TEXT("Игра окончена!"), MB_OK); }
            else if (p2->CheckFiveInRow()) { MessageBox(NULL, TEXT("ЧЁРНЫЕ победили!"), TEXT("Игра окончена!"), MB_OK); }

};

int FallInPlatform(POINT cursor, Platform** platform) {

    if (cursor.x <= platform[0]->GetP1().x && cursor.y <= platform[0]->GetP1().y
        &&
        cursor.x >= platform[0]->GetP2().x && cursor.y >= platform[0]->GetP2().y)
    {
        return 1;
    }
    if (cursor.x >= platform[1]->GetP1().x && cursor.y <= platform[1]->GetP1().y
        &&
        cursor.x <= platform[1]->GetP2().x && cursor.y >= platform[1]->GetP2().y)
    {
        return 2;
    }
    if (cursor.x <= platform[2]->GetP1().x && cursor.y >= platform[2]->GetP1().y
        &&
        cursor.x >= platform[2]->GetP2().x && cursor.y <= platform[2]->GetP2().y)
    {
        return 3;
    }
    if (cursor.x >= platform[3]->GetP1().x && cursor.y >= platform[3]->GetP1().y
        &&
        cursor.x <= platform[3]->GetP2().x && cursor.y <= platform[3]->GetP2().y)
    {
        return 4;
    }

    return 0;
};

int FallInCounter(POINT cursor, RECT rect, Platform** platform, Counter*** counter) {
    int number_platform = FallInPlatform(cursor, platform) - 1;
    int number_counter;
    bool fall = false;
    const double SIDE_PLATFORM = min(rect.bottom, rect.right) / (2 * sqrt(2)) - PUSH_FROM_THE_CENTER;// - 40
    const double RADIUS_CIRCLE = (SIDE_PLATFORM) / (3 * 2) - REDUCE_RADIUS;
    for (number_counter = 1; number_counter <= QUANTITY_COUNTER; number_counter++)
    {
        int t = counter[number_platform][number_counter - 1]->_center_circle.x;
        int t2 = counter[number_platform][number_counter - 1]->_center_circle.y;

        if ((pow((cursor.x - counter[number_platform][number_counter - 1]->_center_circle.x), 2) + pow((cursor.y - counter[number_platform][number_counter - 1]->_center_circle.y), 2)) <= pow(RADIUS_CIRCLE, 2))
        {
            fall = true;
            break;
        }
    }
    if (!fall) { number_counter = 0; }

    return number_counter;
};

void Turn(HWND hwnd, RECT rect, HDC hdc, WPARAM wParam, LPARAM lParam, bool& first_player, bool& start_turn, Player* p1, Player* p2, Platform** platform, Counter*** counter, POINT cursor, bool& direction) { //p1 - ходит

    int number_platform = FallInPlatform(cursor, platform) - 1;
    if (start_turn)
    {
        if (FallInCounter(cursor, rect, platform, counter)) {
            int number_counter = FallInCounter(cursor, rect, platform, counter) - 1;
            if (p1->SetMatrix(number_platform, number_counter, p2->GetMatrix())) {
                counter[number_platform][number_counter]->RecolorCounter(hdc, p1->GetColor());

                CheckWin(p1, p2);

                start_turn = false;
                TextOut(hdc, 0, 15, TEXT("выбрать платформу___________"), 28);
            }
        }
    }
    else
    {
            TextOut(hdc, 0, 15, TEXT("выбрать направление поворота"), 28);
            SendMessage(hwnd, WM_KEYDOWN, wParam, lParam);

            p1->RotateMatrix(number_platform, direction);
            p2->RotateMatrix(number_platform, direction);
            MoveAndRotateObject(hdc, rect, platform, counter, number_platform, direction);

            CheckWin(p1, p2);

            start_turn = true;
            if (first_player) { first_player = false; }
            else { first_player = true; }
            if (first_player) {
                TextOut(hdc, 0, 0, TEXT("ход белых_"), 10);
                TextOut(hdc, 0, 15, TEXT("закрасить фишку____________"), 28);
            }
            else {
                TextOut(hdc, 0, 0, TEXT("ход чёрных"), 10);
                TextOut(hdc, 0, 15, TEXT("закрасить фишку____________"), 28);
            }
    };
};

void RestartGame(HWND hwnd, RECT rect, HDC hdc, WPARAM wParam, LPARAM lParam, Player* p1, Player* p2, Platform** platform, Counter*** counter, bool &start, bool &first_player, bool &start_turn) {
    start = true;
    first_player = true;
    start_turn = true;
    for (int number_platform = 0; number_platform < QUANTITY_PLATFORM; number_platform++)
    {
        for (int number_counter = 0; number_counter < QUANTITY_COUNTER; number_counter++)
        {
            counter[number_platform][number_counter]->NullifyCounter();
        }
        platform[number_platform]->ErasePlarform(hdc);
    }

    p1->NullifyMatrix();
    p2->NullifyMatrix();
    InvalidateRect(hwnd, NULL, false);

    SendMessage(hwnd, WM_PAINT, wParam, lParam);
}