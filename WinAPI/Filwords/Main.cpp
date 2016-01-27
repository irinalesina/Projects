#include <windows.h>
#include <tchar.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <locale.h>
#include <Windowsx.h>


int window_score_x = 400, window_score_y = 30;
int letter_width = 100, letter_heigth = 100;

std::string answers_file("Data/answers/");
std::string task_file("Data/task/");

HDC hdc;
PAINTSTRUCT ps;


using namespace std;

//�������� ������� ���������
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szClassWindow[] = TEXT("��������� ����������"); /* ��� ������ ���� */


void CreateContent(HWND hWnd, HINSTANCE hInst);

LRESULT CALLBACK OnExit(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam){
	PostQuitMessage(0);	// ������� ��������� WM_QUIT
	return 0;
}
 

struct Data
{
	// for create letters
	vector<vector<char>> letters;

	// answers
	vector<wstring> answers;
};

Data data;

int score = 0;
wstring user_answer;

vector<HWND> buttons;
vector<HWND> click_buttons;
HWND button_check;
HWND score_window;
int level = 1, level_count = 6;
HINSTANCE hInst;




INT WINAPI _tWinMain(HINSTANCE hInst_, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	setlocale(LC_ALL, "rus");
	// ���������� �������� ����
	HWND hWnd;
	hInst = hInst_;
	MSG Msg;

	// ���������, ������� �������� �������� ������ ����
	WNDCLASSEX wcl;

	/* 1. ����������� ������ ����  */

	wcl.cbSize = sizeof (wcl);	// ������ ��������� WNDCLASSEX 
	wcl.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	// ���� ������ �������� ��������� � ������� ������ (DBLCLK)
	wcl.lpfnWndProc = WindowProc;	// ����� ������� ���������
	wcl.cbClsExtra = 0;		// ������������ Windows 
	wcl.cbWndExtra = 0; 	// ������������ Windows 
	wcl.hInstance = hInst;	// ���������� ������� ����������
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// �������� ����������� ������
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);		// �������� ������������ �������
	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//���������� ���� ����� ������			
	wcl.lpszMenuName = NULL;	// ���������� �� �������� ����
	wcl.lpszClassName = szClassWindow;	// ��� ������ ����
	wcl.hIconSm = NULL;	// ���������� ��������� ������ ��� ����� � ������� ����


	/*  2. ����������� ������ ����  */

	if (!RegisterClassEx(&wcl))
		return 0;	// ��� ��������� ����������� - �����

	/*  3. �������� ����  */

	// ��������� ���� �  ���������� hWnd ������������� ���������� ����
	hWnd = CreateWindowEx(
		0,		// ����������� ����� ����
		szClassWindow,	// ��� ������ ����
		TEXT("��������"),	// ��������� ����
		/* ���������, �����, ����������� ������ �������, ��������� ����,
		������ ������������ � ���������� ����  */
		WS_OVERLAPPEDWINDOW,	// ����� ����
		CW_USEDEFAULT,	// �-���������� ������ �������� ���� ����
		CW_USEDEFAULT,	// y-���������� ������ �������� ���� ����
		500,	// ������ ����
		600,	// ������ ����
		NULL,			// ���������� ������������� ����
		NULL,			// ���������� ���� ����
		hInst,		// ������������� ����������, ���������� ����
		NULL);		// ��������� �� ������� ������ ����������

	
	button_check = CreateWindow(
	L"BUTTON",  // Predefined class; Unicode assumed 
	L"check out",      // Button text 
	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	200,         // x position 
	450,         // y position 
	100,        // Button width
	30,        // Button height
	hWnd, (HMENU)2, hInst, NULL);

	score_window = CreateWindow(TEXT("Static"), TEXT("Score: 0"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
		50, 25, window_score_x, window_score_y, hWnd, NULL, hInst, NULL);



	
	CreateContent(hWnd, hInst);
	

	/* 4. ����������� ���� */

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);	// ����������� ����
	MessageBox(hWnd, (wstring(L"Level: ") + to_wstring(level)).c_str(), L"Statistic", MB_OK | MB_ICONINFORMATION);
	/* 5. ������ ����� ��������� ���������  */

	// ��������� ���������� ��������� �� ������� ���������
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);	// ���������� ���������
		DispatchMessage(&Msg);	// ��������������� ���������
	}
	return Msg.wParam;
}


