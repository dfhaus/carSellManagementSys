#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
//以下代码用于解决中文出现乱码问题
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#include <QDialog>
#include <QPushButton>
#include<QLineEdit>
#include<QLabel>
#include <QLayout>
#include <QGridLayout>



class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    
private slots:
	void loginBtnSlot();

    void quitBtnSlot();

private:
    QDialog *ui;
	QPushButton *loginBtn;
	QLabel *spaceLabel;
	QPushButton *quitBtn;
	QLineEdit *userNameLineEdit;
	QLineEdit *pwdLineEdit;
	QLabel *userNameLabel;
	QLabel *pwdLabel;
	QGridLayout *mainLayout;
	QString password;
	QString userName;
};

#endif // LOGINDIALOG_H
