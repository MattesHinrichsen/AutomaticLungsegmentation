/**
 * @file seg_menu.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief A menu to select parameters for and run a segmentation
 * 
 */
#ifndef SEG_MENU_H
#define SEG_MENU_H

#pragma once
#include <QDialog>
#include"../../Core/Segmentation.h"
#include"../../Core/Scan.h"
#include"../../Core/Nifti_container.cpp"
#include <QListWidget>

namespace Ui {
class Seg_Menu;
}

class Seg_Menu : public QDialog
{
    Q_OBJECT
    Nifti_container* n_container;
    QListWidget* main_list;
    QString name;


public:
    /**
     * @brief Construct a new Seg_Menu object
     * 
     * @param nc Pointer to current Nifti container
     * @param ml Pointer to current list of segmentations
     * @param n Name of the current scan
     */
    explicit Seg_Menu(Nifti_container* nc, QListWidget* ml, QString n, QWidget *parent = nullptr);
    ~Seg_Menu();

private slots:
    /**
     * @brief Runs the segmentation with the set parameters
     * 
     */
    void segment();

private:
    Ui::Seg_Menu *ui;
};

#endif // SEG_MENU_H
