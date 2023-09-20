/*
	* 编译环境：Windows 11 + Visual Studio 2022 + OpenCV 4.8.0。
*/

#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <io.h>

//开启O2优化
#pragma GCC optimize(2)
#pragma G++ optimize(2)

using namespace cv;
using namespace std;

//显示或隐藏光标
void ShowCursor(bool visible) {
	CONSOLE_CURSOR_INFO cursor_info = { 20, visible };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	return;
}

int main() {
	//定义一堆变量
	int mode = 0;
	char ch[256];
	Mat temp;
	FILE* fc = NULL;
	FILE* fp = NULL;
	vector<string> output;
	clock_t begin, end;

	//输入输出加速
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	//获取句柄和光标坐标
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0,0 };

	//关闭OpenCV日志
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	//读取字符集
	if (fc = fopen("CHAR.txt", "r")) {
		fscanf(fc, "%[^\n]", ch);
	}
	else {
		sprintf_s(ch, " .'-+=x1[hCQW");
	}

	//获取数据
	string video_name;
	cout << "请输入视频路径: ";
	getline(cin, video_name);
	VideoCapture video;
	video.open(video_name);

	Size out_size;
	cout << "\n生成字符画的分辨率: ";
	cin >> out_size.width >> out_size.height;

	int lag;
	cout << "\n请输入延迟: ";
	cin >> lag;

	char c;
	cout << "\n是否保存字符画?[Y/n]";
	cin >> c;
	bool flag = (c == 'Y' || c == 'y');

	if (flag) {
		cout << "\n请输入生成模式: ";
		cin >> mode;

		//创建或更新文件夹
		if (!_access("ASCII_Art", 0)) {
			system("rd /s /q ASCII_Art");
		}
		system("md ASCII_Art");
	}

	//创建文本文档
	if (mode == 2 ||  mode == 3) {
		fp = fopen("ASCII_Art/0.txt", "w");
	}

	//清屏
	system("cls");

	//隐藏光标并开启计时器
	ShowCursor(0);
	begin = clock();

	for (int n = 1; n <= video.get(CAP_PROP_FRAME_COUNT); ++n) {
		//获取每一帧图片并将其转GRAY颜色、缩放
		video.read(temp);
		cvtColor(temp, temp, COLOR_BGR2GRAY);
		resize(temp, temp, out_size, 0, 0, IMREAD_GRAYSCALE);

		//生成每一帧图片的字符画
		string s = "";

		for (int i = 0; i < temp.rows; ++i) {
			for (int j = 0; j < temp.cols; ++j) {
				char tmpc = ch[temp.at<uchar>(i, j) * strlen(ch) / 256];
				s += tmpc;
				if (mode == 3) {
					fprintf(fp, "%c", tmpc);
				}
			}
			s += '\n';
		}

		//保存字符动画
		output.push_back(s);
		if (mode == 1) {
			char text_name[32];
			sprintf_s(text_name, "ASCII_Art/%d.txt", n);
			fp = fopen(text_name, "w");
			fprintf(fp, "%s\n", s.c_str());
			fclose(fp);
		}
		else if (mode == 2) {
			fprintf(fp, "%s", s.c_str());
		}
		else if (mode == 3) {
			fprintf(fp, "\n");
		}

		//输出加载信息，n%17是为了减少输出次数来给程序加速
		if (n % 17 == 0) {
			SetConsoleCursorPosition(hOut, pos);	//输出不清屏，下同
			cout << "加载中...\t" << n << "/" << video.get(CAP_PROP_FRAME_COUNT) << "    ";
			cout << n / video.get(CAP_PROP_FRAME_COUNT) * 100 << "%";
		}
	}

	//结束计时器并显示光标
	end = clock();
	ShowCursor(1);

	//清屏并打印信息
	system("cls");
	cout << "\n共生成了 " << video.get(CAP_PROP_FRAME_COUNT) << "张字符画\n";
	cout << "耗时 " << (double)(end - begin) / CLOCKS_PER_SEC << "s\n";
	cout << "平均速度为 " << video.get(CAP_PROP_FRAME_COUNT) / (int)((double)(end - begin) / CLOCKS_PER_SEC) << "帧/s\n";

	cout << "\n按下任意键播放";
	_getch();

	//隐藏光标并开启计时器
	ShowCursor(0);
	begin = clock();

	//打印字符画
	for (int i = 0; i < video.get(CAP_PROP_FRAME_COUNT);) {
		if (lag < 0) {
			//原速打印字符画
			i = (double)(clock() - begin) / (1000 / video.get(CAP_PROP_FPS));
			SetConsoleCursorPosition(hOut, pos);
			cout << output[i];
		}
		else {
			//指定速度打印字符画
			SetConsoleCursorPosition(hOut, pos);
			cout << output[i++];
			waitKey(lag);
		}
	}

	//结束计时器并显示光标
	end = clock();
	ShowCursor(1);

	//打印信息
	cout << "共播放了 " << video.get(CAP_PROP_FRAME_COUNT) << "张字符画\n";
	cout << "耗时 " << (double)(end - begin) / CLOCKS_PER_SEC << "s\n";
	cout << "平均速度为 " << video.get(CAP_PROP_FRAME_COUNT) / (int)((double)(end - begin) / CLOCKS_PER_SEC) << "帧/s\n";

	//优雅地退出程序
	cout << "\n按下任意键关闭窗口";
	_getch();

	return 0;
}