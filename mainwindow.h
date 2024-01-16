#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QCloseEvent>
#include <QFontDialog>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_textEdit_textChanged();
    void save();
    void saveAs();
    void print();

    void printPreviewDialog();
    void printPreview(QPrinter *printer);

    void exportPdf();
    void undo();
    void redo();

    void copy();
    void cut();
    void paste();

    void fontBold();
    void fontItalic();
    void fontUnderline();

    void right();
    void left();
    void center();
    void justify();

    void changeFont();
    void changeColor();
    void about();


private:
    Ui::MainWindow *ui;
    QString currentFileName;

};
#endif // MAINWINDOW_H
