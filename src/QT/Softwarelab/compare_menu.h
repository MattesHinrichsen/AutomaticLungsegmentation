/**
 * @file compare_menu.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief A menu to compare to scans against each other
 * 
 */

#ifndef COMPARE_MENU_H
#define COMPARE_MENU_H

#pragma once
#include <QListWidget>
#include <QDialog>
#include"../../Core/Nifti_container.cpp"

namespace Ui {
class Compare_Menu;
}

class Compare_Menu : public QDialog
{
    Q_OBJECT
    vector<Seg_Tuple>* segs;
    int first_index;

public:
    /**
     * @brief Construct a new Compare_Menu object
     * 
     * @param s Pointer to open segmentations
     * @param idx Index of the originally selected segmentation
     */
    explicit Compare_Menu(vector<Seg_Tuple>* s, int idx, QListWidget *parent = nullptr);
    ~Compare_Menu();

private slots:
    /**
     * @brief Compares the selected to the original scan
     * 
     */
    void run_comparison();

    /**
     * @brief Updates information in the UI when the selection for class one is changed
     * 
     */
    void changed_class_one();

    /**
     * @brief Updates information in the UI when the selection for class two is changed
     * 
     */
    void changed_class_two();

private:
    Ui::Compare_Menu *ui;
};

#endif // COMPARE_MENU_H
