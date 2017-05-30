#pragma once

#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\video\background_segm.hpp>


using namespace std;
using namespace cv;

class BkgImage {
private:
	VideoCapture capture; //��� �̹����� ���� ����
	Size size;            //������ ���μ��� ������ ����
	string file;          //���� ������ �̸� ����
	int fps;              //���� ������ �ӵ� ����
	int delay;            //������ ������ ������ ���� �ӵ� ���� 
	int frameNum;         //������ ������ �б� ���� ����
	Mat frame;            //������ ������ ����
	Mat backPicture;      //��� ����̹��� ����
	Mat grayImage;        //��鿵�� �ӽ� ����
	stringstream bkg_name;//��� ���� �̸�
public:
	BkgImage(){ frameNum = 0;}               //�Ű������� ���� ������
	BkgImage(string file);                   //���� ���� �̸��� �Ű������� �޴� ������ 
	BkgImage(string file, string b_name);    //���� ���� �̸�, ����̹��� �̸��� �Ű������� �޴� ������
	void isAcummulateBkgImage();             //��� ��� �̹��� ���ϴ� ���
	void isAcummulateBkgImage(string b_name);//����̹��� �̸��� �Ű������� �޾� ��� ����̹����� ���ϴ� ���
public:
	int    getFps()        { return fps; }
	int    getDelay()      { return delay; }
	int    getFrameNum()   { return frameNum;}
	Mat    getBackPicture(){ return backPicture; }
	Mat    getGrayImage()  { return grayImage; }
	Size   getSize()       { return size; }
	string getFile()       { return file; }

	void setFile(string file) { this->file = file; }
};

