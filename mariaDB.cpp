#include "mriaDB.h"

//mariaDB ����
MHLdata::MHLdata()
{
	connection = NULL;

	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char *)NULL, 0);

	if (connection == NULL)
	{
		cout << " CONNECT FAIL" << endl;
	}
}

//������ ����� data�� ��ȣ�� �ֱ� ����� no +1�� �о��
string MHLdata :: isSelectQuery()
{
	int num;

	string isselectQuery = "select no from detectiondata order by no desc limit 1";

	query_read = mysql_query(&conn, isselectQuery.c_str());

	sql_result = mysql_store_result(&conn);
	cout << "NO" << endl;

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		cout << sql_row[0] << endl;
		num = atoi(sql_row[0]);
	}

	string a = to_string(num + 1);
	return a;
}

//����� �����͵��� mariaDB�� ����
void MHLdata :: isInsertQuery(string no, string imgname, string inputname, string datapath, string licenseno)
{
	string insertQuery = "INSERT INTO detectiondata (no, imgname,inputname,datapath,licenseno) values('"
		+ no + "','" + imgname + "','" + inputname + "','" + datapath + "','" + licenseno + "')";

	query_stat = mysql_query(&conn, insertQuery.c_str());
}