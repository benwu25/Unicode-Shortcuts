// Unicode-Shortcuts.cpp : Defines the entry point for the application.
//

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

// array-based functions (soon delete)
int checkEqual(USHORT*); // uses user-given array
void sendInput(USHORT, int);
void makeSurrogates(WORD*, UINT);
void sendInputLarge(WORD*, int);
USHORT processLarge(USHORT[], UINT, USHORT); // uses user-given array
USHORT processSmall(USHORT[], USHORT, USHORT); // uses user-given array
USHORT arr_size(USHORT[]); // uses user-given array
// 

// vector-based function s
int checkEqual(std::vector<USHORT>&);
USHORT processLarge(std::vector<USHORT>&, UINT, USHORT);
USHORT processSmall(std::vector<USHORT>&, USHORT, USHORT);
USHORT arr_size(std::vector<USHORT>&);

std::vector<USHORT> chars;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// APIENTRY vs WINAPI? // PWSTR vs. PSTR?
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

    RAWINPUTDEVICE Rid[1];

    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x06;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hwnd;

    RegisterRawInputDevices(Rid, 1, sizeof(RAWINPUTDEVICE)); // sizeof(Rid[0])

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
    case WM_INPUT: // add the raw data. Check if you have one of the constants. Send input if so.
    {
        UINT dwSize;
        GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        LPBYTE lpb = new BYTE[dwSize];
        GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
        RAWINPUT* raw = (RAWINPUT*)(lpb);

        USHORT x = raw->data.keyboard.VKey;
        UINT y = raw->data.keyboard.Message;

        if (y == WM_KEYDOWN) {


            // only add with backslash identifier in the vector
            if ((x == VK_OEM_5 || (!chars.empty() && chars[0] == VK_OEM_5))) {
                chars.push_back(x);
            }

            //USHORT possibStreak = 0;
            Bit possibStreak = { 0 };
            USHORT result;
            std::vector<USHORT> names;
            std::vector<USHORT>& ref = names;

            //-> \nabla
            names.insert(names.end(), { BACKSLASH, N_KEY, A_KEY, B_KEY, L_KEY, A_KEY, END });
            //USHORT nabla[] = { BACKSLASH, N_KEY, A_KEY, B_KEY, L_KEY, A_KEY, END };
            //possibStreak.b |= processSmall(nabla, 0x2207, arr_size(nabla));
            possibStreak.b |= processSmall(ref, 0x2207, arr_size(ref));
            names.clear();
            /*result = checkEqual(nabla);
            if (result == 1) {
                sendInput(0x2207, 6);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            //-> \lineint
            names.insert(names.end(), { BACKSLASH, L_KEY, I_KEY, N_KEY, E_KEY, I_KEY, N_KEY, T_KEY, END });
            possibStreak.b |= processSmall(ref, 0x222E, arr_size(ref));
            names.clear();
            //USHORT lineint[] = { BACKSLASH, L_KEY, I_KEY, N_KEY, E_KEY, I_KEY, N_KEY, T_KEY, END };
            //possibStreak.b |= processSmall(lineint, 0x222E, arr_size(lineint));
            /*result = checkEqual((USHORT*)lineint);
            if (result == 1) {
                sendInput(0x222E, 8);

            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            //-> \surfint
            names.insert(names.end(), { BACKSLASH, S_KEY, U_KEY, R_KEY, F_KEY, I_KEY, N_KEY, T_KEY, END });
            possibStreak.b |= processSmall(ref, 0x222F, arr_size(ref));
            names.clear();
            /*result = checkEqual(surfint);
            if (result == 1) {
                //sendInputLarge((WORD *)surrogates1, 8);
                sendInput(0x222F, 8);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            //-> \mathbf{t}
            // VK_SHIFT, VK_OEM_4, VK_OEM_6
            names.insert(names.end(), { BACKSLASH, M_KEY, A_KEY, T_KEY, H_KEY, B_KEY, F_KEY, VK_SHIFT, LBRACE, T_KEY, VK_SHIFT, RBRACE, END });
            possibStreak.b |= processLarge(ref, 0x1D42D, arr_size(ref));
            names.clear();
            /*WORD surrogates1[2];
            makeSurrogates((WORD*)surrogates1, (UINT)0x1D42D);
            result = checkEqual((USHORT*)mathbft);
            if (result == 1) {
                sendInputLarge((WORD*)surrogates1, 10);
                //sendInput(0x2122, 12);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            names.insert(names.end(), { BACKSLASH, M_KEY, A_KEY, T_KEY, H_KEY, C_KEY, B_KEY, F_KEY, VK_SHIFT, LBRACE, X_KEY, VK_SHIFT, RBRACE, END });
            possibStreak.b |= processLarge(ref, 0x1D499, arr_size(ref));
            names.clear();
            //WORD surrogates2[2];
            /*makeSurrogates((WORD*)surrogates1, (UINT)0x1D499);
            result = checkEqual((USHORT*)mathcbfx);
            if (result == 1) {
                sendInputLarge((WORD*)surrogates1, 11);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/


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

            // heart
            names.insert(names.end(), { BACKSLASH, H_KEY, E_KEY, A_KEY, R_KEY, T_KEY, END });
            possibStreak.b |= processLarge(ref, 0x1F493, arr_size(ref));
            names.clear();



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
            // USHORT questionman[] = { BACKSLASH, Q_KEY, U_KEY, E_KEY, S_KEY, T_KEY, I_KEY, O_KEY, N_KEY, M_KEY, A_KEY, N_KEY, END };
            // possibStreak.b |= processLarge(questionman, 0x1F9D0, arr_size(questionman));
            /*WORD surrogates5[2];
            makeSurrogates((WORD*)surrogates5, (UINT)0x1F9D0);
            result = checkEqual((USHORT*)questionman);
            if (result == 1) {
                sendInputLarge((WORD*)surrogates5, 12);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT partial[] = { BACKSLASH, P_KEY, A_KEY, R_KEY, T_KEY, I_KEY, A_KEY, L_KEY, END };
            possibStreak.b |= processSmall(partial, 0x2202, arr_size(partial));
            /*result = checkEqual((USHORT*)partial);
            if (result == 1) {
                sendInput(0x2202, 8);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT squared[] = { BACKSLASH, VK_SHIFT, SIX_KEY, TWO_KEY, END };
            possibStreak.b |= processSmall(squared, 0xB2, arr_size(squared));
            /*result = checkEqual((USHORT*)squared);
            if (result == 1) {
                sendInput(0x00B2, 3);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT powerx[] = { BACKSLASH, VK_SHIFT, SIX_KEY, X_KEY, END };
            possibStreak.b |= processSmall(powerx, 0x1D61, arr_size(powerx));
            /*result = checkEqual((USHORT*)powerx);
            if (result == 1) {
                sendInput(0x1D61, 3);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT integral[] = { BACKSLASH, I_KEY, N_KEY, T_KEY, END };
            possibStreak.b |= processSmall(integral, 0x222B, arr_size(integral));
            /*result = checkEqual((USHORT*)integral);
            if (result == 1) {
                sendInput(0x222B, 4);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT laplace[] = { BACKSLASH, L_KEY, A_KEY, P_KEY, L_KEY, A_KEY, C_KEY, E_KEY, END };
            possibStreak.b |= processSmall(laplace, 0x2206, arr_size(laplace));
            /*result = checkEqual((USHORT*)laplace);
            if (result == 1) {
                sendInput(0x2206, 8);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT succeq[] = { BACKSLASH, S_KEY, U_KEY, C_KEY, C_KEY, E_KEY, Q_KEY, END };
            possibStreak.b |= processSmall(succeq, 0x2AB0, arr_size(succeq));
            /*result = checkEqual((USHORT*)succeq);
            if (result == 1) {
                sendInput(0x2AB0, 7);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT vartheta[] = { BACKSLASH, V_KEY, A_KEY, R_KEY, T_KEY, H_KEY, E_KEY, T_KEY, A_KEY, END };
            possibStreak.b |= processSmall(vartheta, 0x3D1, arr_size(vartheta));
            /*result = checkEqual((USHORT*)vartheta);
            if (result == 1) {
                sendInput(0x03D1, 9);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT unionarray[] = { BACKSLASH, C_KEY, U_KEY, P_KEY, END };
            possibStreak.b |= processSmall(unionarray, 0x222A, arr_size(unionarray));
            /*result = checkEqual((USHORT*)unionarray);
            if (result == 1) {
                sendInput(0x222A, 6);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT intersect[] = { BACKSLASH, C_KEY, A_KEY, P_KEY, END };
            possibStreak.b |= processSmall(intersect, 0x2229, arr_size(intersect));
            /*result = checkEqual((USHORT*)intersect);
            if (result == 1) {
                sendInput(0x2229, 10);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT leq[] = { BACKSLASH, L_KEY, E_KEY, Q_KEY, END };
            possibStreak.b |= processSmall(leq, 0x2264, arr_size(leq));
            /*result = checkEqual((USHORT*)leq);
            if (result == 1) {
                sendInput(0x2264, 4);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

            USHORT mathbfx[] = { BACKSLASH, M_KEY, A_KEY, T_KEY, H_KEY, B_KEY, F_KEY, VK_SHIFT, LBRACE, X_KEY, VK_SHIFT, RBRACE, END };
            possibStreak.b |= processLarge(mathbfx, 0x1D431, arr_size(mathbfx));
            /*WORD surrogates6[2];
            result = checkEqual((USHORT*)mathbfx);
            if (result == 1) {
                makeSurrogates((WORD*)surrogates6, (UINT)0x1D431);
                sendInputLarge((WORD*)surrogates6, 10);
            }
            else if (result == 2) {
                possibStreak = 1;
            }*/

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

            // find a way to save space by converting from many arrays to one vector, have to modify types and clear vector between

            // clear vector
            // 0: clear, save nothing
            // 1: save everything
            // 2: save one backslash
            if ((possibStreak.b == 0) || (possibStreak.b == 2)) {
                chars.clear();
            }
            if (possibStreak.b == 2) {
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
    while (word[i] != '\0') {
        if ((i < chars.size()) && (word[i] != chars[i]) && (chars[i] != VK_OEM_5)) { // not equal, no backslash to save
            return 0;
        }
        else if (i >= chars.size()) { // matched so far but not complete
            return 2;
        }
        else if ((i < chars.size()) && (word[i] != chars[i]) && (chars[i] == VK_OEM_5)) // save one backslash in chars
        {
            return 3;
        }
        i++;
    }
    return 1; // equal
}

void makeSurrogates(WORD* surrogate, UINT input) {
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

void sendInput(USHORT input, int keySize) {
    INPUT puts[1] = { };
    puts[0].type = INPUT_KEYBOARD;
    puts[0].ki.dwFlags = KEYEVENTF_UNICODE;
    puts[0].ki.wScan = input;
    INPUT backslash[1] = { };
    backslash[0].type = INPUT_KEYBOARD;
    backslash[0].ki.wVk = VK_BACK;

    for (int i = 0; i < keySize - 1; i++) {
        SendInput(1, backslash, sizeof(backslash));
        Sleep(30);
    }
    SendInput(1, backslash, sizeof(INPUT));
    SendInput(1, puts, sizeof(INPUT));
    chars.clear();
}

void sendInputLarge(WORD* surrogates, int keySize) {
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

    for (int i = 0; i < keySize - 1; i++) {
        SendInput(1, backslash, sizeof(INPUT));
        Sleep(30);
    }
    SendInput(1, backslash, sizeof(INPUT));
    SendInput(4, puts, sizeof(INPUT));
}

// call by possibStreak = processSmall(arr, code, keySize);
USHORT processSmall(USHORT arr[], USHORT code, USHORT keySize) {
    int result = checkEqual(arr);
    if (result == 1) {
        sendInput(code, keySize); // return 0 to reset
    }
    else if (result == 2) { // save everything, could be equal
        return 1;
    }
    else if (result == 3) { // save one backslash
        return 2;
    }
    return 0; // result == 0 or 1
}

// possibStreak = processLarge(arr, code, keySize);
USHORT processLarge(USHORT arr[], UINT code, USHORT keySize) {
    WORD surrogates[PAIR];
    USHORT result = checkEqual((USHORT*)arr);
    if (result == 1) {
        makeSurrogates((WORD*)surrogates, (UINT)code);
        sendInputLarge((WORD*)surrogates, keySize);
    }
    else if (result == 2) {
        return 1;
    }
    else if (result == 3) {
        return 2;
    }
    return 0;
}

USHORT arr_size(USHORT arr[])
{
    USHORT count = 0;
    for (int i = 0; arr[i] != END; i++) {
        if (arr[i] != VK_SHIFT) { count++; }
    }
    return count;
}

// vector based

int checkEqual(std::vector<USHORT>& word)
{
    int i = 0;
    while (word[i] != '\0') {
        if ((i < chars.size()) && (word[i] != chars[i]) && (chars[i] != VK_OEM_5)) { // not equal, no backslash to save
            return 0;
        }
        else if (i >= chars.size()) { // matched so far but not complete
            return 2;
        }
        else if ((i < chars.size()) && (word[i] != chars[i]) && (chars[i] == VK_OEM_5)) // save one backslash in chars
        {
            return 3;
        }
        i++;
    }
    return 1; // equal
}

USHORT processSmall(std::vector<USHORT>& arr, USHORT code, USHORT keySize) {
    int result = checkEqual(arr);
    if (result == 1) {
        sendInput(code, keySize); // return 0 to reset
    }
    else if (result == 2) { // save everything, could be equal
        return 1;
    }
    else if (result == 3) { // save one backslash
        return 2;
    }
    return 0; // result == 0 or 1
}

USHORT processLarge(std::vector<USHORT>& arr, UINT code, USHORT keySize) {
    WORD surrogates[PAIR];
    USHORT result = checkEqual(arr);
    if (result == 1) {
        makeSurrogates((WORD*)surrogates, code);
        sendInputLarge((WORD*)surrogates, keySize);
    }
    else if (result == 2) {
        return 1;
    }
    else if (result == 3) {
        return 2;
    }
    return 0;
}

USHORT arr_size(std::vector<USHORT>& arr)
{
    USHORT count = 0;
    for (int i = 0; arr[i] != END; i++) {
        if (arr[i] != VK_SHIFT) { count++; }
    }
    return count;
}


//////////////////////// Old notes.

/*int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }




    return MessageBox(NULL, L"hello, world", L"caption", 0);
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_KEYDOWN:
        DestroyWindow(hwnd);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}*/

// MessageBox(NULL, L"Hello world", L"caption", 0);

            // send "Alt" + "+" + "2207", how to hold Alt down for the duration?
            // INPUT puts[1] = { };
            // puts[0].type = INPUT_KEYBOARD;
            // puts[0].ki.wVk = 0x42;

            // hold Alt
            /*INPUT put1[1] = {};
            put1[0].type = INPUT_KEYBOARD;
            put1[0].ki.wVk = VK_MENU;
            put1[0].ki.dwFlags = 0;
            SendInput(1, put1, sizeof(put1));

            // press the others in sequence
            INPUT put0[1] = { };
            put0[0].type = INPUT_KEYBOARD;
            put0[0].ki.wVk = VK_ADD;
            SendInput(1, put0, sizeof(put0));
            INPUT put2[1] = { };
            put2[0].type = INPUT_KEYBOARD;
            put2[0].ki.wVk = 0x32;
            SendInput(1, put2, sizeof(put2));
            INPUT put3[1] = { };
            put3[0].type = INPUT_KEYBOARD;
            put3[0].ki.wVk = 0x32;
            SendInput(1, put3, sizeof(put3));
            INPUT put4[1] = { };
            put4[0].type = INPUT_KEYBOARD;
            put4[0].ki.wVk = 0x30;
            SendInput(1, put4, sizeof(put4));
            INPUT put5[1] = { };
            put5[0].type = INPUT_KEYBOARD;
            put5[0].ki.wVk = 0x37;
            SendInput(1, put5, sizeof(put5));

            // lift Alt key
            put1[0].ki.dwFlags = KEYEVENTF_KEYU+bP;
            SendInput(1, put1, sizeof(put1));*/

            /*INPUT puts[1] = {};
            puts[0].type = INPUT_KEYBOARD;
            puts[0].ki.wVk = VK_MENU;
            //puts[0].ki.dwFlags = 0;
            SendInput(1, puts, sizeof(puts));

            puts[0].ki.wVk = VK_ADD;
            SendInput(1, puts, sizeof(puts));

            puts[0].ki.wVk = 0x62;
            SendInput(1, puts, sizeof(puts));

            SendInput(1, puts, sizeof(puts));

            puts[0].ki.wVk = 0x60;
            SendInput(1, puts, sizeof(puts));

            puts[0].ki.wVk = 0x67;
            SendInput(1, puts, sizeof(puts));




            // lift Alt
            puts[0].ki.wVk = VK_MENU;
            puts[0].ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, puts, sizeof(puts));*/

            /*if (wParam == 0x41) {
                        INPUT puts[1] = { };
                        puts[0].type = INPUT_KEYBOARD;
                        puts[0].ki.wVk = 0x42;

                        HWND notepad = FindWindow(L"Notepad", NULL);
                        if (notepad == NULL) {
                            return 0;
                        }
                        HWND edit = FindWindowEx(HWND_BROADCAST, NULL, L"Edit", NULL);
                        if (edit == NULL) {
                            return 0;
                        }
                        // UINT uSent = SendInput(ARRAYSIZE(puts), puts, sizeof(puts));
                        SendMessage(edit, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)(L"hello"));

                        //MessageBox(NULL, L"Hello world", L"caption", 0);
                    }
                    else*/
                    // USHORT x = wParam;
                    // if (x == 0x42) {
                       //  MessageBox(NULL, L"Hello world", L"caption", 0);
                    // }
// case WM_LBUTTONDOWN:
    // {
        //MessageBox(NULL, L"Hello world", L"caption", 0);
    //         INPUT puts[1] = { };
    //        puts[0].type = INPUT_KEYBOARD;
    //        puts[0].ki.wVk = 0x41;
            // UINT didntworked = SendInput(ARRAYSIZE(puts), puts, sizeof(puts));
            // if (didntworked == 1) {
               //  MessageBox(NULL, L"Hello world", L"caption", 0);
            // }
    // }
    // if (GetKeyState(VK_UP) & 0x800) {
    //     MessageBox(NULL, L"Hello world", L"caption", 0);
    // }
// YES! Sent NABLA U+2207 to the FOREGROUND in NOTEPAD! And U+222F.
            // No need for fancy holding of alt and whatnot to do it.
            /*INPUT puts[1] = {};
            puts[0].type = INPUT_KEYBOARD;
            puts[0].ki.dwFlags = KEYEVENTF_UNICODE;
            puts[0].ki.wScan = 0x222F;
            SendInput(ARRAYSIZE(puts), puts, sizeof(puts));
            */
            /*INPUT puts[1] = {};
                        puts[0].type = INPUT_KEYBOARD;
                        puts[0].ki.dwFlags = KEYEVENTF_UNICODE;
                        puts[0].ki.wScan = 0x2207;
                        SendInput(ARRAYSIZE(puts), puts, sizeof(puts));
                        */
                        /*if ((x == 0x4E || stuff[1] == 'n') && y == WM_KEYDOWN) {
                                            if (stuff[1] == 'n') {
                                                // send nabla (just \n for now. \nabla for later).
                                                INPUT puts[1] = { };
                                                puts[0].type = INPUT_KEYBOARD;
                                                puts[0].ki.dwFlags = KEYEVENTF_UNICODE;
                                                puts[0].ki.wScan = 0x2207;
                                                // don't forget to backslash here, get rid of \n.
                                                INPUT backslash[1] = { };
                                                backslash[0].type = INPUT_KEYBOARD;
                                                backslash[0].ki.wVk = VK_BACK;
                                                SendInput(1, backslash, sizeof(backslash));
                                                SendInput(1, backslash, sizeof(backslash));
                                                SendInput(1, puts, sizeof(puts));
                                            }
                                            else { // x == 0x43.
                                                // record, keep going.
                                                stuff[1] = 'n';
                                                MSG msg;
                                                if (GetMessage(&msg, NULL, 0, 0) > 0) {
                                                    TranslateMessage(&msg);
                                                    DispatchMessage(&msg);
                                                }
                                            }*/
                                            // }
// Backslash
        /*if ((x == VK_OEM_5 || stuff[0] == '\\') && y == WM_KEYDOWN) {

            // already had a backslash, check next array item vs message
            if (stuff[0] == '\\') {

                if (x == 0x4E && y == WM_KEYDOWN) {
                    INPUT puts[1] = { };
                    puts[0].type = INPUT_KEYBOARD;
                    puts[0].ki.dwFlags = KEYEVENTF_UNICODE;
                    puts[0].ki.wScan = 0x2207;
                    INPUT backslash[1] = { };
                    backslash[0].type = INPUT_KEYBOARD;
                    backslash[0].ki.wVk = VK_BACK;

                    SendInput(1, backslash, sizeof(backslash));
                    Sleep(50);
                    SendInput(1, backslash, sizeof(backslash));
                    SendInput(1, puts, sizeof(puts));
                    stuff[0] = '\0';
                }
            }
            // x == VK_OEM_5:
            else {
                stuff[0] = '\\';
                MSG msg;
                if (GetMessage(&msg, NULL, 0, 0) > 0) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }




        }*/
        // method:
                        /*INPUT puts[1] = {};
                        puts[0].type = INPUT_KEYBOARD;
                        puts[0].ki.dwFlags = KEYEVENTF_UNICODE;
                        puts[0].ki.wScan = 0x222E; // one param
                        INPUT backslash[1] = { };
                        backslash[0].type = INPUT_KEYBOARD;
                        backslash[0].ki.wVk = VK_BACK;

                        for (int i = 0; i < 7; i++) {
                            SendInput(1, backslash, sizeof(backslash));
                            Sleep(10);
                        }
                        SendInput(1, backslash, sizeof(backslash));
                        SendInput(1, puts, sizeof(puts));
                        chars.clear();*/
                        // end method

//#define MAX_LOADSTRING 100
//
//// Global Variables:
//HINSTANCE hInst;                                // current instance
//WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
//WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//
//// Forward declarations of functions included in this code module:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: Place code here.
//
//    // Initialize global strings
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_UNICODESHORTCUTS, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // Perform application initialization:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UNICODESHORTCUTS));
//
//    MSG msg;
//
//    // Main message loop:
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//    return (int) msg.wParam;
//}
//
//
//
////
////  FUNCTION: MyRegisterClass()
////
////  PURPOSE: Registers the window class.
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEXW wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//
//    wcex.style          = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc    = WndProc;
//    wcex.cbClsExtra     = 0;
//    wcex.cbWndExtra     = 0;
//    wcex.hInstance      = hInstance;
//    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UNICODESHORTCUTS));
//    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
//    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_UNICODESHORTCUTS);
//    wcex.lpszClassName  = szWindowClass;
//    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassExW(&wcex);
//}
//
////
////   FUNCTION: InitInstance(HINSTANCE, int)
////
////   PURPOSE: Saves instance handle and creates main window
////
////   COMMENTS:
////
////        In this function, we save the instance handle in a global variable and
////        create and display the main program window.
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   hInst = hInstance; // Store instance handle in our global variable
//
//   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  PURPOSE: Processes messages for the main window.
////
////  WM_COMMAND  - process the application menu
////  WM_PAINT    - Paint the main window
////  WM_DESTROY  - post a quit message and return
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_COMMAND:
//        {
//            int wmId = LOWORD(wParam);
//            // Parse the menu selections:
//            switch (wmId)
//            {
//            case IDM_ABOUT:
//                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//                break;
//            case IDM_EXIT:
//                DestroyWindow(hWnd);
//                break;
//            default:
//                return DefWindowProc(hWnd, message, wParam, lParam);
//            }
//        }
//        break;
//    case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hWnd, &ps);
//            // TODO: Add any drawing code that uses hdc here...
//            EndPaint(hWnd, &ps);
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//// Message handler for about box.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
