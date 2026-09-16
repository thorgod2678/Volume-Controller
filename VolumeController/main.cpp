#ifndef UNICODE
#define UNICODE

#endif 
#define BTN_ID 2
#define BTNdb_ID 4
#define EDIT_ID 3
#define IDM_FILE_EXIT     100
#define IDM_FILE_COLOR     300
#define IDM_SETTINGS      101
#define IDM_HELP_ABOUT    102
#define BTNinf_ID   200

#include <windows.h>
#include <cstdlib>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Parser.h"

IAudioEndpointVolume* endpoint = nullptr;
IMMDeviceEnumerator* enumerator = nullptr;
IMMDevice* device = nullptr;

COLORREF bg_Color = RGB(255, 255, 255);

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
        
        HDC hdc = BeginPaint(hwnd, &ps);
        HBRUSH brush = CreateSolidBrush(bg_Color);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, brush);

        EndPaint(hwnd, &ps);
    }
    return 0;
    case WM_COMMAND: 
    {
        if (LOWORD(wParam) == BTN_ID) {
            HWND edit = GetDlgItem(hwnd, EDIT_ID);
            int len = GetWindowTextLength(edit);
            if (len >= 512) {
                MessageBox(hwnd, L"Your Input is too big limit it under 512 characters", L"Error", MB_ICONERROR);
                return 0;

            }
            wchar_t buffer[512];

            GetWindowText(edit, buffer, 512);
            
            float value = Parse(buffer); //float value received
            
           
            endpoint->SetMasterVolumeLevelScalar(value/100, nullptr);

            float currentDb;
            float currentScalar;
            endpoint->GetMasterVolumeLevel(&currentDb);

            endpoint->GetMasterVolumeLevelScalar(&currentScalar);

            std::wstring curvoldetailstring = L"Current Db: " + std::to_wstring(currentDb) + L"\nCurrent Scalar: " + std::to_wstring(currentScalar);
            MessageBox(hwnd, curvoldetailstring.c_str(), L"Volume Details After Change", MB_OK);

        }
        else if (LOWORD(wParam) == BTNdb_ID) {
            HWND edit = GetDlgItem(hwnd, EDIT_ID);
            int len = GetWindowTextLength(edit);
            if (len >= 512) {
                MessageBox(hwnd, L"Your Input is too big limit it under 512 characters", L"Error", MB_ICONERROR);
                return 0;

            }
            wchar_t buffer[512];

            GetWindowText(edit, buffer, 512);
            
            float value = Parse(buffer); //float value received

          
            endpoint->SetMasterVolumeLevel(value, nullptr);

            float currentDb;
            float currentScalar;
            endpoint->GetMasterVolumeLevel(&currentDb);

            endpoint->GetMasterVolumeLevelScalar(&currentScalar);

            std::wstring curvoldetailstring = L"Current Db: " + std::to_wstring(currentDb) + L"\nCurrent Scalar: " + std::to_wstring(currentScalar);
            MessageBox(hwnd, curvoldetailstring.c_str(), L"Volume Details After Change", MB_OK);

        }

        if (LOWORD(wParam) == IDM_FILE_EXIT) 
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        else if (LOWORD(wParam) == IDM_SETTINGS) 
        {
            MessageBox(hwnd, L"This is a simple program, which uses IAudioEndpoint to set the master volume, to any floating point value", L"Volume Controller", MB_OK);
            break;
        }
        else if (LOWORD(wParam) == IDM_HELP_ABOUT) 
        {
            MessageBox(hwnd, 
                LR"(Expressions are supported.

                + -> Addition
                - -> Subtraction
                * -> Multiplication
                / -> Division
                ^ -> Exponentiation
                () -> Parentheses for controlling calculation order
                PI -> Mathematical constant π
                E -> Mathematical constant e
                sin() -> Sine
                cos() -> Cosine
                tan() -> Tangent
                csc() -> Cosecant
                sec() -> Secant
                cot() -> Cotangent
                asin() -> Inverse sine
                acos() -> Inverse cosine
                atan() -> Inverse tangent
                sqrt() -> Square root
                abs() -> Absolute value
                floor() -> Rounds down to the nearest whole number
                ceil() -> Rounds up to the nearest whole number)"
                , L"Help", MB_OK);
            break;
        }

        if (LOWORD(wParam) == BTNinf_ID) 
        {
            float minDb;
            float maxDb;
            float incrementDb;
            float currentDb;
            float currentScalar;

            endpoint->GetVolumeRange(
                &minDb,
                &maxDb,
                &incrementDb
            );

            endpoint->GetMasterVolumeLevel(&currentDb);

            endpoint->GetMasterVolumeLevelScalar(&currentScalar);
            std::wstring audiodetailsstring = L"Device Details:" + (L"\n\n Min Db: " + std::to_wstring(minDb) + L"\n Max Db: " + std::to_wstring(maxDb) + L"\n Increment Db: " + std::to_wstring(incrementDb) +
                L"\n\nCurrent Volume Details:") + L"\n\n Current Db: " + std::to_wstring(currentDb) + L"\n Current Scalar: " + std::to_wstring(currentScalar);
            MessageBox(hwnd, audiodetailsstring.c_str(), L"Info", MB_OK);

            

            
        }

        if (LOWORD(wParam) == IDM_FILE_COLOR) 
        {
            CHOOSECOLOR cc = {};
            static COLORREF customColors[16] = {};

            cc.lStructSize = sizeof(CHOOSECOLOR);
            cc.hwndOwner = hwnd;
            cc.rgbResult = bg_Color;
            cc.lpCustColors = customColors;
           // cc.Flags = CC_FULLOPEN | CC_RGBINIT;

            if (ChooseColor(&cc))
            {
                bg_Color = cc.rgbResult;
            }

            InvalidateRect(hwnd, nullptr, TRUE);
        }
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int cleanup() {


    if (endpoint)
        endpoint->Release();

    if (device)
        device->Release();

    if (enumerator)
        enumerator->Release();

    CoUninitialize();

    return 0;


}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t WND_CLS[] = L"WINDOW";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WND_CLS;

    RegisterClass(&wc);

    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr))
        return 1;

    

