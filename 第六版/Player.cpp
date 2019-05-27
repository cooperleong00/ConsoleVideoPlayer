#include "pch.h"
#include "Player.h"
using namespace std;
using namespace cv;

//构造函数，初始化赋值
Player::Player()
{
	FramesNum = 0;
	FrameWidth = 800;
	FrameHeight = 450;
	speed = 1.0;
	FrameRate = 30.0;
}


//得到句柄
void Player::SetStdWindow(HANDLE hd, HWND hwnd)
{
	MyHandle = hd;
	MyHwnd = hwnd;
}


//将数据输出到文件，如果没有，就创建一个
void Player::Output_to_file()
{
	ofstream file_txt;
	file_txt.open("set.txt", std::ios::out);
	file_txt << Myfontsize << endl;
	file_txt << speed << endl;
	file_txt << FrameHeight << endl;
	file_txt << FrameWidth << endl;
	file_txt << file << endl;
	file_txt.close();
}

//从文件中获取储存的数据
void Player::Gain_date_from_file()
{
	const int Number_of_int_data = 4;
	//利用数组存储数据
	int data[Number_of_int_data];
	ifstream file_txt;
	try {
		file_txt.open("set.txt");
	}
	catch (Exception) {
		cout << "找不到set.txt文件，请重新运行本程序并进行初始化" << endl;
		return;
	}
	//读取所有数据
	for (int i = 0; i<Number_of_int_data; i++) {
		file_txt >> data[i];
	}
	string file_info;
	file_txt>>file_info;

	//将值赋给player的量
	Myfontsize = data[0];
	speed = data[1];
	FrameHeight = data[2];
	FrameWidth = data[3];

	file = file_info;
	//这里不清屏，可以在读取每一帧的时候有个信息显示
	cout << endl;
	cout << "文件信息：" << file << endl;
	cout << "字体大小:" << Myfontsize << endl;
	cout << "播放速度:" << speed << endl;
	cout << "视频高度：" << FrameHeight << endl;
	cout << "视频宽度：" << FrameWidth << endl;
	
	file_txt.close();

}

//得到字体大小
int Player::Get_Myfontsize()
{
	return Myfontsize;
}

//整体的设置
void Player::Set()
{
	double key = 1;
	cout << "是否需要进行初始化？输入1进行初始化并进行设置，否则将使用上一次的设置" << endl;
	cout << "注意，如果是第一次使用该程序，必须初始化！" << endl;
	cin >> key;
	system("cls");
	if (key == 1) {
		Input_file();
		Set_speed();
		SetPixelSize();
		Set_frame_width();
		Set_frame_height();
		Output_to_file();
	}
	else {
		Gain_date_from_file();
	}

}

//输入文件信息
void Player::Input_file()
{
	cout << "第一步，请输入文件路径及文件名：" << endl;
	cout << "提示：最简单的方式是将视频放在同一路径下然后直接输入文件名（使用相对路径）" << endl;
	cout << "否则，需要您输入完整的路径（绝对路径）" << endl;
	cout << "现在请输入文件路径及文件名：";
	cin >> file;
	system("cls");

}

//得到文件信息
string Player::Get_file_info()
{
	return file;
}

//设置播放速度
void Player::Set_speed()
{
	double sp;
	cout << "请输入视频的播放速度：（请输入0.5-2.0的一个数字）" << endl;
	cin >> sp;
	while (sp > 2 || sp < 0.5) {
		cout << "输入错误，请重新输入：";
		cin >> sp;
	}
	speed = sp;
}

//设置播放速度（带参数）
void Player::Set_speed(double sp)
{
	speed = sp;
}

//返回播放速度
double Player::Get_speed()
{
	return speed;
}

//设置每一帧的宽度
void Player::Set_frame_width()
{
	int w;
	cout << "请输入您需要的视频宽度：";
	cin >> w;
	cout << endl;
	FrameWidth = w;
}

//设置每一帧的高度
void Player::Set_frame_height()
{
	int h;
	cout << "请输入您需要的视频高度：";
	cin >> h;
	cout << endl;
	FrameHeight = h;
	system("cls");
}

//设置每一帧的宽度（带参数）
void Player::Set_frame_width(int w)
{
	FrameWidth = w;
}

//设置每一帧的高度（带参数）
void Player::Set_frame_height(int h)
{
	FrameHeight = h;
}

//返回帧的宽度
int Player::Get_frame_width()
{
	return FrameWidth;
}

//返回帧的高度
int Player::Get_frame_height()
{
	return FrameHeight;
}

//设置控制台信息（全屏）
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

//从视频中获取每一帧，并且处理，最后存储都字符串数组中
void Player::GetFrameFromVideo()
{
	//通过文件名输入视频（绝对路径或相对路径）
	VideoCapture vc;
	try {
		vc.open(file);
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


	//将光标设为不可见,1表示光标厚度，false表示不可见
	_CONSOLE_CURSOR_INFO cf = { 1,false };


	//使用vector数组来储存每一帧
	//+3是为了冗余
	vector<string> allframe(FramesNum + 3);

	//记录开始时间
	double start = GetTickCount64();

	//计数用
	int framediff = 0, j, k;

	//定义存储一帧的字符串
	string output = "";

	//定义一个二值化字符串
	string sign[2] = { " ","#" };

	//CAP_PROP_POS_AVI_RATIO是视频相对位置,0~1的一个值
	//这样写一样是为了冗余 
	//这整个for循环用于读取视频每一帧，并存放到数组里。
	for (int i = 0; i < FramesNum && vc.get(CAP_PROP_POS_AVI_RATIO) < (1 - 10E-6); i++) {
		Mat frameGray, frameOut;
		//将光标设为不可见
		SetConsoleCursorInfo(MyHandle, &cf);

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
		cout << "当前读取帧数：" << i;

		//计算fps(读取时的fps)
		if (GetTickCount64() - start >= 1000) {
			cout << "  fps：" << i - framediff;
			framediff = i + 1;
			start = GetTickCount64();
		}
	}
	Frames = allframe;
}

//使用锁帧算法进行放映，原理：每秒划分为二十五段，如果播放完一帧还有时间，则暂停播放直至下一段
void Player::PlayVideo()
{
	//清屏
	system("cls");

	//应用字体设置
	SetPixelSize(Get_Myfontsize());

	//全屏
	SetWindowConfig();

	//定义当前帧数
	int framediff = 0, totalFrameNum = FramesNum, start = 0,startDisplay=0,
		span = round(1000/(FrameRate*speed))-2,delay=0;

	//光标控制，设定光标大小为1，不可见
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
		//帧间隔
		if(delay<span)
		Sleep(span-delay);


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

//设置字体大小（即像素点）
void Player::SetPixelSize()
{
	cout << "现在请输入字体大小：" << endl;
	int size;
	cin >> size;
	Myfontsize = size;
	system("cls");
}

//设置字体（像素点）大小（带参数）
void Player::SetPixelSize(int size)
{
	Myfontsize = size;
	WCHAR myFont[] = TEXT("Arial");
	_CONSOLE_FONT_INFOEX fontinfo = { sizeof(CONSOLE_FONT_INFOEX) ,
	1,
	{Myfontsize,Myfontsize},
	TMPF_FIXED_PITCH,
	400,
	*myFont };
	SetCurrentConsoleFontEx(MyHandle, false, &fontinfo);
}
