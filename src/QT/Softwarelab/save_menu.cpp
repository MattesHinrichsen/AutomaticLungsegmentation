#include<QFileDialog>
#include "save_menu.h"
#include "ui_save_menu.h"

void Save_Menu::save() {

    QString filepath = QFileDialog::getSaveFileName(this, "Choose file");
    if(filepath != "") {
        string name = filepath.mid(filepath.lastIndexOf("/")+1, filepath.lastIndexOf(".")).toStdString();
        string location = filepath.mid(0, filepath.lastIndexOf("/")).toStdString();
        bool type_extension;
        if(ui->file_ending->currentText() == ".hdr") {
            type_extension = true;
        } else if(ui->file_ending->currentText() == ".nii") {
            type_extension = false;
        }

        string type = ui->data_type->currentText().toStdString();
        if(type == "unsigned int (8 bits)") {
            seg->write_to_file<uint8_t>(location, name, 2, type_extension);
            seg->get_cls().write<int>(location, name + ".cls");
        } else if(type == "short") {
            seg->write_to_file<short>(location, name, 4, type_extension);
            seg->get_cls().write<int>(location, name + ".cls");
        } else if(type == "int") {
            seg->write_to_file<int>(location, name, 8, type_extension);
            seg->get_cls().write<int>(location, name + ".cls");
        } else if(type == "float") {
            seg->write_to_file<float>(location, name, 16, type_extension);
            seg->get_cls().write<float>(location, name + ".cls");
        } else if(type == "double") {
            seg->write_to_file<double>(location, name, 64, type_extension);
            seg->get_cls().write<double>(location, name + ".cls");
        } else if(type == "int (8 bits)") {
            seg->write_to_file<int8_t>(location, name, 256, type_extension);
            seg->get_cls().write<int>(location, name + ".cls");
        } else if(type == "unsigned short") {
            seg->write_to_file<unsigned short>(location, name, 512, type_extension);
            seg->get_cls().write<unsigned int>(location, name + ".cls");
        } else if(type == "unsigned int") {
            seg->write_to_file<unsigned int>(location, name, 768, type_extension);
            seg->get_cls().write<unsigned int>(location, name + ".cls");
        } else if(type == "long") {
            seg->write_to_file<long>(location, name, 1024, type_extension);
            seg->get_cls().write<int>(location, name + ".cls");
        } else {
            throw invalid_argument("I mistyped something. Sry.");
        }

        this->close();
    }
    return;
}

Save_Menu::Save_Menu(Segmentation* s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Save_Menu),
    seg(s)
{
    ui->setupUi(this);
}

Save_Menu::~Save_Menu()
{
    delete ui;
}
