#include "pch.h"
#include "Player.h"

Player::Player(HANDLE hd, HWND hwnd, string FileName)
{
	this->MyHwnd = hwnd;
	this->MyHandle = hd;
	GetFrameFromVideo(FileName);

}

void Player::GetFrameFromVideo(string FileName)
{
	//Get the video by file name
	//通过文件名输入视频（绝对路径或相对路径）
	VideoCapture vc(FileName);

	//The unchanged frame from video
	//声明将要预处理的帧
	Mat frameRaw;

	//Get the totoal amount of frames
	//获得视频总帧数
	//CAP_PROP_FRAME_COUNT是opencv宏定义的抓取帧数总量
	int totalFrameNum = FramesNum =  vc.get(CAP_PROP_FRAME_COUNT);

	//获取帧率
	this->FrameRate = vc.get(CAP_PROP_FPS);

	//Get console handle to use the functions of it
	//得到控制台句柄，以使用控制台函数
	//说明当前用的控制台是哪个
	HANDLE temp_hd = MyHandle;

	

	//Set the console cursor invisible for a better performance
	//将光标设为不可见,1表示光标厚度，false表示不可见
	_CONSOLE_CURSOR_INFO cf = { 1,false };


	//Use a vector to stored all the preprocess  frames 
	//使用vector数组来储存每一帧
	//+3是为了冗余
	vector<string> allframe(totalFrameNum + 3);

	//记录开始时间
	double start = GetTickCount();

	//计数用
	int framediff = 0,width = FrameWidth=80,height = FrameHeight = 45,j,k;
	string output = "";

	//Grab and preprocess all frames 
	//CAP_PROP_POS_AVI_RATIO是视频相对位置,0~1的一个值
	//这样写一样是为了冗余 

	//这整个for循环用于读取视频每一帧，并存放到数组里。
	for (int i = 0; i < totalFrameNum&&vc.get(CAP_PROP_POS_AVI_RATIO) < (1 - 10E-6); i++) {
		Mat frameGray, frameOut;
		//Set the console cursor invisible for a better performance
		//将光标设为不可见
		SetConsoleCursorInfo(temp_hd, &cf);

		//the frame changed in gray,and the one changed in size to match the console
		//定义灰度值矩阵，和输出矩阵

		//Grab the next frame from video
		//获得下一帧，如果还未获得过，则获取第一帧
		vc >> frameRaw;

		//Change the RGB color to gray
		//将当前RGB帧转换为灰度图像
		cvtColor(frameRaw, frameGray, 7);

		//Change the size of the frame
		//缩小尺寸
		resize(frameGray, frameOut, Size(FrameWidth, FrameHeight));

		//Stored it in a vector
		//存储进数组
		

		//获取输出的字符串
		//第一个for循环是行遍历，第二个for循环是列遍历
		for (j=0; j < FrameHeight; j++) {
			for (k=0; k < FrameWidth; k++) {

				//二值化显示像素，灰度大于127即显示
				if (frameOut.at<bool>(j, k) > 127) {
					output += "#";
					//putchar(' ');
				}
				else {
					output += " ";
					//putchar(' ');
				}
			}
			output += "\n";
		}
		allframe[i] = output;
		//Show the number of the current processing frame
		//提示当前处理到第几个帧了（读取时的第几帧）
		//SetConsoleCursorPosition用于设置输出frame：（显示帧数）的位置
		SetConsoleCursorPosition(temp_hd, { 0,0 });
		cout << "frame：" << i;

		//计算fps(读取时的fps)
		if (GetTickCount() - start >= 1000) {
			cout << "  fps：" << i - framediff;
			framediff = i + 1;
			start = GetTickCount();
		}
	}
	Frames = allframe;
}

void Player::PlayVideo()
{
	//Clear console
	//清屏
	system("cls");

	//定义当前帧数
	int framediff = 0, totalFrameNum = FramesNum,start = 0;
	_CONSOLE_CURSOR_INFO cf = { 1,false };
	for (int i = 0; i < totalFrameNum; i++) {
		//Make sure the cursor is invisible in every frame
		//确保每一帧的显示光标都不可见
		SetConsoleCursorInfo(MyHandle, &cf);

		//把光标位置定义在（0,0）位置，可以实现重置效果
		SetConsoleCursorPosition(MyHandle, { 0,0 });

		//定义输出的字符串

		//输出获取的字符串

		//Set the current position of the displaying pixel
		//2*k 增加同行像素间距，否则高宽比不合适
		//使用设置光标位置的方法比直接putchar帧率高4-5
		COORD cd = { FrameHeight * 2,FrameWidth };
		SetConsoleCursorPosition(MyHandle, cd);
		cout << Frames[i] << endl;
		//休眠	
		Sleep(Mydelay);


		//Show the number of the current playing frame
		//展示目前的帧数并计算帧率
		cout << "frame：" << i;
		if (GetTickCount() - start >= 1000) {
			cout << "  fps：" << i - framediff;
			framediff = i + 1;
			start = GetTickCount();


		}
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

void Player::SetPixelSize(int FontSize)
{
	Myfontsize = FontSize;
	WCHAR myFont[] = TEXT("Arial");
	_CONSOLE_FONT_INFOEX fontinfo = { sizeof(CONSOLE_FONT_INFOEX) ,1,{Myfontsize,Myfontsize},TMPF_FIXED_PITCH,400, *myFont };
	SetCurrentConsoleFontEx(MyHandle, false, &fontinfo);
}

int Player::GetFramesNum()
{
	return this->FramesNum;
}

double Player::GetFrameRate()
{
	return this->FrameRate;
}

//用户设置延迟
void Player::Setdelay() {
	cout << endl;
	cout << "Please enter the delay of every frame:" << endl;
	int delay;
	cin >> delay;
	Mydelay = delay;
}

void Player::Setdelay(int delay){ 
	Mydelay = delay;
}

//输出数据到文件
void Player::Output_to_file() {
	ofstream file;
	file.open("set.txt", std::ios::out);
	file << Mydelay << endl;
	file << Myfontsize << endl;
	file.close();



}


void Player::Set_FrameSize(int width,int height)
{
	FrameHeight = height;
	FrameWidth = width;

}

int Player::Get_Myfontsize()
{
	return Myfontsize;
}

int Player::Get_mydelay()
{
	return Mydelay;
}

//从文件中读取数据
void Player::Gain_date_from_file() {
	//利用数组存储数据
	char data[10];
	ifstream file("set.txt");
	//读取所有数据
	while (!file.eof()) {
		file.getline(data, 10);
	}
	Mydelay = data[0];
	Myfontsize = data[1];
	
}
