#ifndef CSMS_H
#define CSMS_H
//以下代码用于解决中文出现乱码问题
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


//前置声明
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

//槽的定义
private slots:
	void manageActionSlot();		//实现“品牌车管理”子菜单功能函数
	void chartActionSlot();			//实现“销售统计”子菜单功能函数
	void quitActionSlot();			//实现“退出”子菜单功能函数
	void sellFactoryComboBoxSlot(const QString &arg1);	//实现厂家组合框与品牌组合框同步显示
	void sellBrandComboBoxSlot(const QString &arg1);	//实现品牌组合框与报价、数量同步
	void sellCancelBtnSlot();		//点击取消按钮槽
	void sellNumSpinBoxSlot(int arg1);		//更改数量后显示总金额槽
	void sellOkBtnSlot();			//点击确定按钮槽
	void showSellLastNumLabelSlot();//显示剩余数量
	void factoryComboBoxSlot(const QString &arg1);//显示图表
	void updateBtnSlot();			//更新图表
private:
	Ui::CSMSClass ui;
	QDomDocument doc;
	bool docRead();					//读取XML文档
	bool docWrite();				//写入XML文档
	void writeXml();				//修改XML文档
	void createNodes(QDomElement &data);//创建销售汽车日期节点
	void showDailyList();

	QStandardItemModel *chartModel;
	void createChartModelView();
	void showChart();
	
	void aboutSoftwareSlot();//关于软件
	void helpActionSlot();//打开网页


};

#endif // CSMS_H
