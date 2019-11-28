// TestWin.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "stdio.h"
#include "CommCtrl.h"
#include <string>
#include "QAInfor.h"

#define MAX_LOADSTRING 100
#define QA_INFO_ITEM_LENGTH 500
#define SEARCH_MAX_LENGTH 512

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWindow;
HWND hwndListView;
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
QAInfoItem infoList[QA_INFO_ITEM_LENGTH];

QAInfoItem showList[QA_INFO_ITEM_LENGTH];
int showLength = QA_INFO_ITEM_LENGTH;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Dialog(HWND, UINT, WPARAM, LPARAM);
void                getIntoQAInfoList();
void                reflushListView(HWND hwndListView);
int                 charPointLength(const char* orgString);
int                 indexOf(const char* orgString, const char* idxString);
void                searchListView(HWND hDlg, LPSTR keys);
void                searchAndKeys(LPSTR keyPoint[], int keyLength);
void                searchOrKeys(LPSTR keyPoint[], int keyLength);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTWIN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TESTWIN);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TESTWIN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TESTWIN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(
	   szWindowClass, 
	   szTitle, 
	   WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 
	   0, 
	   CW_USEDEFAULT, 
	   0, 
	   NULL, 
	   NULL, 
	   hInstance, 
	   NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_CREATE:
			hWindow = hWnd;
			DialogBox(hInst, (LPCTSTR)IDD_DIALOG1, hWnd, (DLGPROC)Dialog);
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				case IDM_SEARCH:
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG1, hWnd, (DLGPROC)Dialog);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	

	switch (message)
	{
		case WM_INITDIALOG:
			

				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

LRESULT CALLBACK Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD dwStyle = WS_TABSTOP | WS_CHILD | WS_BORDER | WS_VISIBLE 
		| LVS_AUTOARRANGE | LVS_REPORT;// | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	
	LV_COLUMN lvColumn;
	HWND edit = GetDlgItem(hDlg, IDC_EDIT1);
	LPSTR editText = (LPSTR)malloc(SEARCH_MAX_LENGTH);

	switch (message)
	{
		case WM_INITDIALOG:
			hwndListView = CreateWindowEx(
				WS_EX_CLIENTEDGE, WC_LISTVIEW, TEXT(""), dwStyle, 14, 48, 524, 300, hDlg, NULL, hInst, NULL);
			
			ListView_SetExtendedListViewStyle(hwndListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
			lvColumn.cx = 160;
			lvColumn.iSubItem = 0;
			lvColumn.pszText = TEXT("问题");
			ListView_InsertColumn(hwndListView, 0, &lvColumn);

			lvColumn.cx = 340;
			lvColumn.iSubItem = 1;
			lvColumn.pszText = TEXT("答案");
			ListView_InsertColumn(hwndListView, 1, &lvColumn);

			getIntoQAInfoList();
			reflushListView(hwndListView);

			return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				DestroyWindow(hWindow);
				return TRUE;
			}else
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDC_SEARCH_BTN){
				GetWindowText(edit, editText, 512);
			//	MessageBox(hDlg, editText, TEXT("提示"), MB_OK);
			//	if(indexOf(TEXT("人可以活到多少岁？"), editText) >= 0){
			//		MessageBox(hDlg, TEXT("匹配"), TEXT("提示"), MB_OK);
			//	}else{
			//		MessageBox(hDlg, TEXT("不匹配"), TEXT("提示"), MB_OK);
			//	}
				searchListView(hDlg, editText);
				reflushListView(hwndListView);
				//DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hDlg, (DLGPROC)About);
			}
			break;
	}
    return FALSE;
}

void searchListView(HWND hDlg, LPSTR keys){
	LPSTR keyParam = (LPSTR)malloc(SEARCH_MAX_LENGTH);
	LPSTR keyPoint[100];
	int keyLength = 0, ch = 0, count = 0, begin = 0, i = 0, j = 0;
	int isOr = 0;

	strcpy(keyParam, keys);
//	MessageBox(hDlg, keyParam, TEXT("提示"), MB_OK);

	begin = 0;
	i = 0;
	ch = *(unsigned char*)(keyParam);

	for(i=0; ch != 0; i++){
		ch = *(unsigned char*)(keyParam + i);
		if(ch == '|'){
			isOr = 1;
		}
		if(ch == ' ' || ch == '|'){
			if(count <= 0){
				continue;
			}
			*(keyParam + i) = 0;
			keyPoint[keyLength] = keyParam + begin;
			keyLength++;
			begin = i + 1;
		}else{
			count++;
		}
	}
	if(count > 0){
		keyPoint[keyLength] = keyParam + begin;
		keyLength++;
	}

//	for(i=0; i<keyLength; i++){
//		MessageBox(hDlg, keyPoint[i], TEXT("提示"), MB_OK);
//	}

	if(isOr > 0){
		searchOrKeys(keyPoint, keyLength);
	}else{
		searchAndKeys(keyPoint, keyLength);
	}
}

