/**
 * @file seg_option_menu.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief A menu to set the name and displayed classes if a segmentation
 * 
 */

#ifndef SEG_OPTION_MENU_H
#define SEG_OPTION_MENU_H

#include <QDialog>
#include<vector>
#include <set>
using namespace std;

namespace Ui {
class seg_option_menu;
}

class seg_option_menu : public QDialog
{
    Q_OBJECT
    QString* name;
    set<int>* classes;

public:
    /**
     * @brief Construct a new seg option menu object
     * 
     * @param name Current name of segmentation
     * @param classes  Currently displayed classes
     */
    explicit seg_option_menu(QString* name, set<int>* classes, QWidget *parent = nullptr);
    ~seg_option_menu();

private:
    Ui::seg_option_menu *ui;

private slots:
    /**
     * @brief Saves changes
     * 
     */
    void save();
};

#endif // SEG_OPTION_MENU_H
