// ez-wmr.cpp : Defines the entry point for the application.
// Author: Burnt-Delta

#include "framework.h"
#include "ez-wmr.h"
#include "ToggleFunctions.cpp"

#define MAX_LOADSTRING 100

// global variables
//--------------------------------------------------------
HINSTANCE hInst;                                               // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"ezWMR by Burnt-Delta";       // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = L"ezWMR by Burnt-Delta"; // the main window class name
wchar_t wfileloc[200] = L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\MixedRealityVRDriver\\resources\\settings\\default.vrsettings";
HWND text, status;                                             // displays current filepath and file status 

string fileloc = FILELOC_DEFAULT; // filepath of default.vrsettings
int wchar_size = 0;				  // used in conversion from string to wchar
short int tf[] = { -1, 0, 0, 0 }; // flags to indicate current file condition
//--------------------------------------------------------

// forward function declarations
//--------------------------------------------------------
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                browse(HWND);
void				updateStatus(HWND);
void				updatePath(HWND);
//--------------------------------------------------------

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EZWMR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EZWMR));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EZWMR));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EZWMR);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		CW_USEDEFAULT, 0, 500, 265, nullptr, nullptr, hInstance, nullptr);

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
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parses the menu selections
		switch (wmId)
		{
			// Attempts toggle
		case BUTTON_TOGGLE:
		{
			int seek[] = { 0, 0, 0 };
			check(fileloc, tf, seek);

			// Handles result; TODO: switch to switch case for consistency
			if (tf[0] == 0)
			{
				bool check[] = { false, false, false };

				if (IsDlgButtonChecked(hWnd, BUTTON_CHKBOX1) == BST_CHECKED)
					check[0] = true;
				if (IsDlgButtonChecked(hWnd, BUTTON_CHKBOX2) == BST_CHECKED)
					check[1] = true;
				if (IsDlgButtonChecked(hWnd, BUTTON_CHKBOX3) == BST_CHECKED)
					check[2] = true;

				getAnswer(fileloc, check, seek);
				setCheckbox(check);
				updateStatus(hWnd);
			}
			else if (tf[0] == 2)
			{
				MessageBox(hWnd,
					L"There was an error reading the file. Please check its contents and try again.",
					NULL,
					MB_OK);
			}
			else if (tf[0] == 1)
			{
				MessageBox(hWnd,
					L"File could not be opened. Please check filepath and try again.",
					NULL,
					MB_OK);
			}
			else // should never reach this case
			{
				MessageBox(hWnd,
					L"An unexpected error has occured. Please check filepath and try again.",
					NULL,
					MB_OK);
			}
		}
		break;
		case BUTTON_BROWSE:
			browse(hWnd);
			break;
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
	case WM_CREATE:
	{
		CreateWindowEx(0, L"BUTTON", L"Toggle", WS_CHILD | WS_VISIBLE, 20, 140, 114, 50, hWnd, (HMENU)BUTTON_TOGGLE, GetModuleHandle(NULL), NULL);
		CreateWindowEx(0, L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE, 345, 140, 114, 50, hWnd, (HMENU)BUTTON_BROWSE, GetModuleHandle(NULL), NULL);

		CreateWindowEx(0, L"BUTTON", L"Enable Turn", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT, 20, 85, 100, 20, hWnd, (HMENU)BUTTON_CHKBOX1, GetModuleHandle(NULL), NULL);
		CreateWindowEx(0, L"BUTTON", L"Enable Move", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT, 195, 85, 105, 20, hWnd, (HMENU)BUTTON_CHKBOX2, GetModuleHandle(NULL), NULL);
		CreateWindowEx(0, L"BUTTON", L"Smooth Turn", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT, 360, 85, 100, 20, hWnd, (HMENU)BUTTON_CHKBOX3, GetModuleHandle(NULL), NULL);

		getFileloc(fileloc, tf); // initializes filepath if config.txt exists

		if (tf[1] == 2)
			CheckDlgButton(hWnd, BUTTON_CHKBOX1, BST_CHECKED);

		if (tf[2] == 2)
			CheckDlgButton(hWnd, BUTTON_CHKBOX2, BST_CHECKED);

		if (tf[3] == 2)
			CheckDlgButton(hWnd, BUTTON_CHKBOX3, BST_CHECKED);

		// initialization of fileloc text
		updatePath(hWnd);

		// initialization of status text
		updateStatus(hWnd);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
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

//
// FUNCTION: browse()
//
// PURPOSE: Creates dialog box to choose fileloc.
//
void browse(HWND hWnd)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = wfileloc;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 200;
	ofn.lpstrFilter = L"vrsettings Files\0*.vrsettings\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn))
	{
		// transfers filepath from local struct to global variable
		wstring ws(wfileloc);
		string stringtemp(ws.begin(), ws.end());
		fileloc = stringtemp;

		// updates filepath text
		updatePath(hWnd);

		// update status text
		updateStatus(hWnd);

		int mb = MessageBox(hWnd,
			L"Set to default filepath?",
			L"Browse",
			MB_YESNO);

		if (mb == IDYES)
			setFileloc(fileloc); // sets to default
	}
}

void updateStatus(HWND hWnd)
{
	int seek[] = { 0, 0, 0 };
	check(fileloc, tf, seek);

	if (tf[0] != 0)
		status = CreateWindowW(L"static", L"Thumbstick Controls: N/A", WS_VISIBLE | WS_CHILD, 20, 112, 440, 20, hWnd, NULL, NULL, NULL);
	else
	{
		string stat = "Thumbstick Turn: ";
		if (tf[1] == 1)
			stat += "OFF;  ";
		else
			stat += "ON;  ";

		stat += "Thumbstick Move: ";
		if (tf[2] == 1)
			stat += "OFF;  ";
		else
			stat += "ON;  ";

		stat += "Smooth Turn: ";
		if (tf[3] == 1)
			stat += "OFF;";
		else
			stat += "ON;";

		wchar_size = MultiByteToWideChar(CP_UTF8, 0, stat.c_str(), -1, NULL, 0);
		MultiByteToWideChar(CP_UTF8, 0, stat.c_str(), -1, wfileloc, wchar_size);
		status = CreateWindowW(L"static", wfileloc, WS_VISIBLE | WS_CHILD, 20, 112, 440, 20, hWnd, NULL, NULL, NULL);
	}
}

void updatePath(HWND hWnd)
{
	string greet = "Current filepath:\n";

	greet += fileloc;
	wchar_size = MultiByteToWideChar(CP_UTF8, 0, greet.c_str(), -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, greet.c_str(), -1, wfileloc, wchar_size);
	text = CreateWindow(L"static", wfileloc, WS_CHILD | WS_VISIBLE | SS_EDITCONTROL, 20, 10, 440, 70, hWnd, NULL, NULL, NULL);
}
