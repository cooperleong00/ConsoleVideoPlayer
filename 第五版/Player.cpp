#include "pch.h"
#include "Player.h"
using namespace std;
using namespace cv;
Player::Player()
{
	FramesNum = 0;
	FrameWidth = 1920;
	FrameHeight = 1080;
	speed = 1.0;
	FrameRate = 30.0;
}

void Player::SetPlayerConfig(string cmd)
{
	string filename=cmd;
	double sp;
	//分析输入命令
	for (int i = 0; i < cmd.length(); i++) {
		if (cmd[i] == ' ') {
			filename = cmd.substr(0, i);
			try { sp = stod(cmd.substr(i)); speed = sp; }
			catch (exception) {
				cout << "输入的视频倍速有误！";
				speed = 1.0;
			}
			break;
		}
	}
	GetFrameFromVideo(filename);
}

void Player::SetStdWindow(HANDLE hd, HWND hwnd)
{
	MyHandle = hd;
	MyHwnd = hwnd;
}

void Player::SetPixelSize(int size)
{
	Myfontsize = size;
	WCHAR myFont[] = TEXT("Arial");
	_CONSOLE_FONT_INFOEX fontinfo = { sizeof(CONSOLE_FONT_INFOEX) ,1,{Myfontsize,Myfontsize},TMPF_FIXED_PITCH,400, *myFont };
	SetCurrentConsoleFontEx(MyHandle, false, &fontinfo);
}

void Player::Output_to_file()
{
	ofstream file;
	file.open("set.txt", std::ios::out);
	file << Myfontsize << endl;
	file.close();
}

void Player::Gain_date_from_file()
{
	//利用数组存储数据
	int data[10];
	ifstream file;
	file.open("set.txt");
	//读取所有数据
	for (int i = 0; !file.eof(); i++) {
		file >> data[i];
	}
	Myfontsize = data[0];
	cout << "Size:" << Myfontsize << endl;
	file.close();

}

int Player::Get_Myfontsize()
{
	return Myfontsize;
}

void Player::Set()
{
	double key = 1;
	cout << "是否需要进行设置？输入1进行设置，否则将使用默认设置" << endl;
	cin >> key;
	if (key == 1) {
		SetPixelSize();
		Output_to_file();
	}
	else {
		Gain_date_from_file();
		SetPixelSize(Get_Myfontsize());
	}

}

void Player::SetWindowConfig()
{
	//得到屏幕长度
	int screen_width = GetSystemMetrics(SM_CXSCREEN);

	//得到屏幕宽度
	int screen_length = GetSystemMetrics(SM_CYSCREEN);

	//获取hwnd的窗口信息，GWL_STYLE获取窗口风格信息
	//注意：它是由一串二进制代码010101010101011等等共32位构成的，每一位代表一个功能
	LONG  l_Winstyle = GetWindowLong(MyHwnd, GWL_STYLE);

	//进行设置
	SetWindowLong(MyHwnd, GWL_STYLE, (l_Winstyle | WS_POPUP | WS_MAXIMIZE));

	//进行设置
	SetWindowPos(MyHwnd, HWND_TOP, 0, 0, screen_width, screen_length, 0);
}

