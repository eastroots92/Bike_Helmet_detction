#include "helmetDetection.h"


//��� ����, ����̹���, ��� rect�� ���ڷ� �޴� ������
Helmet::Helmet(Mat helmetArea, Mat bkgImage, Rect helmetRoI)
{
	this->helmetArea = helmetArea.clone();
	this->bkgImage = bkgImage.clone();
	this->helmetRoi = helmetRoi;
	cvtColor(helmetArea, gray, CV_RGB2GRAY);
}

/*
������ ������ ���� �Ǵ� ���
1�� �Ǵ� : adaboost�� �̿��� �з��� ���� ��� ���� �Ǵ�
2�� �Ǵ� : ������ �ȼ� ���� 50%�̻��� ũ���� ����� ũ���� �Ǵ�
*/
int Helmet::isHelmetDetect()
{
	if (!helmet.load(CASCADENAME))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return 0;
	}

	int gr_thr = 4;
	double scale_step = 1.1;

	Size min_obj_sz(5, 5);
	Size max_obj_sz(100, 100);

	// run
	vector<Rect> found;
	helmet.detectMultiScale(gray, found, scale_step, gr_thr, 0, min_obj_sz, max_obj_sz); //�з��⸦ ���� �з� ����

	bool isHelmet;

	if ((int)found.size() == 0) // �����ڶ�� ����
		return false;
	else                        //������ �ǽ��� 2�� �Ǻ�
	{
		Rect hmRect(helmetRoi.x, helmetRoi.y, helmetRoi.width, helmetRoi.height); //��俵�� ����

		Mat helmetBkImg = bkgImage(hmRect);           //����̹������� ��� ���� ����

		Mat hmDiffrent = gray.clone();                //��� ���� �̹��� ����

		absdiff(hmDiffrent, helmetBkImg, hmDiffrent); //��� ���� �̹��� - ��� ���� ����̹��� �� �̹��� ����

		threshold(hmDiffrent, hmDiffrent, 10, 255, THRESH_BINARY);  //�Ӱ谪 10 ����ȭ 

		Mat element4(4, 4, CV_8U, Scalar(1));                       //���� ���� 4x4 ����ũ
		Mat element3(3, 3, CV_8U, Scalar(1));                       //�ݱ� ���� 3x3 ����ũ

		morphologyEx(hmDiffrent, hmDiffrent, MORPH_OPEN, element4); //��Ƽ����
		morphologyEx(hmDiffrent, hmDiffrent, MORPH_CLOSE, element3);//�� ���� ä���

		//imshow("������", hmDiffrent);

		int helmetRatio = 0;

		destroyWindow("HelmetDetection");

		for (int i = 0; i < (int)found.size(); i++)
		{
			found[i].x = found[i].x - 5;
			found[i].y = found[i].y - 5;
			found[i].width = found[i].width + 5;
			found[i].height = found[i].height + 5;

			rectangle(helmetArea, found[i], Scalar(0, 255, 0), 2);

			int helmetFix = 0;

			for (int j = found[i].y; j < found[i].y + found[i].width; j++)      //��� �ȼ��� ���ϱ�
				for (int k = found[i].x; k < found[i].x + found[i].height; k++)
					helmetFix += hmDiffrent.at<uchar>(j, k) / 255;

			helmetRatio = (helmetFix * 100) / (found[i].width*found[i].height);

			imshow("HelmetDetection", helmetArea);

			if (helmetRatio >= 50) //������ 50% �̻��̶�� return helmet
				return HELMET;
			else                   //�� �����ڶ�� return false
				return false;
		}
	}
}

/*
������ ������ ���� �Ǵ� ���
1�� �Ǵ� : adaboost�� �̿��� �з��� ���� ��� ���� �Ǵ�
2�� �Ǵ� : ������ �ȼ� ���� 50%�̻��� ũ���� ����� ũ���� �Ǵ�
*/
int Helmet::isHelmetDetect(Mat helmetArea, Mat bkgImage, Rect helmetRoi)
{
	cvtColor(helmetArea, gray, CV_RGB2GRAY);

	if (!helmet.load(CASCADENAME))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return 0;
	}

	int gr_thr = 4;
	double scale_step = 1.1;

	Size min_obj_sz(5, 5);
	Size max_obj_sz(100, 100);

	// run
	vector<Rect> found;
	//�з��⸦ ���� �з� ����
	helmet.detectMultiScale(gray, found, scale_step, gr_thr, 0, min_obj_sz, max_obj_sz); 

	bool isHelmet;

	if ((int)found.size() == 0) // �����ڶ�� ����
		return false;
	else                        //������ �ǽ��� 2�� �Ǻ�
	{
		Rect hmRect(helmetRoi.x, helmetRoi.y, helmetRoi.width, helmetRoi.height); //��俵�� ����

		Mat helmetBkImg = bkgImage(hmRect);           //����̹������� ��� ���� ����

		Mat hmDiffrent = gray.clone();                //��� ���� �̹��� ����

		//��� ���� �̹��� - ��� ���� ����̹��� �� �̹��� ����
		absdiff(hmDiffrent, helmetBkImg, hmDiffrent); 

		threshold(hmDiffrent, hmDiffrent, 10, 255, THRESH_BINARY);  //�Ӱ谪 10 ����ȭ 

		Mat element4(4, 4, CV_8U, Scalar(1));                       //���� ���� 4x4 ����ũ
		Mat element3(3, 3, CV_8U, Scalar(1));                       //�ݱ� ���� 3x3 ����ũ

		morphologyEx(hmDiffrent, hmDiffrent, MORPH_OPEN, element4); //��Ƽ����
		morphologyEx(hmDiffrent, hmDiffrent, MORPH_CLOSE, element3);//�� ���� ä���

		//imshow("������", hmDiffrent);

		int helmetRatio = 0;

		destroyWindow("HelmetDetection");

		for (int i = 0; i < (int)found.size(); i++) 
		{
			found[i].x = found[i].x - 5;
			found[i].y = found[i].y - 5;
			found[i].width = found[i].width + 5;
			found[i].height = found[i].height + 5;
			
			rectangle(helmetArea, found[i], Scalar(0, 255, 0), 2);

			int helmetFix = 0;

			for (int j = found[i].y; j < found[i].y + found[i].width; j++) //��� �ȼ��� ���ϱ�
				for (int k = found[i].x; k < found[i].x + found[i].height; k++)
					helmetFix += hmDiffrent.at<uchar>(j, k) / 255;

			helmetRatio = (helmetFix * 100) / (found[i].width*found[i].height);

			if (helmetRatio >= 50) //������ 50% �̻��̶�� return helmet
				return HELMET;
			else                   //�� �����ڶ�� return false
				return false;

			imshow("HelmetDetection", helmetArea);
		}
	}
}