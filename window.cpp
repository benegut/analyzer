#include "window.hpp"


Window::Window()
  : splitter(new QSplitter)
  , timePlot(new QCustomPlot)
  , xyPlot(new QCustomPlot)
  , colorMap(new QCPColorMap(xyPlot->xAxis, xyPlot->yAxis))
{
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
}


void Window::actions()
{
  QAction * split_screen_action = new QAction(tr("&Split"));
  connect(split_screen_action, &QAction::triggered, [this](){xyPlot->show();timePlot->show();resize(1700,800);});

  QAction * xyplot_screen_action = new QAction(tr("&X-Y"));
  connect(xyplot_screen_action, &QAction::triggered, [this](){xyPlot->show();timePlot->hide();resize(800,800);});

  QAction * timeplot_screen_action = new QAction(tr("&Time"));
  connect(timeplot_screen_action, &QAction::triggered, [this](){xyPlot->hide();timePlot->show();resize(800,800);});

  QMenu * view = new QMenu;
  view = menuBar()->addMenu(tr("&View"));
  view->addAction(split_screen_action);
  view->addAction(xyplot_screen_action);
  view->addAction(timeplot_screen_action);
}


void Window::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  menu.exec(event->globalPos());
}
