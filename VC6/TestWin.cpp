// TestWin.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "stdio.h"
#include "CommCtrl.h"
#include <string>
#include "QAInfor.h"

#define MAX_LOADSTRING 100
#define SEARCH_MAX_LENGTH 512 //���������������ֽ���

LPSTR VERSION = TEXT("V1.0.0");//����汾��
LPSTR COPYRIGHT_TIME = TEXT("2019");//�����Ȩʱ��
LPSTR COPYRIGHT_BY = TEXT("Hh7");//�����Ȩ����
LPSTR USER_AGREEMENT = TEXT("һ������������\r\n������ʵ����ʵ�ϡ�\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\nһ������������\r\n");//�û�Э��


// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWindow;//���ڵľ��
HWND hwndListView;//�б�ľ��

HFONT hfont;//����

TCHAR szTitle[MAX_LOADSTRING];//����
TCHAR szWindowClass[MAX_LOADSTRING];// The title bar text

QAInfoItem showList[QA_INFO_ITEM_LENGTH];//�ʴ���ʾ�б����ɸѡ����ʴ�
int showLength = QA_INFO_ITEM_LENGTH;//�ʴ���ʾ�б�ĳ���
QAInfoItem selectedItem;//��ǰѡ�е��ʴ�

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);//�����ڵ���Ϣ����
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);//�����ڡ����ڵ���Ϣ����
LRESULT CALLBACK	Dialog(HWND, UINT, WPARAM, LPARAM);//���ʴ��б����ڵ���Ϣ����
LRESULT CALLBACK	Infor(HWND, UINT, WPARAM, LPARAM);//���ʴ����顱���ڵ���Ϣ����

/**
 * ��showList���ʴ���ʾ�б�ˢ���б����ؼ�
 */
void                reflushListView(HWND hwndListView);

/**
 * ��ĳ���ַ����ĳ���
 * orgString Ϊ�����ַ����ݵ�ָ�룬�ַ�����0�ַ�����
 * return �ַ����ĳ��ȣ�ascii��
 */
int                 charPointLength(const char* orgString);

/**
 * orgString�ַ����Ƿ����idxString�ַ���
 * orgString - ���ַ�����idxString - ƥ���ַ��� Ϊ�����ַ����ݵ�ָ�룬�ַ�����0�ַ�����
 * return -1Ϊ�����������ڵ���0ΪidxString��orgString��λ��
 */
int                 indexOf(const char* orgString, const char* idxString);

/**
 * ����keys����infoList���ʴ��б�ɸѡ���ݵ�showList���ʴ���ʾ�б�
 * hDlg Ϊ�б����ľ����keysΪ����Ĺؼ���
 * keys�԰�ǿո���߰�ǡ�|���ַ��ָ�
 * ����ԡ�|���ָ�����ָ���Ĺؼ����ԡ��򡱵ķ�ʽƥ��
 * ����԰�ǿո�ָ�����ָ���Ĺؼ����ԡ��͡��ķ�ʽƥ��
 */
void                searchListView(HWND hDlg, LPSTR keys);

/**
 * �ԡ��͡��ķ�ʽƥ�䲢ɸѡ���ݵ�showList���ʴ���ʾ�б�
 * keyPointΪ�ؼ������飬keyLengthΪkeyPoint�еĹؼ�����Ŀ
 */
void                searchAndKeys(LPSTR keyPoint[], int keyLength);

/**
 * �ԡ��򡱵ķ�ʽƥ�䲢ɸѡ���ݵ�showList���ʴ���ʾ�б�
 * keyPointΪ�ؼ������飬keyLengthΪkeyPoint�еĹؼ�����Ŀ
 */
void                searchOrKeys(LPSTR keyPoint[], int keyLength);

/**
 * ת��intΪ�ַ���������ʱ��ӡ��
 * numberΪҪת�������֣�buffΪת�����ŵ��ַ�����
 */
void                getNumberString(int number, char buff[]);