void Player::GetFrameFromVideo(string FileName)
{
	//通过文件名输入视频（绝对路径或相对路径）
	VideoCapture vc;
	try {
		vc.open(FileName);
	}
	catch (Exception) {
		cout << "视频路径格式有误或视频不存在" << endl;
		return;
	}

	//声明将要预处理的帧
	Mat frameRaw;

	//获得视频总帧数
	//CAP_PROP_FRAME_COUNT是opencv宏定义的抓取帧数总量
	FramesNum = vc.get(CAP_PROP_FRAME_COUNT);

	//获取帧率
	FrameRate = vc.get(CAP_PROP_FPS);

	//得到控制台句柄，以使用控制台函数
	//说明当前用的控制台是哪个
	//HANDLE temp_hd = MyHandle;



	//将光标设为不可见,1表示光标厚度，false表示不可见
	_CONSOLE_CURSOR_INFO cf = { 1,false };


	//使用vector数组来储存每一帧
	//+3是为了冗余
	vector<string> allframe(FramesNum + 3);

	//记录开始时间
	double start = GetTickCount64();

	//计数用
	int framediff = 0, width = FrameWidth = 800, height = FrameHeight = 450, j, k;
	string output = "";
	string sign[2] = { " ","#" };
	//CAP_PROP_POS_AVI_RATIO是视频相对位置,0~1的一个值
	//这样写一样是为了冗余 
	//这整个for循环用于读取视频每一帧，并存放到数组里。
	for (int i = 0; i < FramesNum && vc.get(CAP_PROP_POS_AVI_RATIO) < (1 - 10E-6); i++) {
		Mat frameGray, frameOut;
		//将光标设为不可见
		SetConsoleCursorInfo(MyHandle, &cf);

		//定义灰度值矩阵，和输出矩阵

		//获得下一帧，如果还未获得过，则获取第一帧
		vc >> frameRaw;

		//将当前RGB帧转换为灰度图像
		cvtColor(frameRaw, frameGray, 7);

		//缩小尺寸
		resize(frameGray, frameOut, Size(FrameWidth, FrameHeight));


		//获取输出的字符串
		for (j = 0; j < FrameHeight; j++) {
			for (k = 0; k < FrameWidth; k++) {

				//二值化显示像素，灰度大于127即显示
				output += sign[frameOut.at<bool>(j, k) / 127];
			}
			output += "\n";
		}
		allframe[i] = output;
		output = "";

		//提示当前处理到第几个帧了（读取时的第几帧）
		//SetConsoleCursorPosition用于设置输出frame：（显示帧数）的位置
		SetConsoleCursorPosition(MyHandle, { 0,0 });
		cout << "frame：" << i;

		//计算fps(读取时的fps)
		if (GetTickCount64() - start >= 1000) {
			cout << "  fps：" << i - framediff;
			framediff = i + 1;
			start = GetTickCount64();
		}
	}
	Frames = allframe;
}

void Player::PlayVideo()
{
	//清屏
	system("cls");

	//定义当前帧数
	int framediff = 0, totalFrameNum = FramesNum, start = 0,startDisplay=0,
		span = round(1000/(FrameRate*speed))-2,delay=0;
	_CONSOLE_CURSOR_INFO cf = { 1,false };
	startDisplay = GetTickCount64();
	for (int i = 0; i < totalFrameNum; i++) {
		//确保每一帧的显示光标都不可见
		
		SetConsoleCursorInfo(MyHandle, &cf);

		//把光标位置定义在（0,0）位置，可以实现重置效果
		SetConsoleCursorPosition(MyHandle, { 0,0 });
		//输出画面
		
		cout << Frames[i] << endl;
		delay = (GetTickCount64() - startDisplay);
		//cout << delay;
		//帧间隔
		if(delay<span)
		Sleep(span-delay);
		//cout << delay  ;


		//展示目前的帧数并计算帧率
		if (GetTickCount64() - start >= 1000) {
			string temp = "frame：" + to_string(i) + "  fps：" + to_string(i - framediff)+" delay:"+to_string(delay);
			SetConsoleTitleA(temp.c_str());
			framediff = i + 1;
			start = GetTickCount64();


		}
		startDisplay = GetTickCount64();
	}
}

void Player::SetPixelSize()
{
	cout << "Please enter the size of the font:" << endl;
	int size;
	cin >> size;
	Myfontsize = size;
	WCHAR myFont[] = TEXT("Arial");
	_CONSOLE_FONT_INFOEX fontinfo = { sizeof(CONSOLE_FONT_INFOEX) ,1,{Myfontsize,Myfontsize},TMPF_FIXED_PITCH,400, *myFont };
	SetCurrentConsoleFontEx(MyHandle, false, &fontinfo);
}
