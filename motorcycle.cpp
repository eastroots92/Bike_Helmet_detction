#include "motorcycle.h"

/*
  �Է� file �̸��� �Ű������� �޴� ������
  �Է� file�� path, Ȯ���� ���� �� name ����
  file�� ���� ���ϸ� �κ� ���� bkg_�� ���� bkg_name.jpg ����̹����� �̸��� 
  bkg_name�� ����
  ji
*/
Motorcycle ::Motorcycle(string file)
{
	this->file = file;
	string name = file.substr(file.find_last_of("/\\") + 1);
	size_t const p(name.find_last_of('.'));
	name = name.substr(0, p);
	bkg_name << "bkg_" << name << ".jpg";
}

/* 
 Motorcycle Helmet licenseplate all detect
 ��ó�� : absdiff (����̹��� ����) -> threshold(����ȭ) -> ���⿬��(��Ƽ����) -> �ݱ⿬��(����� ä���)-> ���̺�
          ���̺� �� ��ü ����/���κ� 1.5 �������� ���� �� �߽� ���� center ��ġ�� ���� ����
 ������� ����(isMotorcycleDetetion) ���� -> ��� �������(!isHelmetDetect(helmetArea, bkgImage, helmetRect)) 
 -> ��ȣ�� ���� isLicensePlateDetection(licenseArea);
*/
void Motorcycle::isMHLDetection()
{
	capture.open(file);      //file ���ڿ� �̸��� ���� ����

	if (!capture.isOpened()) //���� ���� ���� ���� �ߴٸ� ����
	{
		cout << "���� ������ ����" << endl;
		return;
	}

	bkgImage = imread(bkg_name.str(), COLOR_BGR2GRAY); //��� ��� �̹��� read

	if (bkgImage.empty())                              //��� �ִٸ� ��� ��� ���ϱ�
	{
		bkgImg.setFile(file);

		bkgImg.isAcummulateBkgImage(bkg_name.str());

		bkgImage = imread(bkg_name.str(), COLOR_BGR2GRAY);
	}

	int fps = (int)(capture.get(CAP_PROP_FPS));//�Է� ������ �ӵ�
	int nTh = 35;                              //����ȭ �Ӱ谪
	int frameNum = 0;                          //������ ��ȣ ����(�� ������ ������ ������ ��꿡 �ʿ�)
	int delay = 500 / fps;                     //������ �ӵ� ������ ���� ������ ����

	Mat frame, diffImage;                      //�����Ӱ� ��� ��� �� �̹��� ����
	Mat element4(4, 4, CV_8U, Scalar(1));      //�������� ���� ����ũ
	Mat element25(25, 25, CV_8U, Scalar(1));   //�������� �ݱ� ����ũ

	//���� ó��
	while (true)
	{
		capture >> frame;

		if (frame.empty()) break;

		cvtColor(frame, grayImage, COLOR_BGR2GRAY);

		//���� ����
		IplImage *rect = &IplImage(grayImage);

		int rWidth = rect->width;	//rect�� ���α���
		int rHeight = rect->height;	//rect�� ���α���

		Rect rt(rWidth * 1 / 6, rHeight * 1 / 2, rWidth * 2 / 3, rHeight * 1 / 50);
		CvPoint pt1 = cvPoint(rWidth * 1 / 6, rHeight * 1 / 2);
		CvPoint pt2 = cvPoint(rWidth * 5 / 6, rHeight * 26 / 50);
		//Rect rt(���� ������, ���� ������, �ʺ�, ����

		rectangle(grayImage, rt, Scalar(255, 0, 0), 2);            //���� ���� ǥ��

		absdiff(grayImage, bkgImage, diffImage);                   //	�Է� ������(grayImage) - ��� ��� �̹���(bkgImage) = �� ���� (diffImage)

		threshold(diffImage, diffImage, nTh, 255, THRESH_BINARY);  //�Ӱ�ġ ���� ������ 255�� ��ȯ ����ȭ

		morphologyEx(diffImage, diffImage, MORPH_OPEN, element4);   //��Ƽ ���� 4x4 ����Ʈ �������� ���� ����

		morphologyEx(diffImage, diffImage, MORPH_CLOSE, element25); //����� ä��� 25x25 ����Ʈ �������� �ݱ� ����

		IplImage *trans = &IplImage(diffImage);                     //��� ���̺��� ���� mat(c++) -> IplImage(c) ��ȯ

		label.SetParam(trans, 8000);       //8000 ������ ũ�� ����
		label.DoLabeling();                //���̺� ����
		label.BlobWidthHeightSmallRatioConstraint(1.5, VERTICAL);   //���� ���κ� 1.5 �̸� ����

		IplImage *cycle = &IplImage(diffImage);

		for (int i = 0; i < label.m_nBlobs; i++)
		{
			label.m_recBlobs[i].y = label.m_recBlobs[i].y - 10;
			label.m_recBlobs[i].height = label.m_recBlobs[i].height + 10;
			/*rectangle(grayImage, label.m_recBlobs[i], Scalar(255), 2);*/
			cvSetImageROI(cycle, label.m_recBlobs[i]);
			
			label.BlobBigSizeConstraint(4, 6);

			int cWidth = label.m_recBlobs[i].width;
			int cHeight = label.m_recBlobs[i].height;

			Mat motoCycle = cvarrToMat(cycle);

			CvPoint cPt = cvPoint(label.m_recBlobs[i].x, label.m_recBlobs[i].y);

			CvPoint cCenter = cvPoint(cPt.x + (cWidth / 2), cPt.y + (cHeight / 2));

			if ((cCenter.x > pt1.x) && (cCenter.x < pt2.x) && (cCenter.y > pt1.y) && (cCenter.y < pt2.y))
			{
				int result = isMotorcycleDetetion(motoCycle, diffImage);

				if (result) //������� ���� �����ߴٸ�
				{
					imwrite("correctCycle.png", motoCycle);
					//��� ���� ����
					Rect helmetRect(label.m_recBlobs[i].x, label.m_recBlobs[i].y, label.m_recBlobs[i].width, label.m_recBlobs[i].height / 7 * 2);
	
					Mat helmetArea = frame(helmetRect);

					Helmet hdect;

					//true�� ��ȣ�� ����
					if (!hdect.isHelmetDetect(helmetArea, bkgImage, helmetRect))
					{
						destroyWindow("licensePlateArea");
						Rect licensePlate(label.m_recBlobs[i].x, label.m_recBlobs[i].y + label.m_recBlobs[i].height / 7 * 5, label.m_recBlobs[i].width,
													label.m_recBlobs[i].height / 7 * 2);
						Mat licenseArea = frame(licensePlate);
						imshow("licensePlateArea", licenseArea);

						time_t curr;
						struct tm *d;
						curr = time(NULL);
						d = localtime(&curr);

						LicensePlate licenseDetcet;

						if (licenseDetcet.isLicensePlateDetection(licenseArea))
						{
							f_name.str(std::string()); //�����ִ� string �����
							f_name.clear();

							//frame ����
							f_name << d->tm_year + 1900 << "." << d->tm_mon + 1 << "." << d->tm_mday << "_" << d->tm_hour << "." 
										<< d->tm_min << "." << d->tm_sec << "_" << licenseDetcet.getNumber() << ".jpg";
							Mat store = frame(label.m_recBlobs[i]);
							imwrite(f_name.str(), store);

							//DB ����
							MHLdata db;
							db.isInsertQuery(db.isSelectQuery(), f_name.str(), file, "", licenseDetcet.getNumber()); 
						}
					}
				}
			}
			cvResetImageROI(cycle);
		}
		frameNum++;

		showVideo("grayImage", grayImage); //�⺻ ��鿵�� ��ü ���� ��� ǥ�� ���
		showVideo("diffImage", diffImage); //�� ���� ����ȭ ���

		int ckey = waitKey(delay);         //Ű �Է� ��� �ð�
		if (ckey == 27) break;
	}
}

