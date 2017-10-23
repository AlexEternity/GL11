#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSize>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new GraphicView();
    view->setFixedHeight(this->height()*3);
    view->setFixedWidth(this->width()*5);
    this->setCentralWidget(view);
   // ui->gridLayout->addWidget(view);
    view->BuildMode = false;
    view->BezierMode = false;
    view->SplineMode = false;
    CreateMenu();
    CreateToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateMenu()
{
    QMenu *mFile = new QMenu("File");
        QAction *msgSave = mFile->addAction("Save");
        QAction *msgExit = mFile->addAction("Exit");

        connect(msgSave,  SIGNAL(triggered()), this, SLOT(on_save_triggered()));
        connect(msgExit,  SIGNAL(triggered()), this, SLOT(on_exit_triggered()));

    menuBar()->addMenu(mFile);
}

void MainWindow::CreateToolBar()
{
    QToolBar *bFile=new QToolBar();
        QAction *BuildMode = bFile->addAction("BuildMode");
        QAction *BezierT = bFile->addAction("BezierT");
        QAction *SplineT = bFile->addAction("SplineT");

        connect(BuildMode, SIGNAL(triggered()), SLOT(on_build_clicked()));
        connect(BezierT, SIGNAL(triggered()), SLOT(on_bezier_clicked()));
        connect(SplineT, SIGNAL(triggered()), SLOT(on_spline_clicked()));

        addToolBar(Qt::TopToolBarArea, bFile);
}

void MainWindow::mousePressEvent()
{
    return;
}

void MainWindow::on_save_triggered()
{
    sFile = QFileDialog::getSaveFileName(0, "Save Dialog", "", "*.bmp *.jpg *.png");
    if (sFile.isNull())
    {
      return;
    }
    QPixmap screen = QPixmap::grabWindow(centralWidget()->winId());
    screen.save(sFile,0,100);
}

void MainWindow::on_exit_triggered()
{
    close();
}

void MainWindow::on_bezier_clicked()
{
    if(view->BezierMode == false)
        view->BezierMode = true;
    else
        view->BezierMode = false;
}

void MainWindow::on_build_clicked()
{
    if(view->BuildMode == false)
       view-> BuildMode = true;
    else
        view->BuildMode = false;
}

void MainWindow::on_spline_clicked()
{
    if(view->SplineMode == false)
        view->SplineMode = true;
    else
        view->SplineMode = false;
}
