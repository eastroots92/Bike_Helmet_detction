
#include "bkgImage.h"


/* ����̹��� ���� ���� �����̸��� �Ű������� �޴� BkgImage ������
   �Է� file�� path, Ȯ���� ���� �� name ����
   file�� ���� ���ϸ� �κ� ���� bkg_�� ���� bkg_name.jpg ����̹����� �̸���
   bkg_name�� ����
*/
BkgImage::BkgImage(string file)
{
	this->file = file;
	frameNum = 0;
	string name = file.substr(file.find_last_of("/\\") + 1);
	size_t const p(name.find_last_of('.'));
	name = name.substr(0, p);
	bkg_name << "bkg_" << name << ".jpg";
}

/* ����̹��� ���� ���� �����̸�, ����̹��� �̸��� �Ű������� �޴� BkgImage ������
*/
BkgImage::BkgImage(string file,string b_name)
{
	this->file = file;
	frameNum = 0;
	bkg_name << b_name;
}

/* ���� ��� �̹��� ���� ���

���� ���� �� ���� ���� ������� �ӵ��� ���� �� ������ ������ ������ ���� ���Ͽ�
�� ���� ������ �� ������ ���� ���� ����� ���� �� bkg_�Է�����.jpg�� �̸����� ������ �����ϴ� ���

capture : �Է� ���� / size : �Է� ���� ���μ��� ������ / fps ���� frame �ӵ� / delay ó���� ���� ������
sumImage : ������ ��� �̹��� ���� �ӽ� �����
*/
void BkgImage::isAcummulateBkgImage()
{
	capture.open(file);

	if (!capture.isOpened()) {
		cout << "���� ������ ����" << endl;
		return;
	}

	size = Size((int)capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));

	fps = (int)(capture.get(CAP_PROP_FPS));

	delay = 100 / fps;

	Mat sumImage(size, CV_32F, Scalar::all(0));

	//�����б⿡ �����ߴٸ�
	while (capture.isOpened()) {

		capture >> frame;

		if (frame.empty()) break; //������ ��� �о��ٸ� ����

		cvtColor(frame, grayImage, COLOR_BGR2GRAY);

		accumulate(grayImage, sumImage);

		frameNum++;

		int ckey = waitKey(delay); //������
		if (ckey == 27) break;     // 27 = Esc
	}

	sumImage = sumImage / (float)frameNum; //���տ� �����Ӽ��� ���� ��� �̹��� ����

	imwrite(bkg_name.str(), sumImage);     //��� �̹��� ����
}

/* ��� �̹��� �̸��� �Է� ���ڷ� �޴� ���� ��� �̹��� ���� ���

  ���� ���� �� ���� ���� ������� �ӵ��� ���� �� ������ ������ ������ ���� ���Ͽ�
  �� ���� ������ �� ������ ���� ���� ����� ���� �� bkg_�Է�����.jpg�� �̸����� ������ �����ϴ� ���

  capture : �Է� ���� / size : �Է� ���� ���μ��� ������ / fps ���� frame �ӵ� / delay ó���� ���� ������
  sumImage : ������ ��� �̹��� ���� �ӽ� �����
*/
void BkgImage::isAcummulateBkgImage(string b_name)
{
	capture.open(file);

	if (!capture.isOpened()) {
		cout << "���� ������ ����" << endl;
		return;
	}

	bkg_name << b_name;

	size = Size((int)capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));

	fps = (int)(capture.get(CAP_PROP_FPS));

	delay = 100 / fps;

	Mat sumImage(size, CV_32F, Scalar::all(0));

	//�����б⿡ �����ߴٸ� ���ѷ���
	while (capture.isOpened()) {

		capture >> frame;

		if (frame.empty()) break; //������ ��� �о��ٸ� ����

		cvtColor(frame, grayImage, COLOR_BGR2GRAY);

		accumulate(grayImage, sumImage);

		frameNum++;
		int ckey = waitKey(delay); //������
		if (ckey == 27) break;     // 27 = Esc
	}

	sumImage = sumImage / (float)frameNum; //���տ� �����Ӽ��� ���� ��� �̹��� ����

	imwrite(bkg_name.str(), sumImage);     //��� �̹��� ����
}