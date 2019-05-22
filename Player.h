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
	void SetPixelSize(int size);
	void SetWindowConfig();
	void GetFrameFromVideo(string FileName);
	void PlayVideo();
private:
	vector<string> Frames;
	int FramesNum;
	int FrameWidth;
	int FrameHeight;
	double speed;
	double FrameRate;
	HWND MyHwnd;
	HANDLE MyHandle;

};

#endif 