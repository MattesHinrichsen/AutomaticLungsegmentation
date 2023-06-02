/**
 * @file mainwindow.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief The main window of the application
 * 
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include"../../Core/Nifti_container.cpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    vector<Nifti_container> scan_vec;
    int scan_row;

    QString lastSaved = "C:/";

public:
    /**
     * @brief Construct a new Main Window object
     * 
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /**
     * @brief Handles the resizing of the window
     * 
     * @param ev Event pointer to get new dimensions
     */
    virtual void resizeEvent(QResizeEvent* ev);

    /**
     * @brief Updates the list of segmentations when changed
     * 
     */
    void update_seg_list();

private slots:
    /**
     * @brief Closes a scan
     * 
     */
    void close_scan();

    /**
     * @brief Opens a file explorer and loads selected segmentation
     * 
     */
    void load_seg();

    /**
     * @brief Closes a segmentation
     * 
     */
    void close_seg();

    /**
     * @brief Opens the load scan menu
     * 
     */
    void open_load_menu();

    /**
     * @brief Opens the segmentation menu
     * 
     */
    void open_seg_menu();

    /**
     * @brief Opens the save menu
     * 
     */
    void open_save_menu();

    /**
     * @brief Opens the batch menu
     * 
     */
    void open_batch_menu();

    /**
     * @brief Opens the compare menu
     * 
     */
    void open_compare_menu();

    /**
     * @brief Figures out if and what to display in the right image
     * 
     */
    void change_display();

    /**
     * @brief Calculates the image to display on the right
     * 
     * @param layer Layer of the scan to display
     * @param scan Pointer to scan to display
     * @param seg Pointer to segmentation to overlay
     */
    void displayImage(int layer, Scan* scan, Segmentation* seg);

    /**
     * @brief Handles changes to UI when the selected scan was changed
     * 
     */
    void scan_selection_change();

    /**
     * @brief Handles changes to the slider
     * 
     */
    void slider_changed();

    /**
     * @brief  Handles changes to UI when the selected segmentation was changed
     * 
     */
    void seg_selection_changed();

    /**
     * @brief Opens the options menu for segmentations
     * 
     */
    void double_clicked_seg();

    /**
     * @brief Opens the options menu for scans
     * 
     */
    void double_clicked_scan();

    /**
     * @brief Updates UI after options have been changed
     * 
     */
    void option_menu_closed();

    /**
     * @brief Updates the list of scans when changed
     * 
     */
    void update_scan_list();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