#pragma region COM SETUP



    // 2. Create the device enumerator
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        nullptr,
        CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (void**)&enumerator
    );

    if (FAILED(hr))
         cleanup();

    // 3. Get the default playback device
    hr = enumerator->GetDefaultAudioEndpoint(
        eRender,
        eConsole,
        &device
    );

    if (FAILED(hr))
         cleanup();

    // 4. Get IAudioEndpointVolume
    hr = device->Activate(
        __uuidof(IAudioEndpointVolume),
        CLSCTX_ALL,
        nullptr,
        (void**)&endpoint
    );

    if (FAILED(hr))
         cleanup();



#pragma endregion


    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        WND_CLS,                     // Window class
        L"Volume Controller",    // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 360, 400,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    HMENU hMenu = CreateMenu();

    HMENU hFileMenu = CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, L"Exit");
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_COLOR, L"Background Color");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");

    AppendMenu(hMenu, MF_STRING, IDM_SETTINGS, L"About");
    AppendMenu(hMenu, MF_STRING, IDM_HELP_ABOUT, L"Help");

    SetMenu(hwnd, hMenu);

    HWND hEdit = CreateWindowEx(
        0,
        L"EDIT",
        L"0",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        20, 20, 300, 50,
        hwnd,       // parent window
        (HMENU)EDIT_ID,
        hInstance,
        nullptr
    );

    HWND hOk = CreateWindowEx(
        0,
        L"Button",
        L"Set Scalar",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        20, 100, 300, 50,
        hwnd,       // parent window
        (HMENU)BTN_ID,
        hInstance,
        nullptr
    );

    HWND hdb = CreateWindowEx(
        0,
        L"Button",
        L"Set Db",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        20, 180, 300, 50,
        hwnd,       // parent window
        (HMENU)BTNdb_ID,
        hInstance,
        nullptr
    );
    HWND hinfo = CreateWindowEx(
        0,
        L"Button",
        L"Get Info",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        20, 260, 300, 50,
        hwnd,       // parent window
        (HMENU)BTNinf_ID,
        hInstance,
        nullptr
    );

    ShowWindow(hwnd, nCmdShow);
    ShowWindow(hEdit, nCmdShow);
    ShowWindow(hOk, nCmdShow);
    ShowWindow(hdb, nCmdShow);


    // Run the message loop.
    MSG msg = { };
    while (GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }



    return 0;

}

