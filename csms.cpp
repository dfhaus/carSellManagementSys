#include "csms.h"

CSMS::CSMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//setWindowIcon(QIcon("icon.ico"));

	this->setWindowTitle(tr("汽车销售管理系统"));
	this->setFixedSize(750, 500);
	ui.stackedWidget->setCurrentIndex(0);

	//


	QSqlQueryModel *factoryModel = new QSqlQueryModel(this);
	factoryModel->setQuery("select name from factory");
	ui.sellFactoryComboBox->setModel(factoryModel);

	QSplitter *splitter = new QSplitter(ui.managePage);
	splitter->resize(700, 360);
	splitter->move(0, 50);
	splitter->addWidget(ui.toolBox);
	splitter->addWidget(ui.dailyList);
	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 1);
	
	ui.sellFactoryComboBox->setModel(factoryModel);
	createChartModelView();



	//sellCancelBtnSlot();
	//链接信号与槽
	connect(ui.manageAction, SIGNAL(triggered()), this, SLOT(manageActionSlot()));
	connect(ui.chartAction, SIGNAL(triggered()), this, SLOT(chartActionSlot()));
	connect(ui.quitAction, SIGNAL(triggered()), this, SLOT(quitActionSlot()));
	//currentIndexChanged(QString)ComboBox的信号
	connect(ui.sellFactoryComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(sellFactoryComboBoxSlot(QString)));
	connect(ui.sellBrandComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(sellBrandComboBoxSlot(QString)));
	connect(ui.sellCancelBtn, SIGNAL(clicked()), this, SLOT(sellCancelBtnSlot()));
	connect(ui.sellNumSpinBox, SIGNAL(valueChanged(int)), this, SLOT(sellNumSpinBoxSlot(int)));
	connect(ui.sellOkBtn, SIGNAL(clicked()), this, SLOT(sellOkBtnSlot()));
	//connect(ui.sellOkBtn, SIGNAL(clicked()), this, SLOT(showSellLastNumLabelSlot()));
	connect(ui.factoryComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(factoryComboBoxSlot(QString)));
	connect(ui.updateBtn, SIGNAL(clicked()), this, SLOT(updateBtnSlot()));
	//connect(ui.factoryComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(showChart(QString)));
	//connect(ui.baiduAction, SIGNAL(triggered()), this, SLOT(baiduActionSlot()));
	connect(ui.helpAction, &QAction::triggered, this, &CSMS::helpActionSlot);
	showDailyList();
	connect(ui.aboutSoftwareAction, &QAction::triggered, this, &CSMS::aboutSoftwareSlot);
// 	connect(ui.aboutQtAction, &QAction::triggered, this, &CSMS::aboutQt);//[static slot] void QApplication::aboutQt()
// 	connect(ui.aboutQtAction, SIGNAL(triggered()), this, SLOT(manageActionSlot()));
	QObject::connect(ui.aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	ui.factoryComboBox->setModel(factoryModel);
	createChartModelView();

}

CSMS::~CSMS()
{
	
}

void  CSMS::manageActionSlot()	//实现“品牌车管理”子菜单功能函数
{
	ui.stackedWidget->setCurrentIndex(0);
}

void  CSMS::chartActionSlot()	//实现“销售统计”子菜单功能函数
{
	ui.stackedWidget->setCurrentIndex(1);
	ui.updateBtn->setVisible(false);
	QSqlQueryModel *factoryModel = new QSqlQueryModel(this);
	factoryModel->setQuery("select name from factory");
	ui.factoryComboBox->setModel(factoryModel);
}

void  CSMS::quitActionSlot()		//实现“退出”子菜单功能函数
{
	this->close();
}

void CSMS::sellFactoryComboBoxSlot(const QString &arg1)//实现厂家组合框与品牌组合框同步显示
{
	if (arg1 == "请选择厂家")
	{
		//进行其他部件的设置
		sellCancelBtnSlot();
	}
	else
	{
		ui.sellBrandComboBox->setEditable(true);
		QSqlQueryModel *model = new QSqlQueryModel(this);
		model->setQuery(QString("select name from brand where factory = '%1'").arg(arg1));
		//model->setQuery(QString("select name from brand where factory = '一汽大众'"));
		//for debug: find bug in connection :false uiName 
		ui.sellBrandComboBox->setModel(model);
		ui.sellCancelBtn->setEnabled(true);
	}
}

