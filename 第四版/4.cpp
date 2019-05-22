#include "pch.h"
#include "Player.h"

using namespace std;
//使用opencv的命名空间
using namespace cv;

//设置cmd的函数，包括全屏，标题等等
void Setcmd() {

	//HWND是窗口句柄，GetForegroundWindow用于得到前台窗口句柄
	HWND hwnd = GetForegroundWindow();

	//设置标题名称
	SetConsoleTitleA("Console Vedio Player-debug");
	//得到屏幕长度
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	//得到屏幕宽度
	int screen_length = GetSystemMetrics(SM_CYSCREEN);

	//获取hwnd的窗口信息，GWL_STYLE获取窗口风格信息
	//注意：它是由一串二进制代码010101010101011等等共32位构成的，每一位代表一个功能
	LONG  l_Winstyle = GetWindowLong(hwnd, GWL_STYLE);

	//进行设置
	SetWindowLong(hwnd, GWL_STYLE, (l_Winstyle | WS_POPUP | WS_MAXIMIZE));

	//进行设置
	SetWindowPos(hwnd, HWND_TOP, 0, 0, screen_width, screen_length, 0);
}



int main()
{
	
	
	//取消cin与stdin的同步，提升程序效率
	ios::sync_with_stdio(false);
	Player py(GetStdHandle(STD_OUTPUT_HANDLE), GetForegroundWindow(), "0.mp4");
	//这里先把字体调大方便用户进行设置操作
	py.SetPixelSize(20);
	cout << endl;
	cout << "是否需要进行设置？1 or 0：" << endl;
	int key;
	cin >> key;
	//如果输入1则进行设置
	if (key == 1) {
		py.Setdelay();
		py.SetPixelSize();
		py.Output_to_file();
	}
	//否则读取之前的设置
	else {
		py.Gain_date_from_file();
		py.Setdelay(py.Get_mydelay());
		py.SetPixelSize(py.Get_Myfontsize());
	}
	
	py.PlayVideo();

	return 0;
}