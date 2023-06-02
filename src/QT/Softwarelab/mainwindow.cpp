#include <QFileDialog>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "seg_menu.h"
#include "save_menu.h"
#include "batch_menu.h"
#include "compare_menu.h"
#include "load_scan_menu.h"
#include "seg_option_menu.h"
#include "scan_option_menu.h"
#include "QImage"
#include "QColor"
#include "QSlider"
#include "QPixmap"
#include <QWheelEvent>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>

//std::vector<int> v;
//int N = 500;
//Scan scan;
//Segmentation seg;

void MainWindow::close_scan() {
    if(!ui->list_scans->selectedItems().empty()){
        int tmp_row = scan_row;
        ui->list_scans->setCurrentRow(-1);
        scan_vec.erase(scan_vec.begin() + tmp_row);
        ui->list_scans->takeItem(tmp_row);
        ui->list_scans->setCurrentRow(tmp_row - 1);
    }
    return;
}

void MainWindow::load_seg() {

    QString filepath = QFileDialog::getOpenFileName(this, "Choose file", lastSaved, ("Scan(*nii *hdr)"));
    if(filepath != "") {
        lastSaved = filepath.mid(0,filepath.lastIndexOf('/'));
        QString filename = filepath.mid(filepath.lastIndexOf("/")+1);
        Segmentation s;
        s.read_from_file(filepath.toStdString());
        s.get_cls().read(filepath.left(filepath.lastIndexOf(".")).toStdString() + ".cls");
        scan_vec[scan_row].segmentations.push_back({s, filename});
        update_seg_list();
    }
    return;
}

void MainWindow::close_seg() {

    if(!ui->list_seg->selectedItems().empty()){
        int row = ui->list_seg->row(ui->list_seg->selectedItems().at(0));
        ui->list_seg->takeItem(row);
        scan_vec[scan_row].segmentations.erase(scan_vec[scan_row].segmentations.begin() + row);
        scan_vec[scan_row].seg_index = -1;
        update_seg_list();
        change_display();
    }
    return;
}

void MainWindow::open_load_menu() {

    auto menu = new Load_Scan_Menu(&scan_vec, ui->list_scans, &lastSaved);
    menu->show();
    return;
}

void MainWindow::open_seg_menu() {

    if(!ui->list_scans->selectedItems().empty()) {
        auto menu = new Seg_Menu(&scan_vec[scan_row], ui->list_seg, ui->list_scans->currentItem()->text());
        menu->show();
        update_seg_list();
    }
    return;
}

void MainWindow::open_save_menu() {
    if(!ui->list_seg->selectedItems().empty()){
        auto menu = new Save_Menu(&scan_vec[scan_row].segmentations[ui->list_seg->currentRow()].seg);
        menu->show();
    }
    return;
}

void MainWindow::open_batch_menu() {

    auto menu = new Batch_Menu(&lastSaved);
    menu->show();
    return;
}

void MainWindow::open_compare_menu() {
    if(!ui->list_seg->selectedItems().empty()){
        auto menu = new Compare_Menu(&scan_vec[scan_row].segmentations, ui->list_seg->row(ui->list_seg->selectedItems().at(0)), ui->list_seg);
        menu->show();
    }
    return;
}

void MainWindow::resizeEvent(QResizeEvent* ev) {
    change_display();
}


void MainWindow::displayImage(int layer, Scan* scan, Segmentation* seg) {
    ui->slider_label->show();
    ui->layer_slider->show();
    float alpha = 0.5;
    ui->slider_label->setNum(layer);
    ui->layer_slider->setSliderPosition(layer);
    QImage image(scan->get_hdr().dim[2], scan->get_hdr().dim[1], QImage::Format_RGB32);
    QColor value;
    if(seg != nullptr) {
        auto color_map = seg->get_cls().get_color_map();
        for (int i=0;i<scan->get_hdr().dim[2];++i) {
            for (int j=0;j<scan->get_hdr().dim[1];++j) {
                float scan_val = std::min((scan->get_data()[layer][i][j] + 1000), (float)3000)/3000*255;
                float seg_data = seg->get_data()[layer][i][j];

                auto& cls_set = scan_vec[scan_row].segmentations[scan_vec[scan_row].seg_index].classes;
                if((cls_set.empty() && seg_data) || cls_set.count( seg_data)) {
                    if(color_map.count(seg_data)) {
                        value = QColor(alpha*scan_val + (1-alpha)*color_map[seg_data][0], alpha*scan_val + (1-alpha)*color_map[seg_data][1], alpha*scan_val + (1-alpha)*color_map[seg_data][2]);
                    } else {
                        value = QColor(alpha*scan_val + (1-alpha)*255, alpha*scan_val, alpha*scan_val);
                    }
                } else {
                    value = QColor(scan_val, scan_val, scan_val);
                }
                image.setPixel(i, j, value.rgb());
            }
        }
    } else {
        for (int i=0;i<scan->get_hdr().dim[2];++i) {
            for (int j=0;j<scan->get_hdr().dim[1];++j) {
                float scan_val = std::min((scan->get_data()[layer][i][j] + 1000), (float)3000)/3000*255;
                value = QColor(scan_val, scan_val, scan_val);
                image.setPixel(i, j, value.rgb());
            }
        }
    }
    ui->label_pic->setPixmap(QPixmap::fromImage(image.scaled(ui->label_pic->width(), ui->label_pic->height(), Qt::KeepAspectRatio)));

}

