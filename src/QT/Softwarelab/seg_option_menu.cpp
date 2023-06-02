#include "seg_option_menu.h"
#include "ui_seg_option_menu.h"
#include <algorithm>
#include <sstream>
#include <set>
#include <QRegularExpressionValidator>

seg_option_menu::seg_option_menu(QString* name, set<int>* classes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::seg_option_menu),
    classes(classes),
    name(name)
{
    ui->setupUi(this);
    ui->class_edit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[-,0-9 ]+$"), ui->class_edit));
    ui->name_edit->setText(*name);
    QString cls;
    if (!classes->empty()) {
        for(auto it = classes->begin(); it != --classes->end(); it++) {
            cls.append(QString("%1, ").arg(*it));
        }
        cls.append(QString("%1").arg(*(--classes->end())));
    }
    ui->class_edit->setText(cls);
}

void seg_option_menu::save() {
    *name = ui->name_edit->text();


    auto plain_txt = ui->class_edit->text().toStdString();
    stringstream ss(plain_txt);
    classes->clear();
    if (plain_txt != "") {
        while( ss.good() )
        {
            string substr;
            getline( ss, substr, ',' );         //Split on ','
            substr.erase(std::remove_if(substr.begin(), substr.end(), [](unsigned char x){return std::isspace(x);}), substr.end()); //Remove whitespaces
            if (substr != "") classes->insert( stoi(substr) );
        }
    }


    this->close();
}

seg_option_menu::~seg_option_menu()
{
    delete ui;
}