void CSMS::sellCancelBtnSlot()							//点击取消按钮
{
	ui.sellFactoryComboBox->setCurrentIndex(0);
	ui.sellBrandComboBox->clear();
	ui.sellLastNumLabel->setText(tr("剩余数量：000"));
}

void CSMS::sellBrandComboBoxSlot(const QString &arg1)	//实现品牌组合框与报价、数量同步
{
	ui.sellNumSpinBox->setValue(0);
	ui.sellNumSpinBox->setEnabled(false);
	ui.sellSumLineEdit->clear();
	ui.sellSumLineEdit->setEnabled(false);
	ui.sellOkBtn->setEnabled(false);
	//查询报价
	QSqlQuery query;
	query.exec(QString("select price from brand where name = '%1' and factory = '%2'").arg(arg1).arg(ui.sellFactoryComboBox->currentText()));
	query.next();
	ui.sellPriceLineEdit->setEnabled(true);
	ui.sellPriceLineEdit->setReadOnly(true);
	ui.sellPriceLineEdit->setText(query.value(0).toString());
	////查询剩余数量
	//query.exec(QString("select last from brand where name = '%1' and factory = '%2'").arg(arg1).arg(ui.sellFactoryComboBox->currentText()));
	//query.next();
	//int num = query.value(0).toInt();
	//if (num == 0)
	//{
	//	QMessageBox::information(this, tr("提示"), tr("该品牌汽车已售完！"), QMessageBox::Ok);
	//}
	//else
	//{
	//	ui.sellNumSpinBox->setEnabled(true);
	//	ui.sellNumSpinBox->setMaximum(num);
	//	ui.sellLastNumLabel->setText(tr("剩余数量： %1").arg(num));
	//	ui.sellLastNumLabel->setVisible(true);
	//}
	showSellLastNumLabelSlot();
}

void CSMS::sellNumSpinBoxSlot(int arg1)		//更改数量后显示总金额
{
	if (arg1 ==0)
	{
		ui.sellSumLineEdit->clear();
		ui.sellSumLineEdit->setEnabled(false);
		ui.sellOkBtn->setEnabled(false);
	}
	else
	{
		ui.sellSumLineEdit->setEnabled(true);
		ui.sellSumLineEdit->setReadOnly(true);
		qreal sum = arg1*ui.sellPriceLineEdit->text().toInt();
		ui.sellSumLineEdit->setText(QString::number(sum));
		ui.sellOkBtn->setEnabled(true);
	}
}

void CSMS::sellOkBtnSlot()			//点击确定按钮槽
{
	QString factory = ui.sellFactoryComboBox->currentText();
	QString name = ui.sellBrandComboBox->currentText();
	int value = ui.sellNumSpinBox->value();
	//sellNumSpinBox的最大值就是以前的剩余量
	int last = ui.sellNumSpinBox->maximum() - value;

	QSqlQuery query;
	//获取以前的销售量
	query.exec(QString("select sell from brand where name = '%1' and factory = '%2'").arg(name).arg(factory));
	query.next();
	int sell = query.value(0).toInt() + value;

	//事务操作
	QSqlDatabase::database().transaction();
	bool rtn = query.exec(QString("update brand set sell = %1, last = %2 where name = '%3' and factory = '%4'").arg(sell).arg(last).arg(name).arg(factory));
	if (rtn)
	{
		QSqlDatabase::database().commit();
		QMessageBox::information(this, tr("提 示"), tr("购 车 成 功 ！"), QMessageBox::Ok);
		writeXml();
		showDailyList();
		sellCancelBtnSlot();
	}
	else
	{
		QSqlDatabase::database().rollback();
	}
	QSqlQueryModel *factoryModel = new QSqlQueryModel();
	factoryModel->setQuery("select name from factory");
	ui.sellFactoryComboBox->setModel(factoryModel);
}

void CSMS::showSellLastNumLabelSlot()//显示剩余数量
{
	QSqlQuery query;
	//查询剩余数量
	if (!ui.sellBrandComboBox->currentText().isEmpty())
	{
		query.exec(QString("select last from brand where name = '%1' and factory = '%2'").arg(ui.sellBrandComboBox->currentText()).arg(ui.sellFactoryComboBox->currentText()));
		query.next();
		int num = query.value(0).toInt();
		if (num == 0)
		{
			QMessageBox::information(this, tr("提示"), tr("该品牌汽车已售完！"), QMessageBox::Ok);
		}
		else
		{
			ui.sellNumSpinBox->setEnabled(true);
			ui.sellNumSpinBox->setMaximum(num);
			ui.sellLastNumLabel->setText(tr("剩余数量： %1").arg(num));
			ui.sellLastNumLabel->setVisible(true);
		}
	}
	
}

