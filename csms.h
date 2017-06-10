#ifndef CSMS_H
#define CSMS_H
//���´������ڽ�����ĳ�����������
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

#include <QtWidgets/QMainWindow>
#include "ui_csms.h"
#include "connection.h"
#include "pieview.h"
#include "qdom.h"
#include <QSqlQueryModel>
#include <QSplitter>
#include <QSqlQuery>
#include <QMessageBox>
#include<QDate>
#include <QTableView>
#include <QStandardItemModel>
#include <QUrl>
#include <QDesktopServices>


//ǰ������
class QStandardItemModel;
class CSMS : public QMainWindow
{
	Q_OBJECT

public:
	CSMS(QWidget *parent = 0);
	~CSMS();
	enum DataTimeType
	{
		Time,
		Date,
		DateTime
	};
	QString getDateTime(DataTimeType type);

//�۵Ķ���
private slots:
	void manageActionSlot();		//ʵ�֡�Ʒ�Ƴ������Ӳ˵����ܺ���
	void chartActionSlot();			//ʵ�֡�����ͳ�ơ��Ӳ˵����ܺ���
	void quitActionSlot();			//ʵ�֡��˳����Ӳ˵����ܺ���
	void sellFactoryComboBoxSlot(const QString &arg1);	//ʵ�ֳ�����Ͽ���Ʒ����Ͽ�ͬ����ʾ
	void sellBrandComboBoxSlot(const QString &arg1);	//ʵ��Ʒ����Ͽ��뱨�ۡ�����ͬ��
	void sellCancelBtnSlot();		//���ȡ����ť��
	void sellNumSpinBoxSlot(int arg1);		//������������ʾ�ܽ���
	void sellOkBtnSlot();			//���ȷ����ť��
	void showSellLastNumLabelSlot();//��ʾʣ������
	void factoryComboBoxSlot(const QString &arg1);//��ʾͼ��
	void updateBtnSlot();			//����ͼ��
private:
	Ui::CSMSClass ui;
	QDomDocument doc;
	bool docRead();					//��ȡXML�ĵ�
	bool docWrite();				//д��XML�ĵ�
	void writeXml();				//�޸�XML�ĵ�
	void createNodes(QDomElement &data);//���������������ڽڵ�
	void showDailyList();

	QStandardItemModel *chartModel;
	void createChartModelView();
	void showChart();
	
	void aboutSoftwareSlot();//�������
	void helpActionSlot();//����ҳ


};

#endif // CSMS_H