/**
 * ������
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

	wcex.style			= CS_HREDRAW | CS_VREDRAW | DS_CENTER;
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
// �����ڵ���Ϣѭ������Ϊ�����������ˣ�ûʲô�õ�
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
			//������ʾ����
			hfont = CreateFont(
				-18, -9, 0, 0, 400,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET,
				OUT_CHARACTER_PRECIS,
				CLIP_CHARACTER_PRECIS,
				DEFAULT_QUALITY,
				FF_DONTCARE,
				TEXT("")
				);

			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
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
					DeleteObject(hfont);
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
			SendMessage(hDlg, WM_SETFONT, (WPARAM)hfont, NULL);
			//��ʼ����ʱ�򣬸��İ汾�źͰ�Ȩ��Ϣ
			SetDlgItemText(hDlg, IDC_STATIC_VER, VERSION);
			SetDlgItemText(hDlg, IDC_STATIC_TIME, COPYRIGHT_TIME);
			SetDlgItemText(hDlg, IDC_STATIC_BY, COPYRIGHT_BY);
			SetDlgItemText(hDlg, IDC_USER_CFG_EDIT, USER_AGREEMENT);
			
			return TRUE;
		case WM_COMMAND:
			//�س���ȷ����ť���رհ�ť���¼�����-�˳�������
			if (LOWORD(wParam) == IDUNOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				DeleteObject(hfont);
				DestroyWindow(hWindow);
				return TRUE;
			}
			if(LOWORD(wParam) == IDOK){
				EndDialog(hDlg, LOWORD(wParam));
				DialogBox(hInst, (LPCTSTR)IDD_DIALOG_LIST, hDlg, (DLGPROC)Dialog);
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
			SendMessage(hDlg, WM_SETFONT, (WPARAM)hfont, NULL);
			//��selectedItem����ǰѡ�е��ʴ𣩵����ݳ�ʼ����������
			SetDlgItemText(hDlg, IDC_EDIT_QUE, selectedItem.question);
			SetDlgItemText(hDlg, IDC_EDIT_ANS, selectedItem.answer);
			return TRUE;
		case WM_COMMAND:
			//�س���ȷ����ť���رհ�ť���¼�����-�˳�������
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
	//�б����ؼ�����ʽ
	DWORD dwStyle = WS_TABSTOP | WS_CHILD | WS_BORDER | WS_VISIBLE 
		| LVS_AUTOARRANGE | LVS_REPORT;
	
	LV_COLUMN lvColumn;//�б�ı�ͷ
	HWND edit = GetDlgItem(hDlg, IDC_EDIT1);//�ؼ��ֵ������
	LPSTR editText = (LPSTR)malloc(SEARCH_MAX_LENGTH);//�ؼ��ֵĴ������
	

	switch (message)
	{
		case WM_INITDIALOG:
			//��ʼ���б����ؼ�
			hwndListView = CreateWindowEx(
				WS_EX_CLIENTEDGE, WC_LISTVIEW, TEXT(""), dwStyle, 14, 48, 930, 580, hDlg, NULL, hInst, NULL);
			//������չ��ʽ
			ListView_SetExtendedListViewStyle(hwndListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			SendMessage(hwndListView, WM_SETFONT, (WPARAM)hfont, NULL);

			//���ñ�ͷ��ʽ
			lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
			//���ñ�ͷ���ֱ�Ϊ���λ�á���ͷ����
			lvColumn.cx = 920;
			lvColumn.iSubItem = 0;
			lvColumn.pszText = TEXT("����");
			ListView_InsertColumn(hwndListView, 0, &lvColumn);

			getIntoQAInfoList();//��ʼ��ȫ���ʴ�����
			showLength = 0;
			reflushListView(hwndListView);//ˢ���б����ؼ�

			return TRUE;

		case WM_COMMAND:
			//�رհ�ť���رյ�ǰdialog�����˳�������Ϊ�����汻���أ����Ա�dialog�͵��������棩
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				DeleteObject(hfont);
				DestroyWindow(hWindow);
				return TRUE;
			}else
				//�س����߰�����������ť��ִ������
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
				case LVN_GETDISPINFO:{//�����¼�
					//plvdi = (NMLVDISPINFO *)lParam;
					getNumberString(ppp->code, buff);
					//MessageBox(hDlg, buff, TEXT("��ʾ"), MB_OK);
				};
				break;
				case LVN_COLUMNCLICK:{ //��굥���б������ĳ��
					getNumberString(ppp->code, buff);
					//MessageBox(hDlg, buff, TEXT("��ʾ"), MB_OK);
				};
				break;
				case LVN_ITEMACTIVATE:{ //���˫���б�ĳ�����ݣ���ͷ����
					//ת������ΪNM_LISTVIEW
					NM_LISTVIEW *pnmv = (NM_LISTVIEW FAR *)lParam;
					//�����кŴ�showList�л�ȡѡ�е��ʴ�
					selectedItem = showList[pnmv->iItem];
					//��ʾ��������dialog
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG_INFO, hDlg, (DLGPROC)Infor);
				};
				break;
				case NM_CLICK:{ //��굥��ĳ���б�����¼�
					getNumberString(ppp->code, buff);
				//	MessageBox(hDlg, buff, TEXT("��ʾ"), MB_OK);
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

	//����keys�ַ��������ں����޸��õ�
	strcpy(keyParam, keys);

	begin = 0;
	i = 0;
	ch = *(unsigned char*)(keyParam);

	/*
	 * ����forѭ���������keyParam������ַ�
	 * �������ո����|�ַ�ʱ�����ϸ��ո����|�ַ���ĵ�һ���ַ���ָ�븳ֵ��keyPoint
	 * �ı�ո����|�ַ�Ϊ0�����������ڸ�ֵ��keyPoint��ָ�����һ���ַ�����
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
	//�������һ���ַ���
	if(count > 0){
		keyPoint[keyLength] = keyParam + begin;
		keyLength++;
	}

	if(keyLength <= 0){
		showLength = 0;
		return;
	}

	//���ݡ��͡��͡���ѡ��Ҫ���ɸѡ
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
	 * ��forѭ���У�ȫ������ÿ���ʴ�
	 * Сforѭ���У������ؼ���
	 * ����ʹ�����ƥ������һ���ؼ��־Ͱ�����׷�ӵ�showList��
	 */
	for(i=0; i<QA_INFO_ITEM_LENGTH; i++){
		for(j=0; j<keyLength; j++){
			if(indexOf(infoList[i].question, keyPoint[j]) >= 0){
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
	 * ��forѭ���У�ȫ������ÿ���ʴ�
	 * Сforѭ��1�У�����ƥ������͹ؼ���
	 * Сforѭ��2�У�����ƥ��𰸺͹ؼ���
	 * ���� ���� ����ȫƥ�����йؼ��֣��Ͱ�����׷�ӵ�showList��
	 */
	for(i=0; i<QA_INFO_ITEM_LENGTH; i++){
		//�������йؼ����Ƿ�ƥ������
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

	//���Ȼ�ȡ�����ַ����ĳ���
	int orgLeng = charPointLength(orgString);
	int idxLeng = charPointLength(idxString);
	
	//��������ַ���Ϊ�գ���ƥ��
	if(orgLeng <= 0 || idxLeng <= 0){
		return -1;
	}
	//���ƥ�䴮�ĳ��ȴ����������϶���ƥ��
	if(idxLeng > orgLeng){
		return -1;
	}else
	if(idxLeng == orgLeng){
		//��������ַ����ĳ�����ȣ���ֻҪ�Ա������ַ���һ������ƥ�䣬����ƥ��
		if(strcmp(orgString, idxString) > 0){
			return 0;
		}else{
			return -1;
		}
	}

	/*
	 * �ô�forѭ���������ĵ�һ���ַ���ʼ����
	 * ��Сforѭ����ƥ����ƥ�䴮�ĵ�һ���ַ�
	 * �����ƥ�䣬������Сforѭ��������ƥ��
	 * ���ƥ���ϣ���������forѭ��ƥ��������ƥ�䴮�ĺ����ַ�
	 * �����ƥ�䣬����������forѭ����Сforѭ��������ƥ��
	 * ���ƥ���ϣ�����������ƥ���λ��
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
			//����forѭ��������ƥ��������ַ����Ƿ����
			for(;ii<orgLeng && jj<idxLeng; ii++, jj++){
				chA1 = *(unsigned char*)(orgString + ii);
				chB1 = *(unsigned char*)(idxString + jj);
				if(chA1 != chB1){
					beginIdx = -1;
					break;
				}
			}
			//�Ƿ���ƥ����������������forѭ��
			if(beginIdx >= 0){
				//���������ƥ�䴮ֱ��ĩβ��ƥ�䣬��������ƥ��
				if(ii >= orgLeng && jj >= idxLeng){
					return beginIdx;
				}
				//�����������ĩβ��ƥ�䴮��ĩβ�ˣ���������ƥ��
				if(ii < orgLeng && jj >= idxLeng){
					return beginIdx;
				}
				//���������Ϊ��ƥ��
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
	//ɾ����������
	ListView_DeleteAllItems(hwndListView);
	//����mask�������ûᱨ��
	lvItem.mask = LVIF_TEXT;
			
	for(idx=0; idx<showLength; idx++){
		lvItem.iItem = idx;
		//�������
		lvItem.iSubItem = 0;
		lvItem.pszText = showList[idx].question;
		ListView_InsertItem(hwndListView, &lvItem);
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

//TODO ��ȡȫ�����ʴ���Ϣ����ʱû���ʴ���Ϣ�������ģ���
void getIntoQAInfoList(){
/*	int i = 0;
	char ans[] = "100�ꡣ";
	char que[] = "000�˿��Ի�����ꣿ";

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
*/
	infoList[0].question = TEXT("��ͬ��ʩ����ͬ�������ͬ����Ҫ������");
	infoList[1].question = TEXT("�걨ʩ�������Ҫ�˲�ԭ��/��ȡԭ����");
	infoList[2].question = TEXT("��������������/��ô�������ύ");
	infoList[3].question = TEXT("��Ŀ�������ڶ��ٸ���Ŀ����ְ");
	infoList[4].question = TEXT("�ܼ�����ʦ���ڶ��ٸ���Ŀ����ְ");
	infoList[5].question = TEXT("�������Ҫ���£����¡�ע���£���");
	infoList[6].question = TEXT("����ϵͳ�ϱ�������Ա��ʩ����λ������λ���Ҳ���ѡ������ʲôԭ����ô��");
	infoList[7].question = TEXT("��Ա�����ˣ�Ϊʲô��ʾ֤�����ڣ��Ѿ�������");
	infoList[8].question = TEXT("Ļǽ��ȼ����װ�޹���Ҫ����ʩ����ɵ�����ô��");
	infoList[9].question = TEXT("����ʩ������Ƿ�һ��Ҫȡ���˷�������");

	infoList[0].answer = TEXT("����Ҫ");
	infoList[1].answer = TEXT("ȫ����������ֻ���ṩ��ɫ���Ӱ�ɨ�����PDF��ʽ���������ṩֽ�ʰ棬Ҳ����Ҫ��ʵԭ��");
	infoList[2].answer = TEXT("ʩ����ɺ�׼��\r\n��������ʱ�㿴�����ҳ���Ǹ�����Ŀ����ǿհ׵ģ���\r\n��ѯ���й��̹滮���ݡ����뽨�蹤�̹滮���֤�š�\r\nѡ����Ŀ��\r\n��ת��ѡ�������ʩ�����֤����ҳ���\r\n���ȫ����ѡ��\r\n���Ͻǵ�ѡ���\r\n��ʱ����ת���ոյڶ����Ǹ�������ҳ�棨��ʱ�㿴�����ҳ���Ǹ�����Ŀ����������ݣ���\r\n�㱨����\r\n��ʱ�㿴��һ��ҳ�棬���涼�ǿհ׵ģ�����һ���н�����Ŀ�Ǽǡ�������Ϣ����Ŀ�ſ�����Ŀ��Χ�ȵȣ�ȫ������д��������Ϣ���ϴ���Ӧ��ɨ�����ϡ�\r\n��;���Ե��ݴ��\r\nȫ����ɵ��ύ\r\n");
	infoList[3].answer = TEXT("1��");
	infoList[4].answer = TEXT("3��");
	infoList[5].answer = TEXT("�������Ҫ�巽���£��ǹ��£�������Ҫ�����������ע����");
	infoList[6].answer = TEXT("1����ʩ����λ�����λ��ͣ�ƣ������ͣһ�б���ҵ��ϵͳ��������������Ա�ġ�ֻ�ܰ�ͣ��ԭ������ָ�ԭ״���С�\r\n2����Աδ������ϵͳ����Ϣ������ȥ�麣����ס���ֱ�����Ա��\r\n");
	infoList[7].answer = TEXT("��һ�α�����ʱ������Ч�ڣ���Ч�ڵ��˻���ʾ���ڡ����ں���Ҫȥ��ס���ֱ�����Ա�Ǹ�����Ϣ���������ں�ϵͳ���ǻ���ʾ֤���ѹ��ڡ�");
	infoList[8].answer = TEXT("1��רҵ�а���λ��ʩ����λ�����뽨�赥λǩ����ͬ�ģ����赥������ʩ�����\r\n2��רҵ�а���λ��ʩ����λ�������ܰ���λǩ����ͬ�ģ������רҵ�ְ������ܰ�����ʩ��֤�������\r\n");
	infoList[9].answer = TEXT("���ӡ�׮�������̰���ʩ��������迼���˷������幤�̱���ʩ�����ǰ����ȡ���˷���������");

}