QString CSMS::getDateTime(DataTimeType type)
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString date = datetime.toString("yyyy-MM-dd");
	QString time = datetime.toString("hh:mm");
	QString dateAndTime = datetime.toString("yyyy-MM-dd dddd hh:mm");
	if (type == Date)
	{
		return date;
	}
	else if (type == Time)
	{
		return time;
	}
	else
	{
		return dateAndTime;
	}
}

bool CSMS::docRead()					//读取XML文档
{
	QFile file("data.xml");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();
	return true;
}

bool CSMS::docWrite()				//写入XML文档
{
	QFile file("data.xml");
	if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return false;
	}
	QTextStream out(&file);
	doc.save(out, 4);
	file.close();
	return true;
}

void CSMS::writeXml()				//修改XML文档
{
	//首先从文件读取
	if (docRead())
	{
		QString currentDate = getDateTime(Date);
		QDomElement root = doc.documentElement();
		//根据是否有日期节点进行处理
		if (!root.hasChildNodes())
		{
			QDomElement date = doc.createElement(QString("日期"));
			QDomAttr curDate = doc.createAttribute("date");
			curDate.setValue(currentDate);
			date.setAttributeNode(curDate);
			root.appendChild(date);
			createNodes(date);
		}
		else
		{
			QDomElement date = root.lastChild().toElement();
			//根据是否有今天的日期节点进行处理
			if (date.attribute("date")==currentDate)
			{
				createNodes(date);
			}
			else
			{
				QDomElement date = doc.createElement(QString("日期"));
				QDomAttr curDate = doc.createAttribute("date");
				curDate.setValue(currentDate);
				date.setAttributeNode(curDate);
				root.appendChild(date);
				createNodes(date);
			}
		}
		//写入文件
		docWrite();
	}
}

void CSMS::createNodes(QDomElement &data)
{
	QDomElement time = doc.createElement(QString("时间"));
	QDomAttr curTime = doc.createAttribute("time");
	curTime.setValue(getDateTime(Time));
	time.setAttributeNode(curTime);
	data.appendChild(time);
	QDomElement factory = doc.createElement(QString("厂家"));
	QDomElement brand = doc.createElement(QString("品牌"));
	QDomElement price = doc.createElement(QString("报价"));
	QDomElement num = doc.createElement(QString("数量"));
	QDomElement sum = doc.createElement(QString("金额"));
	QDomText text;
	text = doc.createTextNode(QString("%1").arg(ui.sellFactoryComboBox->currentText()));
	factory.appendChild(text);

	text = doc.createTextNode(QString("%1").arg(ui.sellBrandComboBox->currentText()));
	brand.appendChild(text);

	text = doc.createTextNode(QString("%1").arg(ui.sellPriceLineEdit->text()));
	price.appendChild(text);

	text = doc.createTextNode(QString("%1").arg(ui.sellNumSpinBox->value()));
	num.appendChild(text);

	text = doc.createTextNode(QString("%1").arg(ui.sellSumLineEdit->text()));
	sum.appendChild(text);

	time.appendChild(factory);
	time.appendChild(brand);
	time.appendChild(price);
	time.appendChild(num);
	time.appendChild(sum);

}
void CSMS::showDailyList()
{
	ui.dailyList->clear();
	if (docRead())
	{
		//设置标题
		QDomElement root = doc.documentElement();
		QString title = root.tagName();
		QListWidgetItem *titleItem = new QListWidgetItem;
		titleItem->setText(QString("-----%1-----").arg(title));
		titleItem->setTextAlignment(Qt::AlignCenter);
		ui.dailyList->addItem(titleItem);
		if (root.hasChildNodes())
		{
			QString currentDate = getDateTime(Date);
			QDomElement dateElement = root.lastChild().toElement();
			QString date = dateElement.attribute("date");
			if (date == currentDate)
			{
				ui.dailyList->addItem(" ");
				ui.dailyList->addItem(QString("日期：%1").arg(date));
				ui.dailyList->addItem(" ");
				QDomNodeList children = dateElement.childNodes();
				//遍历当前销售的所有汽车
				for (int i = 0; i < children.count();i++)
				{
					QDomNode node = children.at(i);
					QString time = node.toElement().attribute("time");
					QDomNodeList list = node.childNodes();
					QString factory = list.at(0).toElement().text();
					QString brand = list.at(1).toElement().text();
					QString price = list.at(2).toElement().text();
					QString num = list.at(3).toElement().text();
					QString sum = list.at(4).toElement().text();
					QString str = time + "出售" + brand + factory + " " + num + "辆，" + "成交价:" + price + "万，共" + sum + "万元";
					QListWidgetItem *tempItem = new QListWidgetItem;
					tempItem->setText("***********************");
					tempItem->setTextAlignment(Qt::AlignCenter);
					ui.dailyList->addItem(tempItem);
					ui.dailyList->addItem(str);
				}
			}
		}
	}
}

