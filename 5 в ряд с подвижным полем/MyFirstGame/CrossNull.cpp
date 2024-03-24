#include "GamePlay.h"
#include "resource.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
RECT rect;
HDC hdc;
POINT cursor;

bool start = true;
bool first_player = true; //индикатор, чей сейчас ход
bool start_turn = true; //первой клик мыши, поставить фишку. false - повернуть платформу
Platform** platform = new Platform * [QUANTITY_PLATFORM];
Counter*** counter = new Counter * *[QUANTITY_PLATFORM]; //[coordinate_quarter][number_counter]
Player* player1 = new Player(COLOR_COUNTER_PLAYER1);
Player* player2 = new Player(COLOR_COUNTER_PLAYER2);
bool direction;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Game";

    WNDCLASS wc = { }; //отвечает за хар-ки окна 

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(hInstance, (LPWSTR)MAIN_ICON);

    RegisterClass(&wc);

    // Create the window.
    rect.left = 0;
    rect.top = 25;
    rect.right = SCREEN_LENGTH;
    rect.bottom = SCREEN_WIDH;
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Game",                        // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        rect.left, rect.top,
        rect.right - rect.left, rect.bottom - rect.top,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { }; //содержит информацию о сообщении, с помощью которых общаются окна
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void MyMenu(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    AppendMenu(hMenu, MF_POPUP, 1, L"Рестарт");
    AppendMenu(hMenu, MF_POPUP, 2, L"Правила игры");
    AppendMenu(hMenu, MF_POPUP, 3, L"Закрыть");
    SetMenu(hwnd, hMenu);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
        if (start) {
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(CreateSolidBrush(RGB(COLOR_BACKGROUND_WINDOW[0], COLOR_BACKGROUND_WINDOW[1], COLOR_BACKGROUND_WINDOW[2]))));
            GetClientRect(hwnd, &rect);
            CreatePlayingField(hdc, rect, platform, counter);
            start = false;
        }
        else {
            if (FallInPlatform(cursor, platform)) {
                if (first_player)
                {
                    Turn(hwnd, rect, hdc, wParam, lParam, first_player, start_turn, player1, player2, platform, counter, cursor, direction);
                }
                else
                {
                    Turn(hwnd, rect, hdc, wParam, lParam, first_player, start_turn, player2, player1, platform, counter, cursor, direction);
                }
            }
        }
        ValidateRect(hwnd, NULL);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        if (!(player1->CheckFiveInRow() || player2->CheckFiveInRow())) {
            InvalidateRect(hwnd, NULL, false);

            GetCursorPos(&cursor);
            ScreenToClient(hwnd, &cursor);

            SendMessage(hwnd, WM_PAINT, wParam, lParam);
        }
        return 0;
    }
    case WM_KEYDOWN: {
        if (wParam == 1 && wParam != 68) {
                while (true) {
                    if (GetAsyncKeyState(0x44) || GetAsyncKeyState(0x27)) {
                        direction = true;
                        break;
                    }
                    if (GetAsyncKeyState(0x41) || GetAsyncKeyState(0x25))
                    {
                        direction = false;
                        break;
                    }
                }
        }    
        return 0;
    }
    case WM_CREATE: {
        MyMenu(hwnd);
        return 0;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1:
            InvalidateRect(hwnd, NULL, false);
            PAINTSTRUCT ps;
            hdc = BeginPaint(hwnd, &ps);
            RestartGame(hwnd, rect, hdc, wParam, lParam, player1, player2, platform, counter, start, first_player, start_turn);
            ValidateRect(hwnd, NULL);
            EndPaint(hwnd, &ps);
            break;
        case 2:
            MessageBox(hwnd, TEXT("Здравствуй, игра довольно простая, но придётся привыкнуть) сейчас поймёшь.\nВ неё играют двое человек, совершая ходы в строго последовательности.\n1) Закрась любую фишку, просто нажав на неё мышкой), ходишь первый? – красишь в белый, второй? – чёрный\n2) Перекрасил? Хорошо, тогда кликай, на одну из четырёх, интересующую тебя платформу\n3) Выбери направление, в котором повернутся все фишки на этой платформе, хочешь по часовой – жми на D или стрелку вправо → , против – A или ←\n4) Всё, теперь так же делает твой оппонент. Кто из вас первый соберёт 5 фишек своего цвета в ряд (горизонталь, вертикаль, диагональ) победит!\n\np.s. Поворачивай поле, чтобы сломать комбинацию противнику и попасть в лучшую позицию, продумывай ходы на перёд, чтобы застать его врасплох)"), TEXT("Правила игры"), 0);
            break;
        case 3:
            DestroyWindow(hwnd);
            break;
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
};
