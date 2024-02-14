// Oblig1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Oblig1.h"
#include <deque>
#include <time.h>

#define MAX_LOADSTRING 100
#define ROAD_WIDTH 100
#define CAR_WIDTH 20
#define CAR_LENGTH 40
#define TIMER_LIGHTS 1
#define TIMER_CARS 2




// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

std::deque<COLORREF> northCarsColor;
std::deque<int> nortCarsPosition;
std::deque<COLORREF> westCarsColors;
std::deque<int> westCarsPosition;
int pN = 100;
int pW = 100;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OBLIG1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OBLIG1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OBLIG1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OBLIG1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int lightIndex = 0;
    static COLORREF lightColors[] = { RGB(255, 0, 0), RGB(255, 255, 0), RGB(0, 255, 0), RGB(255, 255, 0) }; // rød, rød+gul, grønn, gul

    switch (message)
    {
    case WM_CREATE:
    {
        // Sett opp startposisjoner for bilene
         // Antall biler du vil ha ved starten
        for (int i = 0; i < 5; i++) {
            int northCarPosition = -40;
            int westCarPosition = -40;

            // Legg til startposisjonene i køene
            nortCarsPosition.push_back(northCarPosition);
            westCarsPosition.push_back(westCarPosition);
        }

        SetTimer(hWnd, TIMER_LIGHTS, 2000, 0);
        SetTimer(hWnd, TIMER_CARS, 100, 0);
        srand(time(0));  // ???
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT dims;
            GetClientRect(hWnd, &dims);
            POINT screenCenter = { dims.right / 2, dims.bottom / 2 };

            HPEN outline = CreatePen(PS_SOLID, 2, RGB(191, 191, 191));
            HGDIOBJ hOrgPen = SelectObject(hdc, outline);

             // Tegn trafikklyset
            HBRUSH hBrush = CreateSolidBrush(RGB(242, 242, 242));;
            HGDIOBJ hOrgBrush = SelectObject(hdc, hBrush);
            HBRUSH redBrush = CreateSolidBrush(lightColors[0]);
            HBRUSH yellowBrush = CreateSolidBrush(lightColors[1]);
            HBRUSH greenBrush = CreateSolidBrush(lightColors[2]);

            //Trafikklys boksen
            SelectObject(hdc, outline);
            SelectObject(hdc, hBrush);

            Rectangle(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y - (ROAD_WIDTH * 2), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y - (ROAD_WIDTH / 2));
            Rectangle(hdc, screenCenter.x - (ROAD_WIDTH * 2), screenCenter.y + (ROAD_WIDTH / 2), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y + ROAD_WIDTH);

            // Placeholder for lysene
            hBrush = CreateSolidBrush(RGB(102, 102, 102));
            SelectObject(hdc, outline);
            SelectObject(hdc, hBrush);
            for (double i = 2; i > 0.5; i -= 0.5) {
                Ellipse(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y - (ROAD_WIDTH * i), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y - (ROAD_WIDTH * (i - 0.5)));
                Ellipse(hdc, screenCenter.x - (ROAD_WIDTH * i), screenCenter.y + (ROAD_WIDTH / 2), screenCenter.x - (ROAD_WIDTH * (i - 0.5)), screenCenter.y + ROAD_WIDTH);
            }

            switch (lightIndex) {
            case 0: // rod eller gronn
                SelectObject(hdc, redBrush);
                Ellipse(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y - (ROAD_WIDTH * 2), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y - (ROAD_WIDTH * 1.5));
                SelectObject(hdc, greenBrush);
                Ellipse(hdc, screenCenter.x - (ROAD_WIDTH * 2), screenCenter.y + (ROAD_WIDTH / 2), screenCenter.x - (ROAD_WIDTH * 1.5), screenCenter.y + ROAD_WIDTH);
                break;
            case 1: // rod og gul eller gul
                SelectObject(hdc, redBrush);
                Ellipse(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y - (ROAD_WIDTH * 2), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y - (ROAD_WIDTH * 1.5));
                SelectObject(hdc, yellowBrush);
                Ellipse(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y - (ROAD_WIDTH * 1.5), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y - ROAD_WIDTH);
                Ellipse(hdc, screenCenter.x - (ROAD_WIDTH * 1.5), screenCenter.y + (ROAD_WIDTH / 2), screenCenter.x - ROAD_WIDTH, screenCenter.y + ROAD_WIDTH);
                break;
            case 2: // gronn eller rod
                SelectObject(hdc, greenBrush);
                Ellipse(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y - ROAD_WIDTH, screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y - (ROAD_WIDTH / 2));
                SelectObject(hdc, redBrush);
                Ellipse(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y + (ROAD_WIDTH / 2), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y + ROAD_WIDTH);
                break;
            case 3: // gul eller rod og gul
                SelectObject(hdc, redBrush);
                Ellipse(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y + (ROAD_WIDTH / 2), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y + ROAD_WIDTH);
                SelectObject(hdc, yellowBrush);
                Ellipse(hdc, screenCenter.x - ROAD_WIDTH, screenCenter.y - (ROAD_WIDTH * 1.5), screenCenter.x - (ROAD_WIDTH / 2), screenCenter.y - ROAD_WIDTH);
                Ellipse(hdc, screenCenter.x - (ROAD_WIDTH * 1.5), screenCenter.y + (ROAD_WIDTH / 2), screenCenter.x - ROAD_WIDTH, screenCenter.y + ROAD_WIDTH);
                break;
            }

            // Veikrysset
            HBRUSH roadBrush = CreateSolidBrush(RGB(191, 191, 191));
            SelectObject(hdc, roadBrush);
            // Topp til bunn
            Rectangle(hdc, dims.left, screenCenter.y - (ROAD_WIDTH / 2), dims.right, screenCenter.y + (ROAD_WIDTH / 2));
            
            //Venstre til hoyre
            Rectangle(hdc, screenCenter.x - (ROAD_WIDTH / 2), dims.top, screenCenter.x + (ROAD_WIDTH / 2), dims.bottom);

         
            // Tegner bilene
            // Vest
            outline = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            SelectObject(hdc, outline);
            HBRUSH carBrush = CreateSolidBrush(RGB(0, 0, 111));
            SelectObject(hdc, carBrush);
            auto pos = westCarsPosition.begin();
            while (pos != westCarsPosition.end()) {
                Rectangle(hdc, *pos, screenCenter.y - (CAR_WIDTH / 2), CAR_LENGTH + *pos, screenCenter.y + (CAR_WIDTH / 2));
                pos++;
            }

            // Nord
            auto posN = nortCarsPosition.begin();
            while (posN != nortCarsPosition.end()) {
               Rectangle(hdc, screenCenter.x - (CAR_WIDTH / 2), *posN, screenCenter.x + CAR_WIDTH / 2, *posN + CAR_LENGTH);
               posN++;
            }

            SelectObject(hdc, hOrgBrush);
            SelectObject(hdc, hOrgPen);
            DeleteObject(roadBrush);
            DeleteObject(hBrush);
            DeleteObject(redBrush);
            DeleteObject(yellowBrush);
            DeleteObject(greenBrush);
            DeleteObject(outline);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
    {
        RECT dims;
        GetClientRect(hWnd, &dims);
        POINT screenCenter = { dims.right / 2, dims.bottom / 2 };

        switch (wParam) { // ?????
        case TIMER_LIGHTS:
                lightIndex = ++lightIndex % 4;
                SetTimer(hWnd, TIMER_LIGHTS, lightIndex % 2 == 1 ? 1000 : 5000, 0);
                break;
        case TIMER_CARS:
            int prevPos = 1000;
            int minDistance = 5;
            int speed = 15;

            // Vest
            auto pos = westCarsPosition.begin();
            while (pos != westCarsPosition.end()) {
                if (*pos + + minDistance + CAR_LENGTH + speed < prevPos) // mellomrom mellom bilene
                { 
                    if (lightIndex != 0  // rodt eller gult
                        && (*pos + CAR_LENGTH) <= (screenCenter.x - ROAD_WIDTH / 2) // foer lyskrysset
                        && *pos + CAR_LENGTH + speed >= (screenCenter.x - ROAD_WIDTH / 2)) { // en til bevegelse passerer lyskrysset
                        (*pos) = screenCenter.x - ROAD_WIDTH / 2 - CAR_LENGTH;
                    }
                    else {
                        (*pos) += speed;
                    }
                }
                prevPos = *pos;
                pos++;
            }

            // Nord
            int prevPosN = 1000;
            auto posN = nortCarsPosition.begin();
            while (posN != nortCarsPosition.end()) {
                if (*posN + minDistance + CAR_LENGTH + speed < prevPosN) {

                    if (lightIndex != 2 
                        && (*posN + CAR_LENGTH) <= (screenCenter.y - ROAD_WIDTH / 2)
                        && *posN + CAR_LENGTH + speed >= (screenCenter.y - ROAD_WIDTH / 2)) {
                        *posN = screenCenter.y - ROAD_WIDTH / 2 - CAR_LENGTH;
                    }
                    else {
                        (*posN) += speed;
                    }
                    

                }
                prevPosN = *posN;
                posN++;
            }
            SetTimer(hWnd, TIMER_CARS, 1000, 0);

            break;

            }

            InvalidateRect(hWnd, 0, true);
            break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
