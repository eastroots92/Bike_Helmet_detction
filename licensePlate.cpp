
#include "licensePlate.h"

/*
  ��ó : github
  ���� �� ������ ������ ���� ���ϴ� ��� 
*/
double LicensePlate :: getRotationAngle(const IplImage* src)
{
	// Only 1-Channel
	if (src->nChannels != 1)
		return 0;

	// ������ �� ����� �� �ֵ��� ��â
	cvDilate((IplImage*)src, (IplImage*)src);

	// ���念�� ����
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* seqLines;

	// Image�� ���������� seq�� ����(rho, theta)
	seqLines = cvHoughLines2((IplImage*)src, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 30, 3); 

	// �������� �Ÿ��� ���ؼ� ���� �� ������ �������� �̹��� 0 or 90���� ȸ��
	double    longDistance = 0;    // ���� �� ���� �� ����
	int        longDistanceIndex = 0;    // ���� �� ���� �� ���� �ε���

	for (int i = 0; i < seqLines->total; i++)
	{
		CvPoint* lines = (CvPoint*)cvGetSeqElem(seqLines, i);
		double euclideanDistance;        // sequence�� ����� line���� Euclidean distance�� ����
		euclideanDistance = (lines[1].x - lines[0].x) * (lines[1].x - lines[0].x) + (lines[1].y - lines[0].y) * (lines[1].y - lines[0].y);
		euclideanDistance = sqrt(euclideanDistance);

		// ���� �� Euclidean distance�� ���� 
		if (longDistance < euclideanDistance)
		{
			longDistanceIndex = i;
			longDistance = euclideanDistance;
		}
	}

	// ȸ���� ���� ���
	CvPoint *lines = (CvPoint*)cvGetSeqElem(seqLines, longDistanceIndex);
	int         dx = lines[1].x - lines[0].x;
	int         dy = lines[1].y - lines[0].y;
	double     rad = atan2((double)dx, (double)dy);    // ȸ���� ����(radian)
	double  degree = abs(rad * 180) / CV_PI;            // ȸ���� ����(degree) ����

	// �޸� ����
	cvClearSeq(seqLines);
	cvReleaseMemStorage(&storage);

	return degree;
}

/*
  ��ó : github
  ������ ���� �̿��Ͽ� ȸ���ϴ� ���
*/
void LicensePlate::rotateImage(const IplImage* src, IplImage* dst, double degree)
{
	// Only 1-Channel
	if (src->nChannels != 1)
		return;

	CvPoint2D32f centralPoint = cvPoint2D32f(src->width / 2, src->height / 2);            // ȸ�� ������ ����(�̹����� �߽���)
	CvMat *rotationMatrix = cvCreateMat(2, 3, CV_32FC1);                        // ȸ�� ���� ���

	// Rotation ���� ��� ���� �� ����(90������ ������ ������ ���� �����̹���(�ʿ�� ����))
	cv2DRotationMatrix(centralPoint, 90 - degree, 1, rotationMatrix);

	// Image Rotation
	cvWarpAffine(src, dst, rotationMatrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);

	// Memory ����
	cvReleaseMat(&rotationMatrix);
}

/*
	��ȣ�� x��ǥ���� ������ ���� �� ���� ���
*/
void LicensePlate :: qSort(Mat data[], int numX[], int l, int r)
{
	int left = l;
	int right = r;
	int pivot = numX[(l + r) / 2];

	do{
		while (numX[left] < pivot) left++;
		while (numX[right] > pivot) right--;
		if (left <= right){
			int temp = numX[left];
			numX[left] = numX[right];
			numX[right] = temp;

			Mat mTemp = data[left].clone();
			data[left] = data[right].clone();
			data[right] = mTemp.clone();

			left++;
			right--;
		}
	} while (left <= right);

	if (l < right) qSort(data, numX, l, right);
	if (r > left) qSort(data, numX, left, r);
}

/*
	�Ѱܹ��� ���� �̹����� �������ø� ��Ī�� �̿��� 
	�������� Max���� �̿��Ͽ� ���� ����� ���ڸ� ã�Ƴ��� ���
*/
string LicensePlate ::findNumber(Mat number)
{
	Mat findNum = imread("ten.bmp", CV_8U);
	
	threshold(findNum, findNum, 100, 255, THRESH_BINARY);

	CBlobLabeling labelFindNum;
	
	labelFindNum.SetParam(&(IplImage)findNum, 300);
	
	labelFindNum.DoLabeling();
	
	Mat numRoi[10];
	int numX[10];

	for (int i = 0; i < labelFindNum.m_nBlobs; i++)
	{
		numRoi[i] = findNum(Rect(labelFindNum.m_recBlobs[i].x, labelFindNum.m_recBlobs[i].y, labelFindNum.m_recBlobs[i].width, labelFindNum.m_recBlobs[i].height));
		numX[i] = labelFindNum.m_recBlobs[i].x;	
		resize(numRoi[i], numRoi[i], Size(50, 90));
	}

	qSort(numRoi, numX, 0, 9);

	CvPoint left_top;
	double min, max;
	double temp=0;
	int j = 0;

	//�������� ���� �̹���
	IplImage *coeff;
	for (int i = 0; i < labelFindNum.m_nBlobs; i++)
	{
		coeff = cvCreateImage(cvSize((&IplImage(number))->width - (&IplImage(numRoi[i]))->width + 1, (&IplImage(number))->height - (&IplImage(numRoi[i]))->height + 1), IPL_DEPTH_32F, 1);
		// �������� ���Ͽ� coeff�� �׷��ش�.
		cvMatchTemplate(&IplImage(number), &IplImage(numRoi[i]), coeff, CV_TM_CCOEFF_NORMED);
		// �������� �ִ밪�� ������ ��ġ�� ã�´� 
		cvMinMaxLoc(coeff, &min, &max, NULL, &left_top);

		if (temp < max)
		{
			temp = max;
			j = i;
		}
	}
	return to_string(j);
}

