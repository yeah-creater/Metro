// LoginController.h
#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QObject>

class LoginModel;
class LoginWidget;

class LoginController : public QObject {
    Q_OBJECT

public:
    LoginController(LoginWidget *view, LoginModel *model, QObject *parent = nullptr);

private slots:
    void onLoginClicked(const QString &username, const QString &password);

private:
    LoginWidget *m_view;
    LoginModel *m_model;
};

#endif // LOGINCONTROLLER_H