void CSMS::createChartModelView()
{
	//chartModel = new QStandardItemModel(this);
	chartModel = new QStandardItemModel(this);
	chartModel->setColumnCount(2);
	chartModel->setHeaderData(0, Qt::Horizontal, QString("品牌"));
	chartModel->setHeaderData(1, Qt::Horizontal, QString("销售数量"));

	QSplitter *splitter = new QSplitter(ui.chartPage);
	splitter->resize(700, 320);
	splitter->move(0, 80);
	QTableView *table = new QTableView;
	PieView *pieChart = new PieView;
	splitter->addWidget(table);
	splitter->addWidget(pieChart);
	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 2);

	table->setModel(chartModel);
	pieChart->setModel(chartModel);

	QItemSelectionModel *selectionModel = new QItemSelectionModel(chartModel);
	table->setSelectionModel(selectionModel);
	pieChart->setSelectionModel(selectionModel);

}

void CSMS::showChart()
{
	QSqlQuery query;
	query.exec(QString("select name,sell from brand where factory='%1'").arg(ui.factoryComboBox->currentText()));
	chartModel->removeRows(0, chartModel->rowCount(QModelIndex()), QModelIndex());
	int row = 0;

	while (query.next()){
		int r = qrand() % 256;
		int g = qrand() % 256;
		int b = qrand() % 256;

		chartModel->insertRows(row, 1, QModelIndex());
		chartModel->setData(chartModel->index(row, 0, QModelIndex()), query.value(0).toString());
		chartModel->setData(chartModel->index(row, 1, QModelIndex()), query.value(1).toInt());
		chartModel->setData(chartModel->index(row, 0, QModelIndex()), QColor(r, g, b), Qt::DecorationRole);
		row++;
	}

}


void CSMS::factoryComboBoxSlot(const QString &arg1)//显示图表
{
	if (arg1!="请选择厂家")
	{
		showChart();
	}
	else
	{
		//chartModel->clear();
	}

}

void CSMS::updateBtnSlot()			//更新图表
{
	if (ui.factoryComboBox->currentText()!="请选择厂家")
	{
		showChart();
	}
	else
	{
		//chartModel->clear();
	}
}



void CSMS::aboutSoftwareSlot()//关于软件
{
	QDialog  dlg;
	dlg.setWindowTitle(tr("关于"));
	dlg.resize(374, 400);
	QLabel *copyRight = new QLabel(&dlg);
// 	QFont font(“Microsoft YaHei”, 10, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75） 
// 	ui->label->setFont(font);

	//常见权重
// 	QFont::Light - 25 高亮
// 		QFont::Normal - 50 正常
// 		QFont::DemiBold - 63 半粗体
// 		QFont::Bold - 75 粗体
// 		QFont::Black - 87 黑体
	QFont font("黑体", 10, 75);

	copyRight->setFont(font);
	copyRight->setText(tr("关于软件\n本系统使用VS2013专业版 + Qt5.6.0 + \nqt_vs_addin_1_2_5插件开发, 为本人\n数据库课程设计系统.\n     --- 淮阴工学院 计算机1151黄正果\n\n参考书籍：Qt5开发及实例"));
	dlg.exec();
}

void CSMS::helpActionSlot()//打开网页
{
	QDesktopServices::openUrl(QUrl(tr("www.baidu.com")));
}