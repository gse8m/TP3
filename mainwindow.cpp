#include "mainwindow.h"
#include <iostream>
#include <string>
#include <vector>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  //
  // construct image_widget

  image_widget_ = new QLabel;
  image_widget_->setBackgroundRole(QPalette::Base);
  image_widget_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  image_widget_->setScaledContents(true);
  image_widget_->setMinimumSize(300, 300);

  create_actions();
  create_menus();

  setCentralWidget(image_widget_);
  resize(QGuiApplication::primaryScreen()->availableSize() * 2 / 5);

}


void MainWindow::create_actions() {
  //
  action_checker_board_image_ = new QAction(tr("Checker Board Image"), this);
  connect(action_checker_board_image_, SIGNAL(triggered()), this, SLOT( slot_load_checker_board_image() ));

  action_exit_ = new QAction(tr("E&xit"), this);
  action_exit_->setShortcut(tr("Ctrl+Q"));
  connect(action_exit_, SIGNAL(triggered()), this, SLOT( slot_exit() ));

  action_about_ = new QAction(tr("&About"), this);
  connect(action_about_, SIGNAL(triggered()), this, SLOT( slot_about() ));
}

void MainWindow::slot_exit() {
  close();
}

void MainWindow::slot_about() {
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The user can select several images to be displayed on the screen</p>"));
}


void MainWindow::create_menus() {
  menu_open_ = new QMenu(tr("&Open"), this);

  menu_open_->addAction(action_checker_board_image_);
  menu_open_->addAction(action_exit_);


  menu_help_ = new QMenu(tr("&Help"), this);
  menu_help_->addAction(action_about_);

  menuBar()->addMenu(menu_open_);
  menuBar()->addMenu(menu_help_);
}
const double x1=0.64, y1=0.33;  //Rouge
const double x2=0.30, y2=0.60;  //Verte
const double x3=0.15, y3=0.06;  //Bleu

bool MainWindow::Dans_Tri(const double x, const double y){
    //la line de BLEU a VERT  est : y = 3.6 * x-0.48
    //la line de BLEU a ROUGE est : y = (27/49) * x - (111/4900)
    //la line de VERT a ROUGE est : y = -(27/34) * x + (57/68)

    return (y <= 3.6 * x-0.48) && (y >= 0.551 * x - 0.02265 )
            && (y <= -0.79412 * x + 0.838235);
   // return 1;
}

int* MainWindow::RGB_Valeur(const double x, const double y){
    int RGB_V[3];
    int* p=RGB_V;
    double R, G, B;
    R =ceil(255*(((y2-y3)*(x-x3)+(x3-x2)*(y-y3))/((y2-y3)*(x1-x3)+(x3-x2)*(y1-y3))));
    G =ceil(255*(((y3-y1)*(x-x3)+(x1-x3)*(y-y3))/((y2-y3)*(x1-x3)+(x3-x2)*(y1-y3))));
    B =ceil(255 - R - G);
    int R_=(int)R;
    int G_=(int)G;
    int B_=(int)B;
    RGB_V[0]=R_;
    RGB_V[1]=G_;
    RGB_V[2]=B_;
    return p;
}


//
// Checker Board Triangle
//
const int checker_board_width = 600;
const int checker_board_height = 600;

void MainWindow::slot_load_checker_board_image() {

  // The ~10 lines of code below must be changed with your code
  // Create a QImage of required size
  // Draw a simple black/white checker board

  QImage checker_board_image(checker_board_width, checker_board_height, QImage::Format_RGB32);
  QPainter painter(&checker_board_image);
  painter.fillRect(checker_board_image.rect(), Qt::black); // draw a rectangle
  QPen pen(qRgb(51, 204, 102));

  painter.setPen(pen);
  QPoint P1(0,0), P2(0, checker_board_height),
          P3(checker_board_width, checker_board_height), P4(checker_board_width, 0);
  painter.drawLine(P1,P2);
  painter.drawLine(P2,P3);
  painter.drawLine(P3,P4);
  painter.drawLine(P4,P1);

  pen.setStyle(Qt::DotLine);
  painter.setPen(pen);
  for (int i = 0; i<checker_board_width; i += checker_board_width/10 )
      painter.drawLine(i, 0, i, checker_board_height);
  for (int i = 0; i<checker_board_height; i += checker_board_height/10 )
      painter.drawLine(0, i, checker_board_width, i);
  //checker_board_image.setPixelColor(300,300,RGB_Valeur(0.5,0.5));
  int *p_rgb=nullptr;

  double x=0.0;
  double y=0.0;
  for (int i=0; i<checker_board_width; i++){
      x++;
      for(int j=checker_board_height-1; j>=0; j--){
          y++;
          if(Dans_Tri(x/600,y/600)) {
              p_rgb=RGB_Valeur(x/600,y/600);
              checker_board_image.setPixelColor(i,j,qRgb(*p_rgb, *(p_rgb+1), *(p_rgb+2)));
          }
      }
      y=0.0;
  }

  //
  // rouge=(0.64*600, 0.33*600) RGB=(255, 0, 0) : x3,y3
  // vert= (0.3*600,0.6*600)    RGB=(0, 255, 0) : x1,y1
  // bleu= (0.15*600, 0.06*600) RGB=(0, 0, 255) : x2,y2
  //
  //  for (int i=0; i<checker_board_width; ++i) {
  //      pen.
  //  }
  //  for (int y = 0; y < checker_board_height; ++y) {
  //    for (int x = 0; x < checker_board_width; ++x) {
  //      if ((x ^ y) & 0x20) {
  //        checker_board_image.setPixel(x, y, qRgb(255, 255, 255));
  //      }
  //    }
  //  }
  //
  // You probably don't want to change these lines
  //

  image_widget_->setPixmap(QPixmap::fromImage(checker_board_image));
  image_widget_->setFixedSize(checker_board_width, checker_board_height);
  adjustSize();
}
