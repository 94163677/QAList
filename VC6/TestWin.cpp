// TestWin.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "stdio.h"
#include "CommCtrl.h"
#include <string>
#include "QAInfor.h"

#define MAX_LOADSTRING 100
#define QA_INFO_ITEM_LENGTH 500 //问答最大数
#define SEARCH_MAX_LENGTH 512 //搜索栏最大可输入字节数

LPSTR VERSION = TEXT("V1.0.0");//程序版本号
LPSTR COPYRIGHT_TIME = TEXT("2019 - 2022");//程序版权时间
LPSTR COPYRIGHT_BY = TEXT("Hh7");//程序版权所属


// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWindow;//窗口的句柄
HWND hwndListView;//列表的句柄

TCHAR szTitle[MAX_LOADSTRING];//标题
TCHAR szWindowClass[MAX_LOADSTRING];// The title bar text

QAInfoItem infoList[QA_INFO_ITEM_LENGTH];//问答列表，包括全部的问答数据
QAInfoItem showList[QA_INFO_ITEM_LENGTH];//问答显示列表，存放筛选后的问答
int showLength = QA_INFO_ITEM_LENGTH;//问答显示列表的长度
QAInfoItem selectedItem;//当前选中的问答

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);//主窗口的消息处理
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);//“关于”窗口的消息处理
LRESULT CALLBACK	Dialog(HWND, UINT, WPARAM, LPARAM);//“问答列表”窗口的消息处理
LRESULT CALLBACK	Infor(HWND, UINT, WPARAM, LPARAM);//“问答详情”窗口的消息处理

/*
 * 将所有问答信息载入infoList（问答列表）
 */
void                getIntoQAInfoList();

/**
 * 从showList（问答显示列表）刷新列表界面控件
 */
void                reflushListView(HWND hwndListView);

/**
 * 求某个字符串的长度
 * orgString 为包含字符数据的指针，字符串以0字符结束
 * return 字符串的长度（ascii）
 */
int                 charPointLength(const char* orgString);

/**
 * orgString字符串是否包含idxString字符串
 * orgString - 主字符串，idxString - 匹配字符串 为包含字符数据的指针，字符串以0字符结束
 * return -1为不包含，大于等于0为idxString在orgString的位置
 */
int                 indexOf(const char* orgString, const char* idxString);

/**
 * 根据keys，从infoList（问答列表）筛选数据到showList（问答显示列表）
 * hDlg 为列表界面的句柄，keys为输入的关键字
 * keys以半角空格或者半角“|”字符分隔
 * 如果以“|”分隔，则分隔后的关键字以“或”的方式匹配
 * 如果以半角空格分隔，则分隔后的关键字以“和”的方式匹配
 */
void                searchListView(HWND hDlg, LPSTR keys);

/**
 * 以“和”的方式匹配并筛选数据到showList（问答显示列表）
 * keyPoint为关键字数组，keyLength为keyPoint中的关键字数目
 */
void                searchAndKeys(LPSTR keyPoint[], int keyLength);

/**
 * 以“或”的方式匹配并筛选数据到showList（问答显示列表）
 * keyPoint为关键字数组，keyLength为keyPoint中的关键字数目
 */
void                searchOrKeys(LPSTR keyPoint[], int keyLength);

/**
 * 转换int为字符串，测试时打印用
 * number为要转换的数字，buff为转换后存放的字符数组
 */
void                getNumberString(int number, char buff[]);

/**
 * 主函数
 */
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
// 主窗口的消息循环，因为主窗口隐藏了，没什么用的
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
			DialogBox(hInst, (LPCTSTR)IDD_DIALOG_LIST, hWnd, (DLGPROC)Dialog);
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
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG_LIST, hWnd, (DLGPROC)Dialog);
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


LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			//初始化的时候，更改版本号和版权信息
			SetDlgItemText(hDlg, IDC_STATIC_VER, VERSION);
			SetDlgItemText(hDlg, IDC_STATIC_TIME, COPYRIGHT_TIME);
			SetDlgItemText(hDlg, IDC_STATIC_BY, COPYRIGHT_BY);
			return TRUE;
		case WM_COMMAND:
			//回车、确定按钮、关闭按钮的事件处理-退出本窗口
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