/* 
  Motorcycle Helmet licenseplate select detect
   ��ó�� : absdiff (����̹��� ����) -> threshold(����ȭ) -> ���⿬��(��Ƽ����) -> �ݱ⿬��(����� ä���)-> ���̺�
  ���̺� �� ��ü ����/���κ� 1.5 �������� ���� �� �߽� ���� center ��ġ�� ���� ����
  ���õǾ��� �ɼǿ� ���� ���ϴ� �κ� ����
  option : MOTORCYCLE  -> ������� ����
           HELMET      -> ������ �������� ����
	       LICENSEPLATE-> ��ȣ�� ����
*/
void Motorcycle::isMHLDetection(int option)
{
	capture.open(file);      //file ���ڿ� �̸��� ���� ����

	if (!capture.isOpened()) //���� ���� ���� ���� �ߴٸ� ����
	{
		cout << "���� ������ ����" << endl;
		return;
	}

	if (option & BKGIMG)
	{
		bkgImg.setFile(file);

		bkgImg.isAcummulateBkgImage(bkg_name.str());

		bkgImage = imread(bkg_name.str(), COLOR_BGR2GRAY);
	}

	bkgImage = imread(bkg_name.str(), COLOR_BGR2GRAY); //��� ��� �̹��� read

	if (bkgImage.empty())                              //��� �ִٸ� ��� ��� ���ϱ�
	{
		bkgImg.setFile(file);

		bkgImg.isAcummulateBkgImage(bkg_name.str());

		bkgImage = imread(bkg_name.str(), COLOR_BGR2GRAY);
	}

	int fps = (int)(capture.get(CAP_PROP_FPS));//�Է� ������ �ӵ�
	int nTh = 35;                              //����ȭ �Ӱ谪
	int frameNum = 0;                          //������ ��ȣ ����(�� ������ ������ ������ ��꿡 �ʿ�)
	int delay = 500 / fps;                     //������ �ӵ� ������ ���� ������ ����

	Mat frame, diffImage;                      //�����Ӱ� ��� ��� �� �̹��� ����
	Mat element4(4, 4, CV_8U, Scalar(1));      //�������� ���� ����ũ
	Mat element25(25, 25, CV_8U, Scalar(1));   //�������� �ݱ� ����ũ

	while (true)
	{
		capture >> frame;

		if (frame.empty()) break;

		cvtColor(frame, grayImage, COLOR_BGR2GRAY);

		//���� ����
		IplImage *rect = &IplImage(grayImage);

		int rWidth = rect->width;	//rect�� ���α���
		int rHeight = rect->height;	//rect�� ���α���

		Rect rt(rWidth * 1 / 6, rHeight * 1 / 2, rWidth * 2 / 3, rHeight * 1 / 50);
		CvPoint pt1 = cvPoint(rWidth * 1 / 6, rHeight * 1 / 2);
		CvPoint pt2 = cvPoint(rWidth * 5 / 6, rHeight * 26 / 50);
		//Rect rt(���� ������, ���� ������, �ʺ�, ����

		rectangle(grayImage, rt, Scalar(255, 0, 0), 2); //���� ���� ǥ��

		absdiff(grayImage, bkgImage, diffImage);        //�Է� ������(grayImage) - ��� ��� �̹���(bkgImage) = �� ���� (diffImage)

		threshold(diffImage, diffImage, nTh, 255, THRESH_BINARY);  //�Ӱ�ġ ���� ������ 255�� ��ȯ ����ȭ

		morphologyEx(diffImage, diffImage, MORPH_OPEN, element4);   //��Ƽ ���� 5x5 ����Ʈ �������� ���� ����
		morphologyEx(diffImage, diffImage, MORPH_CLOSE, element25); //����� ä��� 50x50 ����Ʈ �������� �ݱ� ����

		IplImage *trans = &IplImage(diffImage);         //��� ���̺��� ���� mat(c++) -> IplImage(c) ��ȯ

		label.SetParam(trans, 8000);                    //8000 ������ ũ�� ����
		label.DoLabeling();                             //���̺� ����

		if (option & MOTORCYCLE)
			label.BlobWidthHeightSmallRatioConstraint(1.5, VERTICAL); //���� ���κ� 1.5 �̸� ����

		IplImage *object = &IplImage(diffImage);

		for (int i = 0; i < label.m_nBlobs; i++)
		{
			label.m_recBlobs[i].y = label.m_recBlobs[i].y - 10;
			label.m_recBlobs[i].height = label.m_recBlobs[i].height + 10;
			/*rectangle(grayImage, label.m_recBlobs[i], Scalar(255), 2);*/
			cvSetImageROI(object, label.m_recBlobs[i]);

			label.BlobBigSizeConstraint(4, 6);

			int cWidth = label.m_recBlobs[i].width;
			int cHeight = label.m_recBlobs[i].height;

			Mat object_mat = cvarrToMat(object);
			Mat helmetArea;
			Mat licenseArea;

			CvPoint cPt = cvPoint(label.m_recBlobs[i].x, label.m_recBlobs[i].y);

			CvPoint cCenter = cvPoint(cPt.x + (cWidth / 2), cPt.y + (cHeight / 2));

			if ((cCenter.x > pt1.x) && (cCenter.x < pt2.x) && (cCenter.y > pt1.y) && (cCenter.y < pt2.y))
			{
				int result = 0;
				LicensePlate licenseDetcet;
				Helmet hdect;

				if (option & MOTORCYCLE)
					result += isMotorcycleDetetion(object_mat, diffImage);

				if (option & MOTORCYCLE)
				{
					Rect helmetRect(label.m_recBlobs[i].x, label.m_recBlobs[i].y, label.m_recBlobs[i].width,
											label.m_recBlobs[i].height / 7 * 2);

					helmetArea = frame(helmetRect);

					result += hdect.isHelmetDetect(helmetArea, bkgImage, helmetRect);
				}

				//true�� ��ȣ�� ����
				if (option & LICENSEPLATE)
				{
					Rect licensePlate(label.m_recBlobs[i].x, label.m_recBlobs[i].y + label.m_recBlobs[i].height / 7 * 5,
											label.m_recBlobs[i].width, label.m_recBlobs[i].height / 7 * 2);
					
					licenseArea = frame(licensePlate).clone();

					result += licenseDetcet.isLicensePlateDetection(licenseArea);
				}

				if ((result & ALL) == option)
				{
					time_t curr;
					struct tm *d;
					curr = time(NULL);
					d = localtime(&curr);

					f_name.str(std::string()); //�����ִ� string �����
					f_name.clear();

					//frame ����
					f_name << d->tm_year + 1900 << "." << d->tm_mon + 1 << "." << d->tm_mday << "_" << d->tm_hour << "."
						<< d->tm_min << "." << d->tm_sec << "_" << licenseDetcet.getNumber() << ".jpg";
					Mat store = frame(label.m_recBlobs[i]);
					imwrite(f_name.str(), store);

					//DB ����
					MHLdata db;
					db.isInsertQuery(db.isSelectQuery(), f_name.str(), file, "", licenseDetcet.getNumber());
				}
			}
			cvResetImageROI(object);
		}
		frameNum++;

		showVideo("grayImage", grayImage); //�⺻ ��鿵�� ��ü ���� ��� ǥ�� ���
		showVideo("diffImage", diffImage); //�� ���� ����ȭ ���

		int ckey = waitKey(delay);         //Ű �Է� ��� �ð�
		if (ckey == 27) break;
	}
}