void MainWindow::update_seg_list() {
    ui->list_seg->clear();
    if(!ui->list_scans->selectedItems().empty()){
        for(auto i : scan_vec[scan_row].segmentations) {
            ui->list_seg->addItem(i.name);
        }
    }
    if (scan_vec[scan_row].seg_index > -1) {
        ui->list_seg->setCurrentRow(scan_vec[ui->list_scans->currentRow()].seg_index);
    }

}

void MainWindow::update_scan_list() {
    ui->list_scans->clear();
    for(auto i : scan_vec) {
        ui->list_scans->addItem(i.name);
    }
    ui->list_scans->setCurrentRow(scan_row);


}

void MainWindow::scan_selection_change() {
    if(!ui->list_scans->selectedItems().empty()){
        scan_row = ui->list_scans->row(ui->list_scans->selectedItems().at(0));
        update_seg_list();
        ui->close_seg_button->setEnabled(true);
        ui->compare_button->setEnabled(true);
        ui->load_seg_button->setEnabled(true);
        ui->save_seg_button->setEnabled(true);
    } else {
        ui->list_seg->clear();
        ui->close_seg_button->setEnabled(false);
        ui->compare_button->setEnabled(false);
        ui->load_seg_button->setEnabled(false);
        ui->save_seg_button->setEnabled(false);
    }
}

void MainWindow::seg_selection_changed() {
    if(!ui->list_seg->selectedItems().empty()){
        scan_vec[scan_row].seg_index = ui->list_seg->currentRow();
        change_display();
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->layer_slider->hide();
    scan_selection_change();
    //this->setStyleSheet("QToolTip {\n    border: 2px solid black;\n	background-color: rgb(255, 255, 255);\n    padding: 5px;\n    border-radius: 3px;\n    opacity: 200;\n}");
}

void MainWindow::slider_changed() {
    scan_vec[ui->list_scans->currentRow()].slice = ui->layer_slider->value();
    change_display();
}


void MainWindow::change_display() {
    //cout << ui->list_scans->selectedItems().empty() << endl;
    if (!ui->list_scans->selectedItems().empty()) {
        scan_row = ui->list_scans->row(ui->list_scans->selectedItems().at(0));
        int layer = scan_vec[scan_row].slice;

        Scan* scan = &scan_vec[scan_row].scan;
        if(layer>=scan->get_hdr().dim[3]) layer=scan->get_hdr().dim[3]-1;
        ui->layer_slider->setMaximum(scan->get_hdr().dim[3]-1);
        if (scan_vec[scan_row].seg_index > -1) {   //Overlay
            Segmentation* seg = &scan_vec[scan_row].segmentations[scan_vec[scan_row].seg_index].seg;
            if(scan->get_hdr().dim[1] == seg->get_hdr().dim[1] && scan->get_hdr().dim[2] == seg->get_hdr().dim[2] && scan->get_hdr().dim[3] == seg->get_hdr().dim[3]) {
                displayImage(layer, scan, seg);
            } else { //Only scans
                displayImage(layer, scan, nullptr);
            }
        } else {
            displayImage(layer, scan, nullptr);
        }
    } else { //Hide everything
        ui->label_pic->clear();
        ui->slider_label->setText("");
        ui->slider_label->hide();
        //ui->layer_slider->setSliderPosition(0);
        ui->layer_slider->hide();
    }
}

void MainWindow::double_clicked_seg() {
    auto menu = new seg_option_menu(&scan_vec[scan_row].segmentations[scan_vec[scan_row].seg_index].name, &scan_vec[scan_row].segmentations[scan_vec[scan_row].seg_index].classes, this);
    menu->exec();
    delete menu;
    option_menu_closed();
}

void MainWindow::option_menu_closed() {
    update_seg_list();
    update_scan_list();
    change_display();
}

void MainWindow::double_clicked_scan() {
    auto menu = new scan_option_menu(&scan_vec[scan_row].name, this);
    menu->exec();
    delete menu;
    option_menu_closed();
}


MainWindow::~MainWindow()
{
    delete ui;
}

