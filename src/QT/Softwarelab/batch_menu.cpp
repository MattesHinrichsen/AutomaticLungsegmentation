#include <QFileDialog>
#include <QProgressBar>
#include "batch_menu.h"
#include "ui_batch_menu.h"
#include <algorithm>

void Batch_Menu::load_batch() {
    QString path = QFileDialog::getExistingDirectory(this, "Choose file", *open_menu_path);
    if(path != "") {
        *open_menu_path = path;
        QString directoryname = path.mid(path.lastIndexOf("/")+1);
        ui->directory->setText(directoryname);
        directory_path = path;
        ui->number_files->clear();
    }
}


void Batch_Menu::update_count() {

    auto plain_txt = ui->scan_names->text().toStdString();
    stringstream ss(plain_txt);
    vector<string> names;

    while( ss.good() )
    {
        string substr;
        getline( ss, substr, ',' );         //Split on ','
        substr.erase(std::remove_if(substr.begin(), substr.end(), [](unsigned char x){return std::isspace(x);}), substr.end()); //Remove whitespaces
        names.push_back( substr );
    }


    int value;
    if (directory_path != "") value = batch.count_entries(directory_path.toStdString(), names , ui->include_subdirectories->isChecked());
    else value = 0;

    ui->number_files->setText("Found " + QString::number(value) + " files matching your input.");
    return;
}


void Batch_Menu::segment_batch() {
    if (directory_path != "") {
        ui->progressBar->show();
        ui->debug_output_box->show();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->count_button->setEnabled(false);
        ui->load_button->setEnabled(false);
        auto plain_txt = ui->scan_names->text().toStdString();
        stringstream ss(plain_txt);
        vector<string> names;

        this->repaint();

        while( ss.good() )
        {
            string substr;
            getline( ss, substr, ',' );         //Split on ','
            substr.erase(std::remove_if(substr.begin(), substr.end(), [](unsigned char x){return std::isspace(x);}), substr.end()); //Remove whitespaces
            names.push_back( substr );
        }

        bool out_type = true;
        if(ui->file_ending->currentText() == ".nii") out_type = false;

        string type = ui->data_type->currentText().toStdString();

        if(type == "unsigned int (8 bits)") {
            batch.run_batch_mode<uint8_t>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 2,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else if(type == "short") {
            batch.run_batch_mode<short>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 4,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else if(type == "int") {
            batch.run_batch_mode<int>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 8,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else if(type == "float") {
            batch.run_batch_mode<float>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 16,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else if(type == "double") {
            batch.run_batch_mode<double>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 64,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else if(type == "int (8 bits)") {
            batch.run_batch_mode<int8_t>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 256,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else if(type == "unsigned short") {
            batch.run_batch_mode<unsigned short>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 512,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else if(type == "unsigned int") {
            batch.run_batch_mode<unsigned int>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 768,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else if(type == "long") {
            batch.run_batch_mode<long>(directory_path.toStdString(), names , ui->include_subdirectories->isChecked(), 1024,
                                         out_type, ui->cores->value(), ui->threshold->value(),
                                         ui->lower_volume_threshold->value(), ui->upper_volume_threshold->value());
        } else {
            throw invalid_argument("Wrong type selected.");
        }
        ui->debug_output_box->setTextColor(Qt::black);
        ui->debug_output_box->insertPlainText("Done");
    }
    return;
}

void Batch_Menu::write_debug(int num, string path) {
    if(ui->No_lung_selector->currentIndex() == 0) {
        if(num<=0) ui->debug_output_box->setTextColor(Qt::red);
        else ui->debug_output_box->setTextColor(Qt::black);
    } else if(ui->No_lung_selector->currentIndex() == 1) {
        if(num != 1) ui->debug_output_box->setTextColor(Qt::red);
        else ui->debug_output_box->setTextColor(Qt::black);
    } else if(ui->No_lung_selector->currentIndex() == 2) {
        if(num != 2) ui->debug_output_box->setTextColor(Qt::red);
        else ui->debug_output_box->setTextColor(Qt::black);
    } else if(ui->No_lung_selector->currentIndex() == 3) {
        if(num != 3) ui->debug_output_box->setTextColor(Qt::red);
        else ui->debug_output_box->setTextColor(Qt::black);
    } else if(ui->No_lung_selector->currentIndex() == 4) {
        if(num < 4) ui->debug_output_box->setTextColor(Qt::red);
        else ui->debug_output_box->setTextColor(Qt::black);
    }

    QString out = QString("Found %1 lung(s) in the scan:\n%2\n\n").arg(QString::number(num), QString::fromStdString(path).replace("\\", "/"));
    ui->debug_output_box->insertPlainText(out);
}


Batch_Menu::Batch_Menu(QString* omp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Batch_Menu),
    open_menu_path(omp)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    this->setWindowFlag(Qt::WindowMaximizeButtonHint, true);

    connect(&batch, SIGNAL(percentageCompleted(int)), ui->progressBar, SLOT(setValue(int)));
    connect(&batch, SIGNAL(send_num_found(int, string)), this, SLOT(write_debug(int, string)));
    ui->progressBar->hide();
    ui->debug_output_box->hide();
    this->setStyleSheet("QToolTip {\n    border: 1px solid black;\n	background-color: rgb(255, 255, 255);\n    padding: 5px;\n    border-radius: 3px;\n    opacity: 200;\n}");
}

Batch_Menu::~Batch_Menu()
{
    delete ui;
}
