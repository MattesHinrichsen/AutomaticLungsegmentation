#include "compare_menu.h"
#include "ui_compare_menu.h"

void Compare_Menu::run_comparison(){
    if(!ui->list_compare->selectedItems().empty()){
        int second_index = ui->list_compare->row(ui->list_compare->selectedItems().at(0));

        double value = (*segs)[first_index].seg.dice_score((*segs)[second_index].seg,ui->spinBox_value_one->value(), ui->spinBox_value_two->value());
        ui->value_label->setText("The DICE-score of the two scans is: " + QString::number(value));
    }
}

Compare_Menu::Compare_Menu(vector<Seg_Tuple>* s, int idx, QListWidget *parent) :
    QDialog(parent),
    ui(new Ui::Compare_Menu),
    segs(s),
    first_index(idx)

{
    ui->setupUi(this);
    for(int i=0; i<parent->count(); i++) {
        ui->list_compare->addItem(parent->item(i)->text());
    }
    changed_class_one();
    changed_class_two();
}

void Compare_Menu::changed_class_one() {
    string name = (*segs)[first_index].seg.get_cls().find_name(ui->spinBox_value_one->value());
    if(name != "") ui->label_class_one->setText("is: " + QString::fromStdString(name));
    else ui->label_class_one->setText("is: -");
}

void Compare_Menu::changed_class_two() {
    if(this->ui->list_compare->selectedItems().size() != 0) {
        int second_index = ui->list_compare->row(ui->list_compare->selectedItems().at(0));
        string name = (*segs)[second_index].seg.get_cls().find_name(ui->spinBox_value_two->value());
        if(name != "") ui->label_class_two->setText("is: " + QString::fromStdString(name));
        else ui->label_class_two->setText("is: -");
    } else ui->label_class_two->setText("is: -");
}

Compare_Menu::~Compare_Menu()
{
    delete ui;
}
