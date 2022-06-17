#include "window.hpp"
#include <iostream>


Window::Window()
  : timePlot(new QCustomPlot)
  , xyPlot(new QCustomPlot)
  , colorMap(new QCPColorMap(xyPlot->xAxis, xyPlot->yAxis))
{
  QSplitter * splitter = new QSplitter;
  splitter->addWidget(timePlot);
  splitter->addWidget(xyPlot);
  timePlot->show();
  xyPlot->show();
  setCentralWidget(splitter);

  timePlot->axisRect()->setupFullAxesBox(true);
  timePlot->setInteraction(QCP::iRangeZoom, true);
  timePlot->setInteraction(QCP::iRangeDrag, true);
  xyPlot->axisRect()->setupFullAxesBox(true);
  xyPlot->setInteraction(QCP::iRangeZoom, true);
  xyPlot->setInteraction(QCP::iRangeDrag, true);

  colorMap->setGradient(QCPColorGradient::gpGrayscale);
  colorMap->setDataRange(QCPRange(-1.0, 1.0));
  colorMap->data()->setSize(200, 200);
  colorMap->data()->fill(0.0);

  actions();

  resize(1700, 800);
  this->show();

  QString fileName = QFileDialog::getOpenFileName(this, tr("&Open File"),"/home/avs-es/Documents/C++", tr("*.txt *csv"));
}


void Window::actions()
{
  QMenu * file = new QMenu;
  file = menuBar()->addMenu(tr("&File"));

  QAction * open_action = new QAction(tr("&Open"));
  connect(open_action, &QAction::triggered, [this](){std::cout << "clicked\n";});
  file->addAction(open_action);

  QAction * save_action = new QAction(tr("&Save"));
  connect(save_action, &QAction::triggered, [this](){std::cout << "clicked\n";});
  file->addAction(save_action);

  QAction * save_as_action = new QAction(tr("&Save As"));
  connect(save_as_action, &QAction::triggered, [this](){std::cout << "clicked\n";});
  file->addAction(save_as_action);

  QAction * import_action = new QAction(tr("&Import"));
  connect(import_action, &QAction::triggered, [this](){std::cout << "clicked\n";});
  file->addAction(import_action);

  QAction * export_action = new QAction(tr("&Export"));
  connect(export_action, &QAction::triggered, [this](){std::cout << "clicked\n";});
  file->addAction(export_action);


  QMenu * view = new QMenu;
  view = menuBar()->addMenu(tr("&View"));

  QAction * split_screen_action = new QAction(tr("&Split"));
  connect(split_screen_action, &QAction::triggered, [this](){xyPlot->show();timePlot->show();resize(1700,800);});
  view->addAction(split_screen_action);

  QAction * xyplot_screen_action = new QAction(tr("&X-Y"));
  connect(xyplot_screen_action, &QAction::triggered, [this](){xyPlot->show();timePlot->hide();resize(800,800);});
  view->addAction(xyplot_screen_action);

  QAction * timeplot_screen_action = new QAction(tr("&Time"));
  connect(timeplot_screen_action, &QAction::triggered, [this](){xyPlot->hide();timePlot->show();resize(800,800);});
  view->addAction(timeplot_screen_action);


  QMenu * tools = new QMenu;
  tools = menuBar()->addMenu(tr("&Tools"));

  QAction * math_action = new QAction(tr("&Math"));
  connect(math_action, &QAction::triggered, [this](){std::cout << "math activated\n";});
  tools->addAction(math_action);
}


void Window::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  menu.exec(event->globalPos());
}
