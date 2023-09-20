/*
	* ���뻷����Windows 11 + Visual Studio 2022 + OpenCV 4.8.0��
*/

#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <io.h>

//����O2�Ż�
#pragma GCC optimize(2)
#pragma G++ optimize(2)

using namespace cv;
using namespace std;

//��ʾ�����ع��
void ShowCursor(bool visible) {
	CONSOLE_CURSOR_INFO cursor_info = { 20, visible };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	return;
}

int main() {
	//����һ�ѱ���
	int mode = 0;
	char ch[256];
	Mat temp;
	FILE* fc = NULL;
	FILE* fp = NULL;
	vector<string> output;
	clock_t begin, end;

	//�����������
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	//��ȡ����͹������
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0,0 };

	//�ر�OpenCV��־
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	//��ȡ�ַ���
	if (fc = fopen("CHAR.txt", "r")) {
		fscanf(fc, "%[^\n]", ch);
	}
	else {
		sprintf_s(ch, " .'-+=x1[hCQW");
	}

	//��ȡ����
	string video_name;
	cout << "��������Ƶ·��: ";
	getline(cin, video_name);
	VideoCapture video;
	video.open(video_name);

	Size out_size;
	cout << "\n�����ַ����ķֱ���: ";
	cin >> out_size.width >> out_size.height;

	int lag;
	cout << "\n�������ӳ�: ";
	cin >> lag;

	char c;
	cout << "\n�Ƿ񱣴��ַ���?[Y/n]";
	cin >> c;
	bool flag = (c == 'Y' || c == 'y');

	if (flag) {
		cout << "\n����������ģʽ: ";
		cin >> mode;

		//����������ļ���
		if (!_access("ASCII_Art", 0)) {
			system("rd /s /q ASCII_Art");
		}
		system("md ASCII_Art");
	}

	//�����ı��ĵ�
	if (mode == 2 ||  mode == 3) {
		fp = fopen("ASCII_Art/0.txt", "w");
	}

	//����
	system("cls");

	//���ع�겢������ʱ��
	ShowCursor(0);
	begin = clock();

	for (int n = 1; n <= video.get(CAP_PROP_FRAME_COUNT); ++n) {
		//��ȡÿһ֡ͼƬ������תGRAY��ɫ������
		video.read(temp);
		cvtColor(temp, temp, COLOR_BGR2GRAY);
		resize(temp, temp, out_size, 0, 0, IMREAD_GRAYSCALE);

		//����ÿһ֡ͼƬ���ַ���
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

		//�����ַ�����
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

		//���������Ϣ��n%17��Ϊ�˼���������������������
		if (n % 17 == 0) {
			SetConsoleCursorPosition(hOut, pos);	//�������������ͬ
			cout << "������...\t" << n << "/" << video.get(CAP_PROP_FRAME_COUNT) << "    ";
			cout << n / video.get(CAP_PROP_FRAME_COUNT) * 100 << "%";
		}
	}

	//������ʱ������ʾ���
	end = clock();
	ShowCursor(1);

	//��������ӡ��Ϣ
	system("cls");
	cout << "\n�������� " << video.get(CAP_PROP_FRAME_COUNT) << "���ַ���\n";
	cout << "��ʱ " << (double)(end - begin) / CLOCKS_PER_SEC << "s\n";
	cout << "ƽ���ٶ�Ϊ " << video.get(CAP_PROP_FRAME_COUNT) / (int)((double)(end - begin) / CLOCKS_PER_SEC) << "֡/s\n";

	cout << "\n�������������";
	_getch();

	//���ع�겢������ʱ��
	ShowCursor(0);
	begin = clock();

	//��ӡ�ַ���
	for (int i = 0; i < video.get(CAP_PROP_FRAME_COUNT);) {
		if (lag < 0) {
			//ԭ�ٴ�ӡ�ַ���
			i = (double)(clock() - begin) / (1000 / video.get(CAP_PROP_FPS));
			SetConsoleCursorPosition(hOut, pos);
			cout << output[i];
		}
		else {
			//ָ���ٶȴ�ӡ�ַ���
			SetConsoleCursorPosition(hOut, pos);
			cout << output[i++];
			waitKey(lag);
		}
	}

	//������ʱ������ʾ���
	end = clock();
	ShowCursor(1);

	//��ӡ��Ϣ
	cout << "�������� " << video.get(CAP_PROP_FRAME_COUNT) << "���ַ���\n";
	cout << "��ʱ " << (double)(end - begin) / CLOCKS_PER_SEC << "s\n";
	cout << "ƽ���ٶ�Ϊ " << video.get(CAP_PROP_FRAME_COUNT) / (int)((double)(end - begin) / CLOCKS_PER_SEC) << "֡/s\n";

	//���ŵ��˳�����
	cout << "\n����������رմ���";
	_getch();

	return 0;
}