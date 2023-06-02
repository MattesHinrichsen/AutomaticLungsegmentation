#include "load_scan_menu.h"
#include "ui_load_scan_menu.h"

Load_Scan_Menu::Load_Scan_Menu(vector<Nifti_container>* s, QListWidget* lst, QString* last_path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Load_Scan_Menu),
    scans(s),
    list_scans(lst),
    lastSaved(last_path)
{
    ui->setupUi(this);
    ui->blur_slider->setEnabled(false);
}

Load_Scan_Menu::~Load_Scan_Menu()
{
    delete ui;
}

void Load_Scan_Menu::enable_scale() {

    if(ui->scale_value->isEnabled()){
        ui->scale_value->setEnabled(false);
    }
    else{ui->scale_value->setEnabled(true);}
    return;
}

void Load_Scan_Menu::enable_blur() {

    if(ui->blur_slider->isEnabled()){
        ui->blur_slider->setEnabled(false);
        ui->blur->setText("Blur");
    }
    else{
        ui->blur_slider->setEnabled(true);
        slider_changed();
    }
}

void Load_Scan_Menu::slider_changed() {
    const QString labels[5] = {"Weakest", "Weak", "Medium", "Strong", "Strongest"};
    ui->blur->setText("Blur: " + labels[ui->blur_slider->value()]);
}

void Load_Scan_Menu::load_scan() {
    QString filepath = QFileDialog::getOpenFileName(this, "Choose file", *lastSaved, ("Scan (*nii *hdr)"));
    if(filepath != "") {
        *lastSaved = filepath.mid(0,filepath.lastIndexOf('/'));
        QString filename = filepath.mid(filepath.lastIndexOf("/")+1);
        Scan s;
        const float blur_value[5] = {0.6, 0.8, 1, 1.5, 3};
        s.read_from_file(filepath.toStdString(), ui->scale->isChecked(), ui->scale_value->value()/1000.0, ui->blur->isChecked(), blur_value[ui->blur_slider->value()]*0.14);
        Nifti_container cont;
        cont.name = filename;
        cont.scan = s;
        scans->push_back(cont);
        list_scans->addItem(filename);  //Should be changed
        this->close();
    }
    return;
}
