#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include<QFile>
#include <QtXml/QDomDocument> 
#include <QtXml/QDomElement>
#include <QTextStream>
#include "qdom.h"
#include <QDialog>

//���´������ڽ�����ĳ�����������
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

static bool createConnection()
{
	//debug:list test driver is available or not
	/*qDebug() << "available driver:";
	QStringList drivers = QSqlDatabase::drivers();
	foreach(QString driver, drivers)
	qDebug() << "/t" << driver;
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	qDebug() << "ODBC driver?" << db.isValid();*/

	//connect to dataBase
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	QString dsn = QString::fromLocal8Bit("DRIVER={SQL SERVER};SERVER=.;DATABASE=CSMS");      
	db.setHostName("192.168.1.105");
	db.setDatabaseName(dsn);
	db.setUserName("sa");
	db.setPassword("123");

	//���ݿ�δ�򿪣�������ʾ
	if (!db.open())
	{
		//qDebug() << db.lastError().text();
		QMessageBox::information(NULL, QObject::tr("Database Error"), db.lastError().text());
		return false;
	}

	QSqlQuery query;
	//�������ұ�
	query.exec("create table factory(id varchar(4) primary key, name varchar(12))");
	query.exec(QString("insert into factory(id, name) values('0', '��ѡ�񳧼�')"));
	query.exec(QString("insert into factory(id, name) values('1', 'һ������')"));
	query.exec(QString("insert into factory(id, name) values('2', '��������')"));
	query.exec(QString("insert into factory(id, name) values('3', '�Ϻ�����')"));

	//����Ʒ�Ʊ�
	query.exec("create table brand(bid varchar(4) primary key, name varchar(8), factory  varchar(10), price int, sum int, sell int , last int)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('1', '�µ�A6', 'һ������', 36, 50, 10, 40)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('2', '�ݴ�', 'һ������', 34, 20, 5, 15)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('3', '����', 'һ������', 41, 80, 20, 60)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('4', '����', 'һ������', 83, 40, 15, 25)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('5', '�ϼ���', '��������', 39, 50, 15, 35)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('6', '����', '��������', 28, 60, 10, 50)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('7', '����307', '��������', 27, 70, 20, 50)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('8', 'ɣ����', '�Ϻ�����', 25, 75, 25, 50)");
	query.exec("insert into brand(bid, name, factory, price, sum, sell, last) values('9', '������', '�Ϻ�����', 27, 65, 20, 45)");
	//���������
	query.exec("create table adminuser(id varchar(8) primary key, password varchar(12))");
	query.exec(QString("insert into adminuser(id, password) values('admin', '1')"));
// 	query.exec(QString("insert into adminuser(id, password) values('root', '1')"));
// 	query.exec(QString("insert into adminuser(id, password) values('admin', '123')"));
// 	query.exec(QString("insert into adminuser(id, password) values('sa', '123')"));
// 	query.exec(QString("insert into adminuser(id, password) values('hzg', '1')"));
// 	query.exec(QString("insert into adminuser(id, password) values('������', '1')"));
	return true;
}

static bool createXml()
{
	QFile file("data.xml");
	if (file.exists())
	{
		return true;
	}
	if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return false;
	}
	QDomDocument doc;
	QDomProcessingInstruction instruction;
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);
	QDomElement root = doc.createElement(QString("�������嵥"));
	doc.appendChild(root);
	QTextStream out(&file);
	doc.save(out, 4);
	file.close();
	return true;

}

#endif // CONNECTION_H
