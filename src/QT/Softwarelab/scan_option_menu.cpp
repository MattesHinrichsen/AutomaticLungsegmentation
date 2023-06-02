#include "scan_option_menu.h"
#include "ui_scan_option_menu.h"

scan_option_menu::scan_option_menu(QString* name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scan_option_menu),
    name(name)
{
    ui->setupUi(this);
    ui->name_edit->setText(*name);
}


void scan_option_menu::save() {
    *name = ui->name_edit->text();
    this->close();
}

scan_option_menu::~scan_option_menu()
{
    delete ui;
}
