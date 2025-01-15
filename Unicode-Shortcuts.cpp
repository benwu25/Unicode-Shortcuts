#include "framework.h"
#include "Unicode-Shortcuts.h"

#include <windows.h>
#include <winbase.h>
#include <winuser.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>

#define PAIR 2

#define A_KEY 0x41
#define B_KEY 0x42
#define C_KEY 0x43
#define D_KEY 0x44
#define E_KEY 0x45
#define F_KEY 0x46
#define G_KEY 0x47
#define H_KEY 0x48
#define I_KEY 0x49
#define J_KEY 0x4A
#define K_KEY 0x4B
#define L_KEY 0x4C
#define M_KEY 0x4D
#define N_KEY 0x4E
#define O_KEY 0x4F
#define P_KEY 0x50
#define Q_KEY 0x51
#define R_KEY 0x52
#define S_KEY 0x53
#define T_KEY 0x54
#define U_KEY 0x55
#define V_KEY 0x56
#define W_KEY 0x57
#define X_KEY 0x58
#define Y_KEY 0x59
#define Z_KEY 0x5A
#define ONE_KEY 0x31
#define TWO_KEY 0x32
#define THREE_KEY 0x33
#define FOUR_KEY 0x34
#define FIVE_KEY 0x35
#define SIX_KEY 0x36
#define SEVEN_KEY 0x37
#define EIGHT_KEY 0x38
#define NINE_KEY 0x39
#define ZERO_KEY 0x30
#define BACKSLASH VK_OEM_5
#define END '\0'
#define LBRACE VK_OEM_4
#define RBRACE VK_OEM_6


typedef struct Bit {
    unsigned char b : 2;
} Bit;

int checkEqual(USHORT*);
void sendInput(USHORT, int);
void makeSurrogates(WORD*, UINT);
void sendInputLarge(WORD*, int);
USHORT processLarge(USHORT[], UINT, USHORT);
USHORT processSmall(USHORT[], USHORT, USHORT);
USHORT arr_size(USHORT[]);

int checkEqual(std::vector<USHORT>&);
USHORT processLarge(std::vector<USHORT>&, UINT, USHORT);
USHORT processSmall(std::vector<USHORT>&, USHORT, USHORT);
USHORT arr_size(std::vector<USHORT>&);