LRESULT CALLBACK Infor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			//将selectedItem（当前选中的问答）的内容初始化到界面上
			SetDlgItemText(hDlg, IDC_STATIC_QUE, selectedItem.question);
			SetDlgItemText(hDlg, IDC_STATIC_ANS, selectedItem.answer);
			return TRUE;
		case WM_COMMAND:
			//回车、确定按钮、关闭按钮的事件处理-退出本窗口
			if (LOWORD(wParam) == IDC_BTN_INFO || LOWORD(wParam) == IDCANCEL) 
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
	//列表界面控件的样式
	DWORD dwStyle = WS_TABSTOP | WS_CHILD | WS_BORDER | WS_VISIBLE 
		| LVS_AUTOARRANGE | LVS_REPORT;
	
	LV_COLUMN lvColumn;//列表的表头
	HWND edit = GetDlgItem(hDlg, IDC_EDIT1);//关键字的输入框
	LPSTR editText = (LPSTR)malloc(SEARCH_MAX_LENGTH);//关键字的存放数组

	switch (message)
	{
		case WM_INITDIALOG:
			//初始化列表界面控件
			hwndListView = CreateWindowEx(
				WS_EX_CLIENTEDGE, WC_LISTVIEW, TEXT(""), dwStyle, 14, 48, 524, 300, hDlg, NULL, hInst, NULL);
			//设置扩展样式
			ListView_SetExtendedListViewStyle(hwndListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			//设置表头样式
			lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
			//设置表头，分别为表宽、位置、表头标题
			lvColumn.cx = 160;
			lvColumn.iSubItem = 0;
			lvColumn.pszText = TEXT("问题");
			ListView_InsertColumn(hwndListView, 0, &lvColumn);

			lvColumn.cx = 340;
			lvColumn.iSubItem = 1;
			lvColumn.pszText = TEXT("答案");
			ListView_InsertColumn(hwndListView, 1, &lvColumn);

			getIntoQAInfoList();//初始化全部问答数据
			reflushListView(hwndListView);//刷新列表界面控件

			return TRUE;

		case WM_COMMAND:
			//关闭按钮，关闭当前dialog，并退出程序（因为主界面被隐藏，所以本dialog就当是主界面）
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				DestroyWindow(hWindow);
				return TRUE;
			}else
				//回车或者按“搜索”按钮，执行搜索
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDC_SEARCH_BTN){
				GetWindowText(edit, editText, 512);
				searchListView(hDlg, editText);
				reflushListView(hwndListView);
			}
			break;
		case WM_NOTIFY:
			NMLVDISPINFO *plvdi;
			LPNMHDR ppp = (LPNMHDR)lParam;
			char buff[100];
			

			switch(ppp->code){
				case LVN_GETDISPINFO:{//不明事件
					//plvdi = (NMLVDISPINFO *)lParam;
					getNumberString(ppp->code, buff);
					//MessageBox(hDlg, buff, TEXT("提示"), MB_OK);
				};
				break;
				case LVN_COLUMNCLICK:{ //鼠标单击列表标题栏某项
					getNumberString(ppp->code, buff);
					//MessageBox(hDlg, buff, TEXT("提示"), MB_OK);
				};
				break;
				case LVN_ITEMACTIVATE:{ //鼠标双击列表某个内容，表头除外
					//转换参数为NM_LISTVIEW
					NM_LISTVIEW *pnmv = (NM_LISTVIEW FAR *)lParam;
					//根据行号从showList中获取选中的问答
					selectedItem = showList[pnmv->iItem];
					//显示问题详情dialog
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG_INFO, hDlg, (DLGPROC)Infor);
				};
				break;
				case NM_CLICK:{ //鼠标单击某个列表项的事件
					getNumberString(ppp->code, buff);
				//	MessageBox(hDlg, buff, TEXT("提示"), MB_OK);
				};
				break;
				default:;
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

	//复制keys字符串，用于后续修改用的
	strcpy(keyParam, keys);

	begin = 0;
	i = 0;
	ch = *(unsigned char*)(keyParam);

	/*
	 * 整个for循环逐个遍历keyParam数组的字符
	 * 当遇到空格或者|字符时，将上个空格或者|字符后的第一个字符的指针赋值到keyPoint
	 * 改变空格或者|字符为0，这样可以在赋值到keyPoint的指针就是一个字符串了
	 */
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
	//处理最后一个字符串
	if(count > 0){
		keyPoint[keyLength] = keyParam + begin;
		keyLength++;
	}

	//根据“和”和“或”选择要如何筛选
	if(isOr > 0){
		searchOrKeys(keyPoint, keyLength);
	}else{
		searchAndKeys(keyPoint, keyLength);
	}
}

