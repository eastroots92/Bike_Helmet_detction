#pragma once 
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define HELMET 4 //��� �������� ����� ���ϰ�

#define CASCADENAME "cascade.xml" //cascade �з��� ���� xml ����

class Helmet{
private:
	Mat helmetArea;
	Mat gray;
	Mat bkgImage;
	Rect helmetRoi;
private:
	CascadeClassifier helmet;
public:
	Helmet(){ } //�Ű������� ���� ������
	Helmet(Mat helmetArea, Mat bkgImage, Rect helmetRoI); //��俵��, ����̹���, ���������� ���ڷ� �޴� ������
public:
	int isHelmetDetect();      //��� ���� ������ �Ǵ��ϴ� ���
	int isHelmetDetect(Mat helmetArea, Mat bkgImage, Rect helmetRoI); 
	//��俵��, ����̹���, ���������� ���ڷ� �޾� ���� ������ �Ǵ��ϴ� ���
public:
	void setHelmetArea(Mat helmetArea){ this->helmetArea = helmetArea.clone(); }
	void setBkgImage(Mat bkgImage)    { this->bkgImage = bkgImage.clone(); }
	void setHemetRoi(Rect helmetRoi)  { this->helmetRoi = helmetRoi; }
};
