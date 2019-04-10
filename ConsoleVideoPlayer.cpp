#include "pch.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <windows.h>
#include <stdlib.h>
using namespace std;
using namespace cv;
int main()
{
	ios::sync_with_stdio(false);
	//Get the video by file name
	//通过文件名输入视频（绝对路径）
	VideoCapture vc("0.mp4");

	//The unchanged frame from video
	//声明将要预处理的帧
	Mat frameRaw;

	//Get the totoal amount of frames
	//获得视频总帧数
	int totalFrameNum = round(vc.get(CAP_PROP_FRAME_COUNT));

	//Get console handle to use the functions of it
	//得到控制台句柄，以使用控制台函数
	//大概是说明当前用的控制台是哪个
	HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);

	//Set the console cursor invisible for a better performance
	//将光标设为不可见
	_CONSOLE_CURSOR_INFO cf = { 1,false };
	

	//Use a vector to stored all the preprocess  frames 
	vector<Mat> allframe(totalFrameNum + 3);

	double start = GetTickCount();
	int framediff = 0;
	//Grab and preprocess all frames 
	for (int i = 0; i < totalFrameNum&&vc.get(CAP_PROP_POS_AVI_RATIO) < (1 - 10E-6); i++) {
		//Set the console cursor invisible for a better performance
		//将光标设为不可见
		SetConsoleCursorInfo(hd, &cf);
		//the frame changed in gray,and the one changed in size to match the console
		Mat frameGray, frameOut;
		//Grab the next frame from video
		//获得下一帧，如果还未获得过，则获取第一帧
		vc >> frameRaw;

		//Change the RGB color to gray
		//将当前RGB帧转换为灰度图像
		cvtColor(frameRaw, frameGray, 7);

		//Change the size of the frame
		//缩小尺寸
		resize(frameGray, frameOut, Size(32, 24));

		//Stored it in a vector
		allframe[i] = frameOut;

		//Show the number of the current processing frame
		//提示当前处理到第几个帧了
		SetConsoleCursorPosition(hd, {0,30});
		cout << "frame：" << i;
		//计算fps
		if (GetTickCount() - start >= 1000) {
			cout << "  fps：" << i- framediff;
			framediff = i+1;
			start = GetTickCount();
		}
	}
	//Clear console
	//清屏
	system("cls");
	framediff = 0;
	for (int i = 0; i < totalFrameNum; i++) {
		//Make sure the cursor is invisible in every frame
		//确保每一帧的显示光标都不可见
		SetConsoleCursorInfo(hd, &cf);

		//SetConsoleCursorPosition(hd, {0,0});
		for (int j = 0; j < 24; j++) {
			for (int k = 0; k < 32; k++) {
				//Set the current position of the displaying pixel
				//2*k 增加同行像素间距，否则高宽比不合适
				//使用设置光标位置的方法比直接putchar帧率高4-5
				COORD cd = { 2 * k,j };
				SetConsoleCursorPosition(hd, cd);
				//二值化显示像素，灰度大于127即显示
				if (allframe[i].at<bool>(j,k)>127) {
					putchar('#');
					//putchar(' ');
				}
				else {
					putchar(' ');
					//putchar(' ');
				}
			}
			//putchar('\n');
		}
		//Show the number of the current playing frame
		SetConsoleCursorPosition(hd, {0,40});

		cout << "frame：" << i;
		if (GetTickCount() - start >= 1000) {
			cout << "  fps：" << i - framediff;
			framediff = i + 1;
			start = GetTickCount();
		}
	}

}