void searchOrKeys(LPSTR keyPoint[], int keyLength){
	int i = 0, j = 0;
	showLength = 0;

	/*
	 * 大for循环中，全量遍历每个问答
	 * 小for循环中，遍历关键字
	 * 问题和答案任意匹配其中一个关键字就把数据追加到showList中
	 */
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

	/*
	 * 大for循环中，全量遍历每个问答
	 * 小for循环1中，遍历匹配问题和关键字
	 * 小for循环2中，遍历匹配答案和关键字
	 * 问题 或者 答案完全匹配所有关键字，就把数据追加到showList中
	 */
	for(i=0; i<QA_INFO_ITEM_LENGTH; i++){
		//遍历所有关键字是否匹配问题
		isFound = 1;
		for(j=0; j<keyLength; j++){
			if(indexOf(infoList[i].question, keyPoint[j]) < 0){
				isFound = -1;
				break;
			}
		}
		if(isFound > 0){
			showList[showLength]= infoList[i];
			showLength++;
			continue;
		}
		//遍历所有关键字是否匹配答案
		isFound = 1;
		for(j=0; j<keyLength; j++){
			if(indexOf(infoList[i].answer, keyPoint[j]) < 0){
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

	//首先获取两个字符串的长度
	int orgLeng = charPointLength(orgString);
	int idxLeng = charPointLength(idxString);
	
	//如果两个字符都为空，则不匹配
	if(orgLeng <= 0 || idxLeng <= 0){
		return -1;
	}
	//如果匹配串的长度大于主串，肯定不匹配
	if(idxLeng > orgLeng){
		return -1;
	}else
	if(idxLeng == orgLeng){
		//如果两个字符串的长度相等，则只要对比两个字符串一样，则匹配，否则不匹配
		if(strcmp(orgString, idxString) > 0){
			return 0;
		}else{
			return -1;
		}
	}

	/*
	 * 用大for循环从主串的第一个字符开始遍历
	 * 用小for循环先匹配上匹配串的第一个字符
	 * 如果不匹配，则跳出小for循环，重新匹配
	 * 如果匹配上，则用迷你for循环匹配主串和匹配串的后续字符
	 * 如果不匹配，则跳出迷你for循环和小for循环，重新匹配
	 * 如果匹配上，就立即返回匹配的位置
	 */
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
			//迷你for循环，用来匹配后续的字符串是否相等
			for(;ii<orgLeng && jj<idxLeng; ii++, jj++){
				chA1 = *(unsigned char*)(orgString + ii);
				chB1 = *(unsigned char*)(idxString + jj);
				if(chA1 != chB1){
					beginIdx = -1;
					break;
				}
			}
			//是否在匹配的情况下跳出迷你for循环
			if(beginIdx >= 0){
				//如果主串和匹配串直到末尾都匹配，则两个串匹配
				if(ii >= orgLeng && jj >= idxLeng){
					return beginIdx;
				}
				//如果主串不到末尾，匹配串到末尾了，则两个串匹配
				if(ii < orgLeng && jj >= idxLeng){
					return beginIdx;
				}
				//其他情况视为不匹配
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
	//删除所有数据
	ListView_DeleteAllItems(hwndListView);
	//重置mask，不重置会报错
	lvItem.mask = LVIF_TEXT;
			
	for(idx=0; idx<showLength; idx++){
		lvItem.iItem = idx;
		//添加问题
		lvItem.iSubItem = 0;
		lvItem.pszText = showList[idx].question;
		ListView_InsertItem(hwndListView, &lvItem);
		//添加答案
		lvItem.iSubItem = 1;
		lvItem.pszText = showList[idx].answer;
		ListView_SetItem(hwndListView, &lvItem);
	}
}

void getNumberString(int number, char buff[]){
	int len = 0, temp = number, idx = 0;
	char exg = 0;

	if(number == 0){
		buff[0] = '0';
		buff[1] = 0;
		return;
	}
	if(number < 0){
		temp = -temp;
	}
	for(;temp > 0; temp /=10){
		buff[len++] = ('0' + (temp % 10));
	}
	if(number < 0){
		buff[len++] = '-';
	}
	buff[len] = 0;
	for(temp = 0, idx=len-1; temp<idx; temp++, idx--){
		exg = buff[temp];
		buff[temp] = buff[idx];
		buff[idx] = exg;
	}
	return;
}

//TODO 获取全量的问答信息，暂时没有问答信息，这个是模拟的
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
