#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    // Save, save as
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAs);
    // Print, printPreview
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::print);
    connect(ui->actionPrint_Preview, &QAction::triggered, this, &MainWindow::printPreviewDialog);
    // Export PDF
    connect(ui->actionExport_to_PDF, &QAction::triggered, this, &MainWindow::exportPdf);
    // Quit
    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);
    // Undo, Redo
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    // Cut, copy, paste
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    // Bold, Italic, Underline
    connect(ui->actionBold, &QAction::triggered, this, &MainWindow::fontBold);
    connect(ui->actionItalic, &QAction::triggered, this, &MainWindow::fontItalic);
    connect(ui->actionUnderline, &QAction::triggered, this, &MainWindow::fontUnderline);
    // Left, Right, Center, Justify
    connect(ui->actionCenter, &QAction::triggered, this, &MainWindow::center);
    connect(ui->actionRight, &QAction::triggered, this, &MainWindow::right);
    connect(ui->actionLeft, &QAction::triggered, this, &MainWindow::left);
    connect(ui->actionJustify, &QAction::triggered, this, &MainWindow::justify);
    // Color, Font, About
    connect(ui->actionFont, &QAction::triggered, this, &MainWindow::changeFont);
    connect(ui->actionColor, &QAction::triggered, this, &MainWindow::changeColor);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(windowTitle().contains('*')) {
        int result = QMessageBox::warning(this, QString("Warning!"), QString("Are you sure closing Notepad? You have unsaved file."), QMessageBox::Cancel | QMessageBox::Ok);

        if(result == QMessageBox::Ok) {
            event->accept();
        }   else {
            event->ignore();
        }
    }

}

void MainWindow::on_actionNew_triggered()
{
    currentFileName.clear();
    ui->textEdit->setText(QString());
    setWindowTitle(QString("Notepad - Untitled"));
}


void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select file to open", QDir::homePath(), "Text Files (*.txt);");
    if(filePath.trimmed() == QString()) {
        return;
    }
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
        if(ui->textEdit->toPlainText().trimmed().isEmpty()) {
            setWindowTitle(QString("Notepad - Untitled"));
        } else {
            setWindowTitle(QString("Notepad - Untitled*"));
        }
    } else {
        setWindowTitle(QString("Notepad - " + currentFileName + "*"));
    }

    QTextDocument* document = ui->textEdit->document();
    ui->actionUndo->setEnabled(document->isUndoAvailable());
    ui->actionRedo->setEnabled(document->isRedoAvailable());

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
    QString filePath = QFileDialog::getSaveFileName(this, "Save as...",  QDir::homePath(), "Text Files (*.txt);");
    if(filePath.trimmed() == QString()) {
        return;
    }
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

void MainWindow::print()
{
    QPrinter printDev;
    QPrintDialog dialog(&printDev, this);
    if(dialog.exec() == QDialog::Rejected) {
        return;
    }
    ui->textEdit->print(&printDev);

}

void MainWindow::printPreviewDialog()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog previewDialog(&printer, this);
    previewDialog.setWindowFlags(Qt::Window);
    connect(&previewDialog, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter *)));
    previewDialog.exec();
}


void MainWindow::printPreview(QPrinter *printer)
{
    ui->textEdit->print(printer);
}

void MainWindow::exportPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF...", QDir::homePath(), "PDF Files (*.pdf);");
    if(fileName.trimmed() != QString()) {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        printer.setPageMargins(QMarginsF(10, 10, 10, 10));
        ui->textEdit->document()->print(&printer);
    }
}

void MainWindow::undo()
{
    QTextDocument* document = ui->textEdit->document();
    if(document->isUndoAvailable()) {
        ui->textEdit->undo();
    }
}

void MainWindow::redo()
{
    QTextDocument* document = ui->textEdit->document();
    if(document->isRedoAvailable()) {
        ui->textEdit->redo();
    }
}

void MainWindow::copy()
{
    ui->textEdit->copy();
}

void MainWindow::cut()
{
    ui->textEdit->cut();
}

void MainWindow::paste()
{
    ui->textEdit->paste();
}

void MainWindow::fontBold()
{
    ui->textEdit->setFocus();
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection()) {
        return;
    }

    QTextCharFormat charFormat = cursor.charFormat();
    if(charFormat.fontWeight() == QFont::Bold) {
        charFormat.setFontWeight(QFont::Normal);
    }else {
        charFormat.setFontWeight(QFont::Bold);
    }

    cursor.mergeCharFormat(charFormat);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::fontItalic()
{
    ui->textEdit->setFocus();
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection()) {
        return;
    }

    QTextCharFormat charFormat = cursor.charFormat();

    charFormat.setFontItalic(!charFormat.fontItalic());
    cursor.mergeCharFormat(charFormat);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::fontUnderline()
{
    ui->textEdit->setFocus();
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection()) {
        return;
    }

    QTextCharFormat charFormat = cursor.charFormat();
    charFormat.setFontUnderline(!charFormat.fontUnderline());
    cursor.mergeCharFormat(charFormat);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::right()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignRight);
}

void MainWindow::left()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignLeft);
}

void MainWindow::center()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
}

void MainWindow::justify()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignJustify);
}

void MainWindow::changeFont()
{
    bool ok;
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection()) {
        return;
    }

    QFont font = QFontDialog::getFont(&ok, this);
    if(ok) {
        ui->textEdit->setFocus();
        QTextCharFormat charFormat = cursor.charFormat();
        charFormat.setFont(font);
        cursor.mergeCharFormat(charFormat);
        ui->textEdit->setTextCursor(cursor);
    }
}

void MainWindow::changeColor()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection()) {
        return;
    }
    QColorDialog colorDialog;
    QColor color = colorDialog.getColor();
    ui->textEdit->setFocus();
    QTextCharFormat charFormat = cursor.charFormat();
    charFormat.setForeground(color);
    cursor.mergeCharFormat(charFormat);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::about()
{
    QMessageBox::about(this, "About Notepad", "This is a Notepad application where you can open, edit and save text files. Also you can export files in PDF format and print them if you want. This app provides different stylings for text content like fonts, colors, text types such as bold, italic, underline and text alignments too.");
}