// ������� ��������� ���������� ������������ �������� � �������� � �������� 
// ���������� ��������� �� ������� ��������� ������� ����������	

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_COMMAND:
		{
			if ((HWND)lParam == button_check && HIWORD(wParam) == BN_CLICKED)
			{
				bool check = false;
				//user_answer.push_back(L'\0');
				for (auto it = data.answers.begin(); it != data.answers.end(); it++)
				{
				    if (*it == user_answer)
				    {
						check = true;
						data.answers.erase(it);
						break;
				    }							
				}
				if (check)
				{
					//MessageBox(hWnd, L"You have found!", L"Result", MB_OK | MB_ICONINFORMATION);
					click_buttons.clear();
					score += 100;
					SetWindowText(score_window, (wstring(L"Score: ") + to_wstring(score)).c_str());

					if (data.answers.size() == 0 && level < level_count)
					{
						MessageBox(hWnd, (wstring(L"Level: ") + to_wstring(++level)).c_str(), L"Statistic", MB_OK | MB_ICONINFORMATION);
						data.letters.clear();
						buttons.clear();
						CreateContent(hWnd, hInst);
					}
						
					if (data.answers.size() == 0 && level >= level_count)
					{
						MessageBox(hWnd, (wstring(L"Congratulations, you win ") + to_wstring(score) + L"$").c_str(), L"Statistic", MB_OK | MB_ICONINFORMATION);
						PostMessage(hWnd, WM_DESTROY, 0, 0);
					}
						
				}
				else
				{
					for (auto it : click_buttons)
						EnableWindow(it, true);
					score -= 50;
					if (score < 50 && score >= 0)
					{
						MessageBox(hWnd, (wstring(L"you have last attempt!")).c_str(), L"Statistic", MB_OK | MB_ICONINFORMATION);
					}
					if (score < 0)
					{
						MessageBox(hWnd, (wstring(L"Haha, you lose!")).c_str(), L"Statistic", MB_OK | MB_ICONINFORMATION);
						PostMessage(hWnd, WM_DESTROY, 0, 0);
					}
					SetWindowText(score_window, (wstring(L"Score: ") + to_wstring(score)).c_str());
					
				}
				user_answer.clear();
			}
			else
			{
				wchar_t name[2];
				Button_GetText((HWND)lParam, name, 2);
				user_answer.push_back(name[0]);
				click_buttons.push_back((HWND)lParam);
				//user_answer += name[0];
				//std::wstring s;
				//SetWindowText((HWND)lParam, s.c_str());
				EnableWindow((HWND)lParam, false);
			}
		}
		break;
	case WM_DESTROY: // ��������� � ���������� ���������
		//
		PostQuitMessage(0); // ������� ��������� WM_QUIT
		break;
	default:
		// ��� ���������, ������� �� �������������� � ������ ������� ������� 
		// ������������ ������� Windows �� ��������� �� ���������
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}

	return 0;
}

bool GetAnswers(string level);
bool GetTask(string level);

void CreateContent(HWND hWnd, HINSTANCE hInst)
{
	/*LPRECT size;
	GetWindowRect(hWnd, size);*/
	
	GetAnswers(to_string(level));
	GetTask(to_string(level));

	int x_elements = 0, y_elements = 0;
	for(auto it_1: data.letters)
	{
		for(auto it_2: it_1)
		{
			wchar_t name[2] = {0};
			mbstowcs(name, &it_2, 1);

			HWND button;
			button = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			name,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			letter_width * x_elements + 100,         // x position 
			letter_heigth * y_elements + 100,         // y position 
			letter_width,        // Button width
			letter_heigth,        // Button height
			hWnd,     // Parent window
			(HMENU)2,       // button id (h - menu)
			hInst,
			NULL);
			buttons.push_back(button);
			x_elements++;
		}
		x_elements = 0;
		y_elements++;
	}
}


bool GetAnswers(string level)
{
	wifstream file;
	wstring buf;

	string file_name = answers_file + level + ".txt";

	file.open(file_name);

	file.imbue(locale(file.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>));

	while(!file.eof())
	{
		getline(file, buf);
		data.answers.push_back(buf);
	}

	file.close();
	return true;
}


bool GetTask(string level)
{
	ifstream file;
	string buf;

	string file_name = task_file + level + ".txt";

	file.open(file_name);

	while(!file.eof())
	{
		getline(file, buf);

		vector<char> temp;
		for(int i = 0; i < buf.size(); i++)
			temp.push_back(buf[i]);

		data.letters.push_back(temp);
	}

	file.close();
	return true;
}

