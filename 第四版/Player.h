#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
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
	Player(HANDLE hd, HWND hwnd, string FileName);
	void GetFrameFromVideo(string FileName);
	void PlayVideo();
	int GetFramesNum();
	double GetFrameRate();
	//下面是与用户交互的部分
	void SetPixelSize();
	void SetPixelSize(int Fontsize);
	void Setdelay();
	void Setdelay(int delay);
	void Output_to_file();
	void Gain_date_from_file();
	void Set_FrameSize(int width,int height);
	int Get_Myfontsize();
	int Get_mydelay();

private:
	vector<string> Frames;
	int FramesNum;
	int FrameWidth;
	int FrameHeight;
	double FrameRate;
	HWND MyHwnd;
	HANDLE MyHandle;
	//下面是与用户交互的部分
	int Mydelay;
	int Myfontsize;

};

#endif // !1
