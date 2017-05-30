#pragma once

#include "opencv2\opencv.hpp"
#include <iostream>
#include <ctime>
#include "blob.h"

using namespace std;
using namespace cv;

#define LICENSEPLATE  8 //��ȣ�� ���� ������ ���ϰ�

class LicensePlate
{
private:
	CBlobLabeling label;
	Mat bin;
	Mat imgRoi;
	string licenseNum;
	string type;
	stringstream f_name;
public:
	LicensePlate() { licenseNum = ""; type = ".jpg"; } //Ÿ�� �� licenseNum �ʱ�ȭ�ϴ� �Ű������� ���� ������
	int isLicensePlateDetection(Mat frame);            //�Է� ���� ��ȣ�� ������ ��ȣ�� �����ϴ� ���
	void qSort(Mat data[], int numX[], int l, int r);  //x��ǥ�� �������� mat�� �������ϴ� ���
	string findNumber(Mat number);                     //���� ����� ���ڸ� ���ø� ��Ī�� �������� �ִ밪 ���� ã�� ���
public:
	double getRotationAngle(const IplImage* src);      //���� �� ���� ȸ������ ���ϴ� ���
	void rotateImage(const IplImage* src, IplImage* dst, double degree); //ȸ������ ���� �̹����� ȸ����Ű�� ���
public:
	CBlobLabeling getLabel() { return label; }
	Mat getBin()             { return  bin; }
	Mat getImgRoi()          { return imgRoi; }
	string getNumber()       { return licenseNum; }
	string get_name()        { return f_name.str();}
};