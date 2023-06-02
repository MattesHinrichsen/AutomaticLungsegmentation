/**
 * @file load_scan_menu.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief A menu to load the scan and select wether to blur and scale.
 * 
 */

#ifndef LOAD_SCAN_MENU_H
#define LOAD_SCAN_MENU_H

#pragma once
#include <QDialog>
#include"../../Core/Nifti_container.cpp"
#include <QListWidget>
#include <QFileDialog>

namespace Ui {
class Load_Scan_Menu;
}

class Load_Scan_Menu : public QDialog
{
    Q_OBJECT
    vector<Nifti_container>* scans;
    QListWidget* list_scans;
    QString* lastSaved;

public:
    /**
     * @brief Construct a new Load_Scan_Menu object
     * 
     * @param s Pointer to th list of open Scans
     * @param lst Pointer to the list of all scans
     * @param last_path Path in which the file explorer will be opened
     */
    explicit Load_Scan_Menu(vector<Nifti_container>* s, QListWidget* lst, QString* last_path, QWidget *parent = nullptr);
    ~Load_Scan_Menu();

private slots:
    /**
     * @brief Enables the ability to set scale value
     * 
     */
    void enable_scale();

    /**
     * @brief Enables the ability to set blur value
     * 
     */
    void enable_blur();

    /**
     * @brief Opens file explorer and loads scan
     * 
     */
    void load_scan();

    /**
     * @brief Handles label on slider change
     *
     */
    void slider_changed();

private:
    Ui::Load_Scan_Menu *ui;
};

#endif // LOAD_SCAN_MENU_H