/*
	���� ������ ���� ������ ���� ���� ���� �κ��� ����� ���� ��
	��ü �̹����� ��� ���� ������ ������ ũ�ٸ� ������̷� �Ǻ��ϴ� ���
	parameter -> motocycle :������� �ǽ� ����  diffImage : ��� �� ����
*/
int Motorcycle::isMotorcycleDetetion(Mat motocycle,Mat diffImage)
{
	IplImage *rect = &IplImage(grayImage);

	IplImage *cycle = &IplImage(diffImage);

	//////////������� ���� �ȼ� �� ���ϱ�//////////
	int cycleFix = 0;
	for (int i = 0; i < motocycle.rows; i++)
		for (int j = 0; j < motocycle.cols; j++)
			cycleFix += motocycle.at<uchar>(i, j) / 255;

	//////////������� ���� �ȼ� �� ���ϱ�//////////
	int colFix = motocycle.cols; //�� �ȼ���
	int rowFix = motocycle.rows; //�� �ȼ���
	int allFix = colFix * rowFix; //��ü �ȼ� ��
	int colAvg = cycleFix / colFix;//�����
	int rowAvg = cycleFix / rowFix;//�����

	/////////�� ��� ���� ��� ���ϱ�//////////
	int cFirst = colFix / 10 * 3;
	int cSecond = colFix / 10 * 7;
	int cCenterFix = 0;

	for (int i = 0; i < rowFix; i++)
		for (int j = cFirst; j <= cSecond; j++)
			cCenterFix += motocycle.at<uchar>(i, j) / 255;

	int cCenterAvg = cCenterFix / (cSecond - cFirst);
	/////////�� ��� ���� ��� ���ϱ�//////////

	/////////�� �� ���� ��� ���ϱ�//////////
	int rFirst = rowFix / 7 * 2;
	int rSecond = rowFix / 7 * 3;
	int rArmFix = 0;

	for (int i = 0; i < rowFix; i++)
		for (int j = rFirst; j <= rSecond; j++)
			rArmFix += motocycle.at<uchar>(i, j) / 255;

	int rArmAvg = rArmFix / (rSecond - rFirst);
	/////////�� �� ���� ��� ���ϱ�//////////

	if (cCenterAvg > colAvg&&rArmAvg > rowAvg) return MOTORCYCLE;

	else return false;
}


