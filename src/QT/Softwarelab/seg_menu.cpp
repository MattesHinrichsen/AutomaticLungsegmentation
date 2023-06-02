#include "seg_menu.h"
#include "ui_seg_menu.h"

void Seg_Menu::segment() {
    n_container->segmentations.push_back({n_container->scan.create_segmentation(ui->cores->value(), ui->threshold->value(), ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value()), "Seg_" + name});
    main_list->addItem("Seg_" + name);   //Should be changed
    this->close();
}

Seg_Menu::Seg_Menu(Nifti_container* nc, QListWidget* ml, QString n, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Seg_Menu),
    n_container(nc),
    main_list(ml),
    name(n)
{
    ui->setupUi(this);
}

Seg_Menu::~Seg_Menu()
{
    delete ui;
}
