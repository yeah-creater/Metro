#include "loginwidget.h" // 包含声明文件
#include "ui_loginwidget.h" // 包含由Qt Designer生成的实现文件

LoginWidget::LoginWidget(QWidget *parent) : QWidget(), ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
}

LoginWidget::~LoginWidget() {
    delete ui;
}


void LoginWidget::on_loginButton_clicked()
{
    QString username = this->ui->usernameEdit->text();
    QString password = this->ui->passwordEdit->text();
    this->ui->loginInfo->clear();
    //登录成功
    if (emit(login(username, password))) {
        this->hide();
    }
    else {
        this->ui->loginInfo->setText("账户或密码错误");
    }
}

void LoginWidget::on_skipButton_clicked()
{
    emit(skip());
    this->hide();
}


