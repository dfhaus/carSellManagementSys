//��������Ӧ�ó������

//���´������ڽ�����ĳ�����������
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

#include "csms.h"
#include "connection.h"
#include "logindialog.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include<QDialog>
#include <QPixmap>
#include <QSplashScreen>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //������������õ���time���� 

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	srand((unsigned)time(NULL)); //��ʱ�����֣�ÿ�β����������һ��
	int number = rand() % 4;  //����0-4�������
	QPixmap pixmap;
	/*int i=100��
	//����תQString
	QString s = QString::number(i);*/
	QString s = QString::number(number);
	pixmap.load("res/welcome"+s+".png");
// 	switch (number)
// 	{
// 		
// 	case 0:pixmap.load("res/welcome0.png"); break;
// 	case 1:pixmap.load("res/welcome1.png"); break;
// 	case 2:pixmap.load("res/welcome2.png"); break;
// 	case 3:pixmap.load("res/welcome3.png"); break;
// 	case 4:pixmap.load("res/welcome4.png"); break;
// 	default:
//		break;
// 	}
	
	QSplashScreen splash(pixmap);
	splash.show();
	for (long i = 0; i < 100000000; i++);//ϵͳ�����ȴ�ʱ��,ѭ��һ�ڴ�
// 	CSMS w;
// 	w.show();
	if (!createConnection() || !createXml()) return 0;
	CSMS w;

	LoginDialog login;

	splash.finish(&login);
	if (login.exec() == QDialog::Accepted)
	{

		w.show();
		//splash.finish(&login);
		return a.exec();
	}
	else
	{
		//splash.finish(&login);
		return 0;
	}

	

	
	/*w.show();
	return a.exec();*/
	//return a.exec();
}
//���ɿ���ϵͳ��ӭͼƬ
/*int main(int argc, char *argv[])
  {
  QApplication app(argc, argv);
  QPixmap pixmap(":/splash.png");
  QSplashScreen splash(pixmap);
  splash.show();
  app.processEvents();
  ...
  QMainWindow window;
  window.show();
  splash.finish(&window);
  return app.exec();
  }
*/