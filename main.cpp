#include "main.h"

using namespace std;
using namespace cv;


int main() 
{
	string file = "Video\\t_b_1.mp4"; // ���� file

	Motorcycle detection(file);       // ������� ���� ��ü �����Է�

	detection.isMHLDetection(HELMET | MOTORCYCLE); //������� �������� ��� ������ ���� ����

	waitKey(0);

	return 0;
}
