#include "pch.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <windows.h>
#include <stdlib.h>
#include <string>
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
	//调用设置cmd的函数
	Setcmd();

	//取消cin与stdin的同步，提升程序效率
	ios::sync_with_stdio(false);

	//Get the video by file name
	//通过文件名输入视频（绝对路径或相对路径）
	VideoCapture vc("0.mp4");

	//The unchanged frame from video
	//声明将要预处理的帧
	Mat frameRaw;

	//Get the totoal amount of frames
	//获得视频总帧数
	//CAP_PROP_FRAME_COUNT是opencv宏定义的抓取帧数总量
	int totalFrameNum = round(vc.get(CAP_PROP_FRAME_COUNT));

	//Get console handle to use the functions of it
	//得到控制台句柄，以使用控制台函数
	//说明当前用的控制台是哪个
	HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);

	bool SetConsoleFont(HANDLE, DWORD);

	//Set the console cursor invisible for a better performance
	//将光标设为不可见,1表示光标厚度，false表示不可见
	_CONSOLE_CURSOR_INFO cf = { 1,false };


	//Use a vector to stored all the preprocess  frames 
	//使用vector数组来储存每一帧
	//+3是为了冗余
	vector<Mat> allframe(totalFrameNum + 3);

	//记录开始时间
	double start = GetTickCount();

	//计数用
	int framediff = 0;

	//Grab and preprocess all frames 
	//CAP_PROP_POS_AVI_RATIO是视频相对位置,0~1的一个值
	//这样写一样是为了冗余 

	//这整个for循环用于读取视频每一帧，并存放到数组里。
	for (int i = 0; i < totalFrameNum&&vc.get(CAP_PROP_POS_AVI_RATIO) < (1 - 10E-6); i++) {

		//Set the console cursor invisible for a better performance
		//将光标设为不可见
		SetConsoleCursorInfo(hd, &cf);

		//the frame changed in gray,and the one changed in size to match the console
		//定义灰度值矩阵，和输出矩阵

		Mat frameGray, frameOut;
		//Grab the next frame from video
		//获得下一帧，如果还未获得过，则获取第一帧
		vc >> frameRaw;

		//Change the RGB color to gray
		//将当前RGB帧转换为灰度图像
		cvtColor(frameRaw, frameGray, 7);

		//Change the size of the frame
		//缩小尺寸
		resize(frameGray, frameOut, Size(288 , 162));

		//Stored it in a vector
		//存储进数组
		allframe[i] = frameOut;

		//Show the number of the current processing frame
		//提示当前处理到第几个帧了（读取时的第几帧）
		//SetConsoleCursorPosition用于设置输出frame：（显示帧数）的位置
		SetConsoleCursorPosition(hd, { 0,40 });
		cout << "frame：" << i;

		//计算fps(读取时的fps)
		if (GetTickCount() - start >= 1000) {
			cout << "  fps：" << i - framediff;
			framediff = i + 1;
			start = GetTickCount();
		}
	}
	//Clear console
	//清屏
	system("cls");

	//定义当前帧数
	framediff = 0;
	for (int i = 0; i < totalFrameNum; i++) {
		//Make sure the cursor is invisible in every frame
		//确保每一帧的显示光标都不可见
		SetConsoleCursorInfo(hd, &cf);

		//把光标位置定义在（0,0）位置，可以实现重置效果
		SetConsoleCursorPosition(hd, { 0,0 });

		//定义输出的字符串
		string output = "";
		
		//获取输出的字符串
		//第一个for循环是行遍历，第二个for循环是列遍历
		for (int j = 0; j < allframe[i].rows; j++) {
			for (int k = 0; k < allframe[i].cols; k++) {

				//二值化显示像素，灰度大于127即显示
				if (allframe[i].at<bool>(j, k) > 127) {
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
		//输出获取的字符串

		//Set the current position of the displaying pixel
		//2*k 增加同行像素间距，否则高宽比不合适
		//使用设置光标位置的方法比直接putchar帧率高4-5
		COORD cd = { allframe[i].cols*2,allframe[i].rows };
		SetConsoleCursorPosition(hd, cd);
		cout << output << endl;
		//休眠	
		//Sleep(10);


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
