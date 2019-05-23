#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <fstream>
using namespace std;
//使用opencv的命名空间
using namespace cv;
class Player {
public:
	Player();
	void SetPlayerConfig(string cmd);
	void SetStdWindow(HANDLE hd, HWND hwnd);
	void SetWindowConfig();
	void GetFrameFromVideo(string FileName);
	void PlayVideo();

	//设置字体大小（用户交互）
	void SetPixelSize();

	//设置字体大小
	void SetPixelSize(int size);

	//输出数据到文本
	void Output_to_file();

	//从文本中获取数据
	void Gain_date_from_file();

	//得到当前字体大小
	int Get_Myfontsize();

	//用户设定模式
	void Set();

private:
	vector<string> Frames;
	int FramesNum;
	int FrameWidth;
	int FrameHeight;
	double speed;
	double FrameRate;
	HWND MyHwnd;
	HANDLE MyHandle;
	//字体大小
	int Myfontsize;

};

#endif 