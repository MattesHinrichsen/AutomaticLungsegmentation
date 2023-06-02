/**
 * @file batch_menu.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief A menu to set and run the batch mode. With additional information on result and success.
 * 
 */
#ifndef BATCH_MENU_H
#define BATCH_MENU_H

#include <QDialog>
#include "../../Core/batch_mode_qt.h"

namespace Ui {
class Batch_Menu;
}

class Batch_Menu : public QDialog
{
    Q_OBJECT
    QString directory_path = "";
    Batch_Mode batch;
    QString* open_menu_path;

public:
    /**
     * @brief Construct a new Batch_Menu object
     * 
     * @param omp Path in which the file explorer will be opened
     */
    explicit Batch_Menu(QString* omp, QWidget *parent = nullptr);
    ~Batch_Menu();

private slots:
    /**
     * @brief Implements the directory explorer
     * 
     */
    void load_batch();

    /**
     * @brief Runs the segmentation
     * 
     */
    void segment_batch();

    /**
     * @brief Counts and displays how many files have been selected by the user.
     * 
     */
    void update_count();

    /**
     * @brief Writes the progress informations send to the class during the batch run
     * 
     * @param num Number of Lungs found in scan
     * @param path Path of current scan
     */
    void write_debug(int num, string path);

private:
    Ui::Batch_Menu *ui;
};

#endif // BATCH_MENU_H
