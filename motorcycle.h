#pragma once

#include <opencv2\opencv.hpp>
#include <opencv2\video\background_segm.hpp>
#include "bkgImage.h"
#include "blob.h"
#include "ShowVideo.h"
#include "helmetDetection.h"
#include "LicensePlate.h"
#include "mriaDB.h"

//MHL ���� �ɼ�
#define BKGIMG        1 //����̹���
#define MOTORCYCLE    2 //�������
#define HELMET        4 //������
#define LICENSEPLATE  8 //��ȣ��
#define ALL          15 //����̹���, �������, ������, ��ȣ�� ���

using namespace std;
using namespace cv;

class Motorcycle
{
private:
	VideoCapture capture; //�Է� ����
	string file;          //�Է� ���� �̸�
	stringstream bkg_name;//��� ���� �̸�
	Mat frame;            //������ ������ ����
	Mat bkgImage;         //�ε�� ����̹���
	Mat grayImage;        //���ó�� ����
	CBlobLabeling label;  //���̺� ��� ����
	BkgImage bkgImg;      //��� �̹��� ������ ���� ��ü
	stringstream f_name;  //���� �� ������ ������ �̸�
public:
	Motorcycle() {}          //�Ű����� ���� ������
	Motorcycle(string file); //���� �̸��� �Űܺ��� �޴� ������

public:
	void isMHLDetection();                 //������� ��� ��ȣ�� ���� ���� �Լ�
	void isMHLDetection(int option);       //������� ��� ��ȣ�� �Ϻκ� ���� �Լ�
public:
	int  isMotorcycleDetetion(Mat motocycle, Mat diffImage); //������� ����
public:
	Mat getBackPicture()    { return bkgImage;}  //��� �̹��� get �޼ҵ�
	Mat getGrayImage()      { return grayImage; }//�׷��� �̹��� get �޼ҵ�
	string getFile()        { return file; }     //���� �̸� get �޼ҵ�
	CBlobLabeling getLabel(){ return label; }    //���̺� ���� get �޼ҵ�

	void setFile(string file) { this->file = file; } // ���� �̸� set �޼ҵ�
};
