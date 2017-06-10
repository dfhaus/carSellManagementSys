#include "logindialog.h"
#include <QMessageBox>
#include <QtSql>


LoginDialog::LoginDialog(QWidget *parent) :
QDialog(parent)
/* ui(new Ui::LoginDialog)*/
{
	// ui->setupUi(this);

	this->setFixedSize(400, 300);
	setWindowTitle(tr("login"));
	//1
	userNameLabel = new QLabel(this);
	userNameLabel->setText(tr("userName:"));

	userNameLineEdit = new QLineEdit(this);
	//2
	pwdLabel = new QLabel(this);
	pwdLabel->setText(tr("password:"));
	
	
	pwdLineEdit = new QLineEdit(this);
	pwdLineEdit->setEchoMode(QLineEdit::Password);
	//3
	loginBtn = new QPushButton(this);
	loginBtn->setText(tr("login"));

	spaceLabel = new QLabel(this);
	spaceLabel->setText("     ");

	quitBtn = new QPushButton(this);
	quitBtn->setText(tr("quit"));

	//layout
	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(userNameLabel, 0, 0);
	mainLayout->addWidget(userNameLineEdit, 0, 1);
	mainLayout->addWidget(pwdLabel, 1, 0);
	mainLayout->addWidget(pwdLineEdit, 1, 1);
	mainLayout->addWidget(loginBtn, 2, 0);
	mainLayout->addWidget(spaceLabel, 2, 1);
	mainLayout->addWidget(quitBtn, 2, 2);
	this->setLayout(mainLayout);
	userNameLineEdit->setFocus();
	loginBtn->setDefault(true);


	//
	/*void loginBtnSlot()

		void quitBtnSlot();*/
	connect(loginBtn, SIGNAL(clicked()), this, SLOT(loginBtnSlot()));
	connect(quitBtn, SIGNAL(clicked()), this, SLOT(quitBtnSlot()));
}

void LoginDialog::loginBtnSlot()
{
	if (userNameLineEdit->text().isEmpty())
	{
		QMessageBox::information(this, tr("please input username"), tr("please input username then...."));
		userNameLineEdit->setFocus();
		//return;
	}
	else
	{
		if (pwdLineEdit->text().isEmpty())
		{

			QMessageBox::information(this, tr("please input password"), tr("please input password then...."));
			pwdLineEdit->setFocus();
		}
		else
		{
			//方法0
// 			QSqlQuery query;
// 			//用户名不存在
// 			bool ok = query.exec(QString("select password from adminuser where id = '%1'").arg(userNameLineEdit->text()));
// 			//query.next();
// 			password = query.value(0).toString();//?
// 			if (!password.isEmpty())
// 			{
// 				QMessageBox::warning(this, tr("username error"), tr("please input right username"), QMessageBox::Ok);
// 				userNameLineEdit->clear();
// 				userNameLineEdit->setFocus();
// 			}
// 			
// 			//密码正确
// 			if (password == pwdLineEdit->text())
// 			{
// 				QDialog::accept();//
// 			}
// 			//密码错误
// 			else
// 			{
// 				QMessageBox::warning(this, tr("password error"), tr("please input right password"), QMessageBox::Ok);
// 				pwdLineEdit->clear();
// 				pwdLineEdit->setFocus();
// 			}
			//方法1(未实现，有问题)
			//QSqlQuery sql_query;
// 			while (sql_query.next())
// 			{
// 				usr_id = sql_query.value(0).toInt();
// 				usr_passwd = sql_query.value(1).toString();
// 				usr_name = sql_query.value(2).toString();
// 				usr_email = sql_query.value(3).toString();
// 				usr_history = sql_query.value(4).toInt();
// 
// 				qDebug() << QString("chatid=%1    passwd=%2     name=%3       email=%4    history=%5").arg(usr_id).arg(usr_passwd).arg(usr_name).arg(usr_email).arg(usr_history);
// 
// 			}
// 			QSqlQuery query;
// 			query.exec("select id, password from adminuser");
// 			while (query.next())
// 			{
// 				userName = query.value(0).toInt();
// 				password = query.value(1).toString();
// 				if (userName==userNameLineEdit->text()&&password==pwdLineEdit->text())
// 				{
// 					QDialog::accept();
// 				}
// 				//密码错误
// 				else if (userName == userNameLineEdit->text())
// 				{
// 					QMessageBox::warning(this, tr("password error"), tr("please input right password"), QMessageBox::Ok);
// 					pwdLineEdit->clear();
// 					pwdLineEdit->setFocus();
// 					//continue;
// 				}
// 				else//用户名错误
// 				{
// 					QMessageBox::warning(this, tr("username error"), tr("please input right username"), QMessageBox::Ok);
// 					userNameLineEdit->clear();
// 					userNameLineEdit->setFocus();
// 					continue;
// 				}
// 			}
			//方法2
			QSqlQuery queryUser;
			queryUser.exec("select id from adminuser");
			queryUser.next();

			QSqlQuery queryPwd;
			queryPwd.exec("select password from adminuser");
			queryPwd.next();
			//匹配
			if (queryPwd.value(0).toString() == pwdLineEdit->text() && queryUser.value(0).toString() == userNameLineEdit->text())
			{
				QDialog::accept();
			}
			else
			{//密码不正确
				if (queryPwd.value(0).toString() != pwdLineEdit->text())
				{
					QMessageBox::warning(this, tr("password error"), tr("please input right password"), QMessageBox::Ok);
					pwdLineEdit->clear();
					pwdLineEdit->setFocus();
				}
				else
				{
					QMessageBox::warning(this, tr("username error"), tr("please input right username"), QMessageBox::Ok);
					userNameLineEdit->clear();
					userNameLineEdit->setFocus();
				}
				
			}
		}

	}
		
    
}

void LoginDialog::quitBtnSlot()
{
    QDialog::reject();
}

LoginDialog::~LoginDialog()
{
	delete ui;
}