void searchOrKeys(LPSTR keyPoint[], int keyLength){
	int i = 0, j = 0;
	showLength = 0;

	for(i=0; i<QA_INFO_ITEM_LENGTH; i++){
		for(j=0; j<keyLength; j++){
			if(indexOf(infoList[i].question, keyPoint[j]) >= 0
				|| indexOf(infoList[i].answer, keyPoint[j]) >= 0){
				showList[showLength]= infoList[i];
				showLength++;
				break;
			}
		}
	}
}

void searchAndKeys(LPSTR keyPoint[], int keyLength){
	int i = 0, j = 0, isFound = 0;
	showLength = 0;

	for(i=0; i<QA_INFO_ITEM_LENGTH; i++){
		isFound = 1;
		for(j=0; j<keyLength; j++){
			if(indexOf(infoList[i].question, keyPoint[j]) < 0
				&& indexOf(infoList[i].answer, keyPoint[j]) < 0){
				isFound = -1;
				break;
			}
		}
		if(isFound > 0){
			showList[showLength]= infoList[i];
			showLength++;
		}
	}
}

int charPointLength(const char* orgString){
	int idx = 0, charOrg = 0;
	for(charOrg = *(unsigned char*)orgString; charOrg != 0; idx++){
		charOrg = *(unsigned char*)(orgString + idx + 1);
	}
	return idx;
}

int indexOf(const char* orgString, const char* idxString){
	int i = 0, j = 0, ii = 0, jj = 0;
	int chA = 0, chB = 0;
	int chA1 = 0, chB1 = 0;
	int beginIdx = -1;

	int orgLeng = charPointLength(orgString);
	int idxLeng = charPointLength(idxString);
	
	if(orgLeng <= 0 || idxLeng <= 0){
		return -1;
	}
	if(idxLeng > orgLeng){
		return -1;
	}else
	if(idxLeng == orgLeng){
		if(strcmp(orgString, idxString) > 0){
			return 0;
		}else{
			return -1;
		}
	}

	for(i=0; i<orgLeng; i++){
		chA = *(unsigned char*)(orgString + i);
		for(j=0; j<idxLeng; j++){
			chB = *(unsigned char*)(idxString + j);
			if(chA != chB){
				beginIdx = -1;
				break;
			}
			beginIdx = i;
			ii = i+1;
			jj = j+1;
			for(;ii<orgLeng && jj<idxLeng; ii++, jj++){
				chA1 = *(unsigned char*)(orgString + ii);
				chB1 = *(unsigned char*)(idxString + jj);
				if(chA1 != chB1){
					beginIdx = -1;
					break;
				}
			}
			if(beginIdx >= 0){
				if(ii >= orgLeng && jj >= idxLeng){
					return beginIdx;
				}
				if(ii < orgLeng && jj >= idxLeng){
					return beginIdx;
				}
				beginIdx = -1;
			}else{
				break;
			}
		}
	}
	return -1;
}

void reflushListView(HWND hwndListView){
	LV_ITEM lvItem;
	int idx = 0;
	ListView_DeleteAllItems(hwndListView);

	lvItem.mask = LVIF_TEXT;
			
	for(idx=0; idx<showLength; idx++){
		lvItem.iItem = idx;
		lvItem.iSubItem = 0;
		lvItem.pszText = showList[idx].question;
		ListView_InsertItem(hwndListView, &lvItem);

		lvItem.iSubItem = 1;
		lvItem.pszText = showList[idx].answer;
		ListView_SetItem(hwndListView, &lvItem);
	}
}

void getIntoQAInfoList(){
	int i = 0;
	char ans[] = "100岁。";
	char que[] = "000人可以活到多少岁？";

	for(;i<QA_INFO_ITEM_LENGTH; i++){
		infoList[i].question = (LPSTR)malloc(30);
		infoList[i].answer = (LPSTR)malloc(10);
		showList[i] = infoList[i];

		strcpy(showList[i].question, que);
		strcpy(showList[i].answer, ans);

		*(showList[i].question + 0) = ('0' + (((999 - i) / 100) % 10));
		*(showList[i].question + 1) = ('0' + (((999 - i) / 10) % 10));
		*(showList[i].question + 2) = ('0' + ((999 - i) % 10));

		*(showList[i].answer + 0) = ('0' + ((i / 100) % 10));
		*(showList[i].answer + 1) = ('0' + ((i / 10) % 10));
		*(showList[i].answer + 2) = ('0' + (i % 10));
	}
}
