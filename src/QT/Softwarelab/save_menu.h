/**
 * @file save_menu.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief A menu to save a segmentation to file
 * 
 */
#ifndef SAVE_MENU_H
#define SAVE_MENU_H

#pragma once
#include <QDialog>
#include <string>
#include"../../Core/Segmentation.h"
#include <QListWidget>

namespace Ui {
class Save_Menu;
}

class Save_Menu : public QDialog
{
    Q_OBJECT
    Segmentation* seg;


public:
    /**
     * @brief Construct a new Save_Menu object
     * 
     * @param s Pointer to segmetation to save
     */
    explicit Save_Menu(Segmentation* s, QWidget *parent = nullptr);
    ~Save_Menu();

private slots:
    /**
     * @brief Saves the segmentation
     * 
     */
    void save();

private:
    Ui::Save_Menu *ui;
};

#endif // SAVE_MENU_H
