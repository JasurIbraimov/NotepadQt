#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentFileName.clear();
    ui->textEdit->setText(QString());
    setWindowTitle(QString("Notepad - Untitled"));
}


void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select file to open");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, "Error", "Can't open file " + filePath);
        return;
    }

    currentFileName = filePath;
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    setWindowTitle(QString("Notepad - " + currentFileName));
    file.close();

}


void MainWindow::on_textEdit_textChanged()
{
    if(currentFileName.trimmed().isEmpty()) {
        setWindowTitle(QString("Notepad - Untitled*"));
    } else {
        setWindowTitle(QString("Notepad - " + currentFileName + "*"));
    }
}

void MainWindow::save()
{
    if(currentFileName.trimmed().isEmpty()) {
        emit saveAs();
    } else {
        QFile file(currentFileName);
        file.open(QIODevice::WriteOnly | QFile::Text);
        setWindowTitle(QString("Notepad - " + currentFileName));
        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.close();
    }
}

void MainWindow::saveAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save as...");
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::critical(this, "Error", "Can't save file " + filePath);
        return;
    }

    currentFileName = filePath;
    setWindowTitle(QString("Notepad - " + currentFileName));
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();

}

