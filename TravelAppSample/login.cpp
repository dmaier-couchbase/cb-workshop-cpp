#include "login.h"
#include "ui_login.h"
#include "cbdatasourcefactory.h"
#include "cbdatasource.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

bool Login::createUserOrLogin()
{
    QJsonObject result = CBDataSourceFactory::Instance().GetJsonObject("user::" + ui->lnEdUsername->text());
    if (ui->cbCreateNewUser->isChecked())
    {
        // check if user exists
        if (!result.contains("username"))
        {
            // create
            result["username"] = ui->lnEdUsername->text();
            result["password"] = ui->lnEdpassword->text();
            CBDataSourceFactory::Instance().Upsert("user::" + ui->lnEdUsername->text(), result);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (result.contains("username"))
        {
            if (result["password"] == ui->lnEdpassword->text())
            {
                return true;
            }
        }
        return false;
    }
}

QString Login::userName()
{
    return ui->lnEdUsername->text();
}

Login::~Login()
{
    delete ui;
}
