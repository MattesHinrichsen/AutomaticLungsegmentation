/**
 * @file scan_option_menu.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief A menu to set the name of a scan
 * 
 */

#ifndef SCAN_OPTION_MENU_H
#define SCAN_OPTION_MENU_H

#include <QDialog>
#include"../../Core/Nifti_container.cpp"

namespace Ui {
class scan_option_menu;
}

class scan_option_menu : public QDialog
{
    Q_OBJECT
    QString* name;

public:
    /**
     * @brief Construct a new scan option menu object
     * 
     * @param name Current name of the scan
     */
    explicit scan_option_menu(QString* name, QWidget *parent = nullptr);
    ~scan_option_menu();

private:
    Ui::scan_option_menu *ui;

private slots:
    /**
     * @brief Saves the changes and closes window
     * 
     */
    void save();
};

#endif // SCAN_OPTION_MENU_H
