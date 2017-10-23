#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <QFileDialog>
#include <graphicsview.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CreateMenu();
    void CreateToolBar();


private slots:
    void on_build_clicked();
    void on_bezier_clicked();
    void on_spline_clicked();
    void on_save_triggered();
    void on_exit_triggered();
    virtual void mousePressEvent();

private:
    Ui::MainWindow *ui;
    GraphicView *view;
    QString sFile;
    QGraphicsItemGroup *Points;
    QGraphicsItemGroup *Lines;
};

#endif // MAINWINDOW_H
