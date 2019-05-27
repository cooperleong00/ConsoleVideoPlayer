#include "pch.h"
#include "Player.h"

using namespace std;
using namespace cv;

int main()
{


	//取消cin与stdin的同步，提升程序效率
	ios::sync_with_stdio(false);
	Player py;
	//用户输入cmd 
	//string cmd;
	//cout << "请输入视频文件地址 (-视频倍速，0.5-2.0，默认为1.0)";
	//getline(cin, cmd);
	py.SetStdWindow(GetStdHandle(STD_OUTPUT_HANDLE), GetForegroundWindow());
	//py.SetPlayerConfig(cmd);

	//Set设置是包括很多设置的
	py.Set();
	py.GetFrameFromVideo();
	
	
	py.SetWindowConfig();
	//py.SetWindowConfig();
	py.PlayVideo();

	return 0;
}