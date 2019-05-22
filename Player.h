#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <windows.h>
#include <stdlib.h>
#include <string>
using namespace std;
//使用opencv的命名空间
using namespace cv;
class Player {
public:
	Player(HANDLE hd, HWND hwnd, string FileName);
	void GetFrameFromVideo(string FileName);
	void PlayVideo();
	void SetPixelSize(int width,int height);
	int GetFramesNum();
	double GetFrameRate();
private:
	vector<string> Frames;
	int FramesNum;
	int FrameWidth;
	int FrameHeight;
	double FrameRate;
	HWND MyHwnd;
	HANDLE MyHandle;
};

#endif // !1
