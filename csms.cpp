#include "csms.h"

CSMS::CSMS(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//setWindowIcon(QIcon("icon.ico"));

	this->setWindowTitle(tr("�������۹���ϵͳ"));
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
	//�����ź����
	connect(ui.manageAction, SIGNAL(triggered()), this, SLOT(manageActionSlot()));
	connect(ui.chartAction, SIGNAL(triggered()), this, SLOT(chartActionSlot()));
	connect(ui.quitAction, SIGNAL(triggered()), this, SLOT(quitActionSlot()));
	//currentIndexChanged(QString)ComboBox���ź�
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

void  CSMS::manageActionSlot()	//ʵ�֡�Ʒ�Ƴ������Ӳ˵����ܺ���
{
	ui.stackedWidget->setCurrentIndex(0);
}

void  CSMS::chartActionSlot()	//ʵ�֡�����ͳ�ơ��Ӳ˵����ܺ���
{
	ui.stackedWidget->setCurrentIndex(1);
	ui.updateBtn->setVisible(false);
	QSqlQueryModel *factoryModel = new QSqlQueryModel(this);
	factoryModel->setQuery("select name from factory");
	ui.factoryComboBox->setModel(factoryModel);
}

void  CSMS::quitActionSlot()		//ʵ�֡��˳����Ӳ˵����ܺ���
{
	this->close();
}

void CSMS::sellFactoryComboBoxSlot(const QString &arg1)//ʵ�ֳ�����Ͽ���Ʒ����Ͽ�ͬ����ʾ
{
	if (arg1 == "��ѡ�񳧼�")
	{
		//������������������
		sellCancelBtnSlot();
	}
	else
	{
		ui.sellBrandComboBox->setEditable(true);
		QSqlQueryModel *model = new QSqlQueryModel(this);
		model->setQuery(QString("select name from brand where factory = '%1'").arg(arg1));
		//model->setQuery(QString("select name from brand where factory = 'һ������'"));
		//for debug: find bug in connection :false uiName 
		ui.sellBrandComboBox->setModel(model);
		ui.sellCancelBtn->setEnabled(true);
	}
}

void CSMS::sellCancelBtnSlot()							//���ȡ����ť
{
	ui.sellFactoryComboBox->setCurrentIndex(0);
	ui.sellBrandComboBox->clear();
	ui.sellLastNumLabel->setText(tr("ʣ��������000"));
}

void CSMS::sellBrandComboBoxSlot(const QString &arg1)	//ʵ��Ʒ����Ͽ��뱨�ۡ�����ͬ��
{
	ui.sellNumSpinBox->setValue(0);
	ui.sellNumSpinBox->setEnabled(false);
	ui.sellSumLineEdit->clear();
	ui.sellSumLineEdit->setEnabled(false);
	ui.sellOkBtn->setEnabled(false);
	//��ѯ����
	QSqlQuery query;
	query.exec(QString("select price from brand where name = '%1' and factory = '%2'").arg(arg1).arg(ui.sellFactoryComboBox->currentText()));
	query.next();
	ui.sellPriceLineEdit->setEnabled(true);
	ui.sellPriceLineEdit->setReadOnly(true);
	ui.sellPriceLineEdit->setText(query.value(0).toString());
	////��ѯʣ������
	//query.exec(QString("select last from brand where name = '%1' and factory = '%2'").arg(arg1).arg(ui.sellFactoryComboBox->currentText()));
	//query.next();
	//int num = query.value(0).toInt();
	//if (num == 0)
	//{
	//	QMessageBox::information(this, tr("��ʾ"), tr("��Ʒ�����������꣡"), QMessageBox::Ok);
	//}
	//else
	//{
	//	ui.sellNumSpinBox->setEnabled(true);
	//	ui.sellNumSpinBox->setMaximum(num);
	//	ui.sellLastNumLabel->setText(tr("ʣ�������� %1").arg(num));
	//	ui.sellLastNumLabel->setVisible(true);
	//}
	showSellLastNumLabelSlot();
}

void CSMS::sellNumSpinBoxSlot(int arg1)		//������������ʾ�ܽ��
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

void CSMS::sellOkBtnSlot()			//���ȷ����ť��
{
	QString factory = ui.sellFactoryComboBox->currentText();
	QString name = ui.sellBrandComboBox->currentText();
	int value = ui.sellNumSpinBox->value();
	//sellNumSpinBox�����ֵ������ǰ��ʣ����
	int last = ui.sellNumSpinBox->maximum() - value;

	QSqlQuery query;
	//��ȡ��ǰ��������
	query.exec(QString("select sell from brand where name = '%1' and factory = '%2'").arg(name).arg(factory));
	query.next();
	int sell = query.value(0).toInt() + value;

	//�������
	QSqlDatabase::database().transaction();
	bool rtn = query.exec(QString("update brand set sell = %1, last = %2 where name = '%3' and factory = '%4'").arg(sell).arg(last).arg(name).arg(factory));
	if (rtn)
	{
		QSqlDatabase::database().commit();
		QMessageBox::information(this, tr("�� ʾ"), tr("�� �� �� �� ��"), QMessageBox::Ok);
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

void CSMS::showSellLastNumLabelSlot()//��ʾʣ������
{
	QSqlQuery query;
	//��ѯʣ������
	if (!ui.sellBrandComboBox->currentText().isEmpty())
	{
		query.exec(QString("select last from brand where name = '%1' and factory = '%2'").arg(ui.sellBrandComboBox->currentText()).arg(ui.sellFactoryComboBox->currentText()));
		query.next();
		int num = query.value(0).toInt();
		if (num == 0)
		{
			QMessageBox::information(this, tr("��ʾ"), tr("��Ʒ�����������꣡"), QMessageBox::Ok);
		}
		else
		{
			ui.sellNumSpinBox->setEnabled(true);
			ui.sellNumSpinBox->setMaximum(num);
			ui.sellLastNumLabel->setText(tr("ʣ�������� %1").arg(num));
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

bool CSMS::docRead()					//��ȡXML�ĵ�
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

bool CSMS::docWrite()				//д��XML�ĵ�
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

void CSMS::writeXml()				//�޸�XML�ĵ�
{
	//���ȴ��ļ���ȡ
	if (docRead())
	{
		QString currentDate = getDateTime(Date);
		QDomElement root = doc.documentElement();
		//�����Ƿ������ڽڵ���д���
		if (!root.hasChildNodes())
		{
			QDomElement date = doc.createElement(QString("����"));
			QDomAttr curDate = doc.createAttribute("date");
			curDate.setValue(currentDate);
			date.setAttributeNode(curDate);
			root.appendChild(date);
			createNodes(date);
		}
		else
		{
			QDomElement date = root.lastChild().toElement();
			//�����Ƿ��н�������ڽڵ���д���
			if (date.attribute("date")==currentDate)
			{
				createNodes(date);
			}
			else
			{
				QDomElement date = doc.createElement(QString("����"));
				QDomAttr curDate = doc.createAttribute("date");
				curDate.setValue(currentDate);
				date.setAttributeNode(curDate);
				root.appendChild(date);
				createNodes(date);
			}
		}
		//д���ļ�
		docWrite();
	}
}

void CSMS::createNodes(QDomElement &data)
{
	QDomElement time = doc.createElement(QString("ʱ��"));
	QDomAttr curTime = doc.createAttribute("time");
	curTime.setValue(getDateTime(Time));
	time.setAttributeNode(curTime);
	data.appendChild(time);
	QDomElement factory = doc.createElement(QString("����"));
	QDomElement brand = doc.createElement(QString("Ʒ��"));
	QDomElement price = doc.createElement(QString("����"));
	QDomElement num = doc.createElement(QString("����"));
	QDomElement sum = doc.createElement(QString("���"));
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
		//���ñ���
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
				ui.dailyList->addItem(QString("���ڣ�%1").arg(date));
				ui.dailyList->addItem(" ");
				QDomNodeList children = dateElement.childNodes();
				//������ǰ���۵���������
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
					QString str = time + "����" + brand + factory + " " + num + "����" + "�ɽ���:" + price + "�򣬹�" + sum + "��Ԫ";
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
	chartModel->setHeaderData(0, Qt::Horizontal, QString("Ʒ��"));
	chartModel->setHeaderData(1, Qt::Horizontal, QString("��������"));

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


void CSMS::factoryComboBoxSlot(const QString &arg1)//��ʾͼ��
{
	if (arg1!="��ѡ�񳧼�")
	{
		showChart();
	}
	else
	{
		//chartModel->clear();
	}

}

void CSMS::updateBtnSlot()			//����ͼ��
{
	if (ui.factoryComboBox->currentText()!="��ѡ�񳧼�")
	{
		showChart();
	}
	else
	{
		//chartModel->clear();
	}
}



void CSMS::aboutSoftwareSlot()//�������
{
	QDialog  dlg;
	dlg.setWindowTitle(tr("����"));
	dlg.resize(374, 400);
	QLabel *copyRight = new QLabel(&dlg);
// 	QFont font(��Microsoft YaHei��, 10, 75); //��һ�����������壨΢���źڣ����ڶ����Ǵ�С���������ǼӴ֣�Ȩ����75�� 
// 	ui->label->setFont(font);

	//����Ȩ��
// 	QFont::Light - 25 ����
// 		QFont::Normal - 50 ����
// 		QFont::DemiBold - 63 �����
// 		QFont::Bold - 75 ����
// 		QFont::Black - 87 ����
	QFont font("����", 10, 75);

	copyRight->setFont(font);
	copyRight->setText(tr("�������\n��ϵͳʹ��VS2013רҵ�� + Qt5.6.0 + \nqt_vs_addin_1_2_5�������, Ϊ����\n���ݿ�γ����ϵͳ.\n     --- ������ѧԺ �����1151������\n\n�ο��鼮��Qt5������ʵ��"));
	dlg.exec();
}

void CSMS::helpActionSlot()//����ҳ
{
	QDesktopServices::openUrl(QUrl(tr("www.baidu.com")));
}