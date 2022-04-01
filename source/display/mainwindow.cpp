#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Close program on Escape key
    if(event->key() == Qt::Key_Escape) {
         this->close();
    }
    else
        QWidget::keyPressEvent(event);
}