/*
  ���̺� �� ����/���� ������ 1.5 �������� �������� ���� ��
  �緹�̺� �� ����/���� ������ 1.2 �������� �������� ���� ��
  ���̺� ���� 4�� �� �κ��� ��ȣ�� �����̶�� �Ǵ�.
  ���ø� ��Ī�� ����� �߿��Ͽ� ��������
*/
int LicensePlate::isLicensePlateDetection(Mat frame)
{
	if (frame.empty()) return 0;

	resize(frame, frame, Size(800, 500)); //���ø� ��Ī�� ���� ��������

	bin = frame.clone();

	cvtColor(bin, bin, CV_BGR2GRAY);

	Mat binaryBin = bin.clone();

	for (int t = 0; t < 2; t++)
	{
		if (t == 0)
			threshold(bin, binaryBin, 50, 255, CV_THRESH_BINARY_INV | THRESH_OTSU);
		else if (t == 1) //����ȭ�� ���⿡ �����Ͽ��ٸ� ������ȭ 
			threshold(bin, binaryBin, 50, 255, CV_THRESH_BINARY | THRESH_OTSU);

		CBlobLabeling label;

		label.SetParam(&(IplImage)binaryBin, 500);
		label.DoLabeling();

		////////// ���� ����/���� ���� ����////////////
		label.BlobWidthHeightSmallRatioConstraint(1.5, HORIZONTAL);
		label.BlobBigSizeConstraint(600, 400);

		Mat labelimg = frame.clone();

		////////////////���ɿ�������//////////////
		for (int i = 0; i < label.m_nBlobs; i++)
		{
			Mat roi = labelimg(Rect(label.m_recBlobs[i].x, label.m_recBlobs[i].y, label.m_recBlobs[i].width, label.m_recBlobs[i].height));

			resize(roi, roi, Size(350, 200));

			Mat grayRoi = roi.clone();

			cvtColor(grayRoi, grayRoi, CV_BGR2GRAY);

			Mat binaryRoi = grayRoi.clone();

			threshold(grayRoi, binaryRoi, 50, 255, CV_THRESH_BINARY_INV | THRESH_OTSU);//����ȭ

			CBlobLabeling relabel;

			relabel.SetParam(&(IplImage)binaryRoi, 900);
			relabel.DoLabeling();

			////////// ���� ����/���� ���� ����////////////
			relabel.BlobWidthHeightSmallRatioConstraint(1.2, VERTICAL);
			if (relabel.m_nBlobs != 4) //4���� �ƴ϶�� ���� ���� �˻�
				continue;
			
			/////////////////////////////////////////////////////////////////
			if (relabel.m_nBlobs == 4)
			{
				t = 3;
				Mat numRoi = grayRoi.clone();
				Mat rotate = binaryRoi.clone(); //ȸ���� ���ϱ�

				int angle = getRotationAngle(&(IplImage)rotate); //ȸ����Ű��

				rotateImage(&(IplImage)rotate, &(IplImage)rotate, angle);//�����ֱ���

				Mat num[4];
				int nX[4];

				for (int j = 0; j < relabel.m_nBlobs; j++)
				{
					//4���� ��ȣ ����
					for (int k = 0; k < relabel.m_nBlobs; k++){
						num[k] = numRoi(Rect(relabel.m_recBlobs[k].x, relabel.m_recBlobs[k].y, relabel.m_recBlobs[k].width, relabel.m_recBlobs[k].height));
						nX[k] = relabel.m_recBlobs[k].x;
					}

					//��ȣ�� x�������� ������ ����
					qSort(num, nX, 0, 3);

					Mat number = num[j];

					rotateImage(&(IplImage)number, &(IplImage)number, angle); //���� ȸ��

					threshold(number, number, 50, 255, CV_THRESH_BINARY_INV | THRESH_OTSU);

					resize(number, number, Size(50, 90));

					licenseNum += findNumber(number);

					destroyWindow("1");

					rectangle(roi, relabel.m_recBlobs[j], Scalar(255), 2);
				}
				cout << licenseNum;

				/*time_t curr;
				struct tm *d;
				curr = time(NULL);
				d = localtime(&curr);

				f_name << d->tm_year + 1900 << "." << d->tm_mon + 1 << "." << d->tm_mday << "_" << d->tm_hour << "." << d->tm_min << "." << d->tm_sec << "_" << licenseNum << type;
				imwrite(f_name.str(), roi);*/
				imshow("��ȣ�� ����1", roi);

				return LICENSEPLATE;
			}
		}
	}
	return 0;
}