std::vector<USHORT> chars;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Unicode Shortcuts!",          // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

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

    // Create Raw Input Device to listen for keystrokes.
    RAWINPUTDEVICE Rid[1];

    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x06;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hwnd;

    RegisterRawInputDevices(Rid, 1, sizeof(RAWINPUTDEVICE));

    // Run the message loop.

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
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
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    case WM_INPUT: // Store the keystroke, check for a match and send input.
    {
        UINT dwSize;
        GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        LPBYTE lpb = new BYTE[dwSize];
        GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
        RAWINPUT* raw = (RAWINPUT*)(lpb);

        USHORT x = raw->data.keyboard.VKey;
        UINT y = raw->data.keyboard.Message;

        if (y == WM_KEYDOWN)
        {
            // only add with backslash identifier in the vector
            if ((x == VK_OEM_5 || (!chars.empty() && chars[0] == VK_OEM_5)))
            {
                chars.push_back(x);
            }

            Bit possibStreak = { 0 };
            USHORT result;
            std::vector<USHORT> names;
            std::vector<USHORT>& ref = names;

            //-> \nabla
            names.insert(names.end(), { BACKSLASH, N_KEY, A_KEY, B_KEY, L_KEY, A_KEY, END });
            possibStreak.b |= processSmall(ref, 0x2207, arr_size(ref));
            names.clear();

            //-> \lineint
            names.insert(names.end(), { BACKSLASH, L_KEY, I_KEY, N_KEY, E_KEY, I_KEY, N_KEY, T_KEY, END });
            possibStreak.b |= processSmall(ref, 0x222E, arr_size(ref));
            names.clear();

            //-> \surfint
            names.insert(names.end(), { BACKSLASH, S_KEY, U_KEY, R_KEY, F_KEY, I_KEY, N_KEY, T_KEY, END });
            possibStreak.b |= processSmall(ref, 0x222F, arr_size(ref));
            names.clear();

            // Bold t.
            names.insert(names.end(), { BACKSLASH, M_KEY, A_KEY, T_KEY, H_KEY, B_KEY, F_KEY, VK_SHIFT, LBRACE, T_KEY, VK_SHIFT, RBRACE, END });
            possibStreak.b |= processLarge(ref, 0x1D42D, arr_size(ref));
            names.clear();

            // Bold x.
            names.insert(names.end(), { BACKSLASH, M_KEY, A_KEY, T_KEY, H_KEY, C_KEY, B_KEY, F_KEY, VK_SHIFT, LBRACE, X_KEY, VK_SHIFT, RBRACE, END });
            possibStreak.b |= processLarge(ref, 0x1D499, arr_size(ref));
            names.clear();

            // \cdot
            names.insert(names.end(), { BACKSLASH, D_KEY, O_KEY, T_KEY, END });
            possibStreak.b |= processSmall(ref, 0x2981, arr_size(ref));
            names.clear();

            // sqrt
            names.insert(names.end(), { BACKSLASH, S_KEY, Q_KEY, R_KEY, T_KEY, END });
            possibStreak.b |= processSmall(ref, 0x221A, arr_size(ref));
            names.clear();

            // n
            names.insert(names.end(), { BACKSLASH, N_KEY, K_KEY, END });
            possibStreak.b |= processLarge(ref, 0x1D45B, arr_size(ref));
            names.clear();

            // rightarrow
            names.insert(names.end(), { BACKSLASH, R_KEY, I_KEY, G_KEY, H_KEY, T_KEY, A_KEY, R_KEY, R_KEY, O_KEY, W_KEY, END });
            possibStreak.b |= processSmall(ref, 0x2192, arr_size(ref));
            names.clear();

            // gamma
            names.insert(names.end(), { BACKSLASH, G_KEY, A_KEY, M_KEY, M_KEY, A_KEY, END });
            possibStreak.b |= processLarge(ref, 0x1D6FE, arr_size(ref));
            names.clear();

            // check
            names.insert(names.end(), { BACKSLASH, C_KEY, H_KEY, E_KEY, C_KEY, K_KEY, END});
            possibStreak.b |= processSmall(ref, 0x2714, arr_size(ref));
            names.clear();

            // Capital theta
            names.insert(names.end(), { BACKSLASH, C_KEY, T_KEY, H_KEY, E_KEY, T_KEY, A_KEY, END });
            possibStreak.b |= processSmall(ref, 0x0398, arr_size(ref));
            names.clear();

            // Capital omega
            names.insert(names.end(), { BACKSLASH, C_KEY, O_KEY, M_KEY, E_KEY, G_KEY, A_KEY, END });
            possibStreak.b |= processSmall(ref, 0x03A9, arr_size(ref));
            names.clear();

            // lower omega
            names.insert(names.end(), { BACKSLASH, O_KEY, M_KEY, E_KEY, G_KEY, A_KEY, END });
            possibStreak.b |= processSmall(ref, 0x03C9, arr_size(ref));
            names.clear();

            // heart
            names.insert(names.end(), { BACKSLASH, H_KEY, E_KEY, A_KEY, R_KEY, T_KEY, END });
            possibStreak.b |= processSmall(ref, 0x2665, arr_size(ref));
            names.clear();

            // Emojis.
            USHORT moneysmile[] = { BACKSLASH, M_KEY, O_KEY, N_KEY, E_KEY, Y_KEY, S_KEY, M_KEY, I_KEY, L_KEY, E_KEY, S_KEY, END };
            names.insert(names.end(), { BACKSLASH, M_KEY, O_KEY, N_KEY, E_KEY, Y_KEY, S_KEY, M_KEY, I_KEY, L_KEY, E_KEY, S_KEY, END  });
            possibStreak.b |= processLarge(ref, 0x1F911, arr_size(ref));
            names.clear();

            USHORT nerdglasses[] = { BACKSLASH, N_KEY, E_KEY, R_KEY, D_KEY, END };
            names.insert(names.end(), { BACKSLASH, N_KEY, E_KEY, R_KEY, D_KEY, END });
            possibStreak.b |= processLarge(ref, 0x1F913, arr_size(ref));
            names.clear();

            names.insert(names.end(), { BACKSLASH, Q_KEY, U_KEY, E_KEY, S_KEY, T_KEY, I_KEY, O_KEY, N_KEY, M_KEY, A_KEY, N_KEY, END });
            possibStreak.b |= processLarge(ref, 0x1F9D0, arr_size(ref));
            names.clear();

            // Outdated array method
            USHORT partial[] = { BACKSLASH, P_KEY, A_KEY, R_KEY, T_KEY, I_KEY, A_KEY, L_KEY, END };
            possibStreak.b |= processSmall(partial, 0x2202, arr_size(partial));

            USHORT squared[] = { BACKSLASH, VK_SHIFT, SIX_KEY, TWO_KEY, END };
            possibStreak.b |= processSmall(squared, 0xB2, arr_size(squared));

            USHORT powerx[] = { BACKSLASH, VK_SHIFT, SIX_KEY, X_KEY, END };
            possibStreak.b |= processSmall(powerx, 0x1D61, arr_size(powerx));

            USHORT integral[] = { BACKSLASH, I_KEY, N_KEY, T_KEY, END };
            possibStreak.b |= processSmall(integral, 0x222B, arr_size(integral));

            USHORT laplace[] = { BACKSLASH, L_KEY, A_KEY, P_KEY, L_KEY, A_KEY, C_KEY, E_KEY, END };
            possibStreak.b |= processSmall(laplace, 0x2206, arr_size(laplace));

            USHORT succeq[] = { BACKSLASH, S_KEY, U_KEY, C_KEY, C_KEY, E_KEY, Q_KEY, END };
            possibStreak.b |= processSmall(succeq, 0x2AB0, arr_size(succeq));

            USHORT vartheta[] = { BACKSLASH, V_KEY, A_KEY, R_KEY, T_KEY, H_KEY, E_KEY, T_KEY, A_KEY, END };
            possibStreak.b |= processSmall(vartheta, 0x3D1, arr_size(vartheta));

            USHORT unionarray[] = { BACKSLASH, C_KEY, U_KEY, P_KEY, END };
            possibStreak.b |= processSmall(unionarray, 0x222A, arr_size(unionarray));

            USHORT intersect[] = { BACKSLASH, C_KEY, A_KEY, P_KEY, END };
            possibStreak.b |= processSmall(intersect, 0x2229, arr_size(intersect));

            USHORT leq[] = { BACKSLASH, L_KEY, E_KEY, Q_KEY, END };
            possibStreak.b |= processSmall(leq, 0x2264, arr_size(leq));

            USHORT mathbfx[] = { BACKSLASH, M_KEY, A_KEY, T_KEY, H_KEY, B_KEY, F_KEY, VK_SHIFT, LBRACE, X_KEY, VK_SHIFT, RBRACE, END };
            possibStreak.b |= processLarge(mathbfx, 0x1D431, arr_size(mathbfx));

            USHORT forall[] = { BACKSLASH, F_KEY, O_KEY, R_KEY, A_KEY, L_KEY, L_KEY, END };
            possibStreak.b |= processSmall(forall, 0x2200, arr_size(forall));

            USHORT exists[] = { BACKSLASH, E_KEY, X_KEY, I_KEY, S_KEY, T_KEY, S_KEY, END };
            possibStreak.b |= processSmall(exists, 0x2203, arr_size(exists));

            USHORT infty[] = { BACKSLASH, F_KEY, T_KEY, Y_KEY, END };
            possibStreak.b |= processSmall(infty, 0x221E, arr_size(infty));

            USHORT xeyes[] = { BACKSLASH, X_KEY, E_KEY, Y_KEY, E_KEY, S_KEY, END };
            possibStreak.b |= processLarge(xeyes, 0x1F635, arr_size(xeyes));

            USHORT xi[] = { BACKSLASH, X_KEY, I_KEY, END };
            possibStreak.b |= processSmall(xi, 0x3BE, arr_size(xi));

            USHORT mathbfy[] = { BACKSLASH, M_KEY, A_KEY, T_KEY, H_KEY, B_KEY, F_KEY, VK_SHIFT, LBRACE, Y_KEY, VK_SHIFT, RBRACE, END };
            possibStreak.b |= processLarge(mathbfy, 0x1D432, arr_size(mathbfy));

            USHORT in[] = { BACKSLASH, I_KEY, N_KEY, END };
            possibStreak.b |= processSmall(in, 0x2208, arr_size(in));

            USHORT transpose[] = { BACKSLASH, VK_SHIFT, SIX_KEY, BACKSLASH, T_KEY, O_KEY, P_KEY, END };
            possibStreak.b |= processSmall(transpose, 0x1D40, arr_size(transpose));

            USHORT Sigma[] = { BACKSLASH, VK_SHIFT, S_KEY, I_KEY, G_KEY, M_KEY, A_KEY, END };
            possibStreak.b |= processSmall(Sigma, 0x2211, arr_size(Sigma));

            USHORT subtwo[] = { BACKSLASH, VK_SHIFT, VK_OEM_MINUS, TWO_KEY, END };
            possibStreak.b |= processSmall(subtwo, 0x2082, arr_size(subtwo));

            USHORT subzero[] = { BACKSLASH, VK_SHIFT, VK_OEM_MINUS, ZERO_KEY, END };
            possibStreak.b |= processSmall(subzero, 0x2080, arr_size(subzero));

            USHORT powerplus[] = { BACKSLASH, VK_SHIFT, SIX_KEY, VK_SHIFT, VK_OEM_PLUS, END };
            possibStreak.b |= processSmall(powerplus, 0x207A, arr_size(powerplus));

            USHORT powerminus[] = { BACKSLASH, VK_SHIFT, SIX_KEY, VK_OEM_MINUS, END };
            possibStreak.b |= processSmall(powerminus, 0x207B, arr_size(powerminus));

            // clear vector
            // 0: clear, save nothing
            // 1: save everything
            // 2: save one backslash
            if ((possibStreak.b == 0) || (possibStreak.b == 2))
            {
                chars.clear();
            }
            if (possibStreak.b == 2)
            {
                chars.push_back(VK_OEM_5);
            }
        }
        break;
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int checkEqual(USHORT* word)
{
    int i = 0;
    while (word[i] != '\0')
    {
        if ((i < chars.size()) && (word[i] != chars[i]) && (chars[i] != VK_OEM_5))
        {  // not equal, no backslash to save
            return 0;
        }
        else if (i >= chars.size())
        {  // matched so far but not complete
            return 2;
        }
        else if ((i < chars.size()) && (word[i] != chars[i]) && (chars[i] == VK_OEM_5))  // save one backslash in chars
        {
            return 3;
        }
        i++;
    }
    return 1;  // equal
}

void makeSurrogates(WORD* surrogate, UINT input)
{
    input -= 0x10000;
    UINT maskUpper = 0x3FF;
    UINT maskLower = 0xFFC00;
    USHORT lowerTen = input & maskUpper;
    USHORT upperTen = (input & maskLower) >> 10;
    USHORT lowerSurrogate = lowerTen + 0xDC00;
    USHORT higherSurrogate = upperTen + 0xD800;
    *surrogate = (WORD)higherSurrogate;
    surrogate++;
    *surrogate = (WORD)lowerSurrogate;
}

void sendInput(USHORT input, int keySize)
{
    INPUT puts[1] = { };
    puts[0].type = INPUT_KEYBOARD;
    puts[0].ki.dwFlags = KEYEVENTF_UNICODE;
    puts[0].ki.wScan = input;
    INPUT backslash[1] = { };
    backslash[0].type = INPUT_KEYBOARD;
    backslash[0].ki.wVk = VK_BACK;

    for (int i = 0; i < keySize - 1; i++)
    {
        SendInput(1, backslash, sizeof(backslash));
        Sleep(30);
    }
    SendInput(1, backslash, sizeof(INPUT));
    SendInput(1, puts, sizeof(INPUT));
    chars.clear();
}

void sendInputLarge(WORD* surrogates, int keySize)
{
    INPUT puts[4] = { };

    // down
    puts[0].type = INPUT_KEYBOARD;
    puts[0].ki.dwFlags = KEYEVENTF_UNICODE;
    //puts[0].ki.wScan = 0xD835;
    puts[0].ki.wScan = surrogates[0];

    // down
    puts[1].type = INPUT_KEYBOARD;
    puts[1].ki.dwFlags = KEYEVENTF_UNICODE;
    // puts[1].ki.wScan = 0xDC95;
    puts[1].ki.wScan = surrogates[1];

    // up
    puts[2] = puts[0];
    puts[2].ki.dwFlags |= KEYEVENTF_KEYUP;

    // up
    puts[3] = puts[1];
    puts[3].ki.dwFlags |= KEYEVENTF_KEYUP;


    INPUT backslash[1] = { };
    backslash[0].type = INPUT_KEYBOARD;
    backslash[0].ki.wVk = VK_BACK;

    for (int i = 0; i < keySize - 1; i++)
    {
        SendInput(1, backslash, sizeof(INPUT));
        Sleep(30);
    }
    SendInput(1, backslash, sizeof(INPUT));
    SendInput(4, puts, sizeof(INPUT));
}

USHORT processSmall(USHORT arr[], USHORT code, USHORT keySize) 
{
    int result = checkEqual(arr);
    if (result == 1)
    {
        sendInput(code, keySize);  // return 0 to reset
    }
    else if (result == 2)
    {  // save everything, could be equal
        return 1;
    }
    else if (result == 3)
    {  // save one backslash
        return 2;
    }
    return 0;  // result == 0 or 1
}

USHORT processLarge(USHORT arr[], UINT code, USHORT keySize)
{
    WORD surrogates[PAIR];
    USHORT result = checkEqual((USHORT*)arr);
    if (result == 1)
    {
        makeSurrogates((WORD*)surrogates, (UINT)code);
        sendInputLarge((WORD*)surrogates, keySize);
    }
    else if (result == 2)
    {
        return 1;
    }
    else if (result == 3)
    {
        return 2;
    }
    return 0;
}

USHORT arr_size(USHORT arr[])
{
    USHORT count = 0;
    for (int i = 0; arr[i] != END; i++)
    {
        if (arr[i] != VK_SHIFT) { count++; }
    }
    return count;
}

// vector based

int checkEqual(std::vector<USHORT>& word)
{
    int i = 0;
    while (word[i] != '\0')
    {
        if ((i < chars.size()) && (word[i] != chars[i]) && (chars[i] != VK_OEM_5))
        {  // not equal, no backslash to save
            return 0;
        }
        else if (i >= chars.size())
        {  // matched so far but not complete
            return 2;
        }
        else if ((i < chars.size()) && (word[i] != chars[i]) && (chars[i] == VK_OEM_5))  // save one backslash in chars
        {
            return 3;
        }
        i++;
    }
    return 1; // equal
}

USHORT processSmall(std::vector<USHORT>& arr, USHORT code, USHORT keySize)
{
    int result = checkEqual(arr);
    if (result == 1)
    {
        sendInput(code, keySize);  // return 0 to reset
    }
    else if (result == 2)
    {  // save everything, could be equal
        return 1;
    }
    else if (result == 3)
    {  // save one backslash
        return 2;
    }
    return 0;  // result == 0 or 1
}

USHORT processLarge(std::vector<USHORT>& arr, UINT code, USHORT keySize)
{
    WORD surrogates[PAIR];
    USHORT result = checkEqual(arr);
    if (result == 1)
    {
        makeSurrogates((WORD*)surrogates, code);
        sendInputLarge((WORD*)surrogates, keySize);
    }
    else if (result == 2)
    {
        return 1;
    }
    else if (result == 3)
    {
        return 2;
    }
    return 0;
}

USHORT arr_size(std::vector<USHORT>& arr)
{
    USHORT count = 0;
    for (int i = 0; arr[i] != END; i++)
    {
        if (arr[i] != VK_SHIFT) { count++; }
    }
    return count;
}
