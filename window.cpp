#include "window.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

Window::Window()
  : timePlot(new QCustomPlot)
  , xyPlot(new QCustomPlot)
  , colorMap(new QCPColorMap(xyPlot->xAxis, xyPlot->yAxis))
  , plotcontextmenu(new PlotContextMenu(timePlot, this))
  , lowerLine(new QCPItemStraightLine(timePlot))
  , upperLine(new QCPItemStraightLine(timePlot))
{
  QSplitter * splitter = new QSplitter;
  splitter->addWidget(timePlot);
  splitter->addWidget(xyPlot);
  timePlot->show();
  xyPlot->show();
  setCentralWidget(splitter);

  timePlot->setInteractions(QCP::iSelectPlottables);
  connect(timePlot, &QCustomPlot::plottableClick, this, &Window::plottableClick_slot);
  connect(timePlot, &QCustomPlot::mouseDoubleClick, this, &Window::mouseDoubleClick_slot);

  timePlot->axisRect()->setupFullAxesBox(true);
  timePlot->setInteraction(QCP::iRangeZoom, true);
  timePlot->setInteraction(QCP::iRangeDrag, true);
  xyPlot->axisRect()->setupFullAxesBox(true);
  xyPlot->setInteraction(QCP::iRangeZoom, true);
  xyPlot->setInteraction(QCP::iRangeDrag, true);

  lowerLine->point1->setCoords(60, 0);
  lowerLine->point2->setCoords(60, 100);
  lowerLine->setVisible(false);
  lowerLine->setSelectable(false);
  lowerLine->setPen(QPen(QColor(Qt::darkRed)));

  upperLine->point1->setCoords(260, 0);
  upperLine->point2->setCoords(260, 100);
  upperLine->setVisible(false);
  upperLine->setSelectable(false);
  upperLine->setPen(QPen(QColor(Qt::darkRed)));

  connect(timePlot, &QCustomPlot::mousePress, this, &Window::mousePress_slot);
  connect(timePlot, &QCustomPlot::mouseRelease, this, &Window::mouseRelease_slot);

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
  QMenu * file = new QMenu;
  file = menuBar()->addMenu(tr("&File"));

  QAction * open_action = new QAction(tr("&Open"));
  connect(open_action, &QAction::triggered, this, &Window::open_action_slot);
  file->addAction(open_action);

  QAction * save_action = new QAction(tr("&Save"));
  connect(save_action, &QAction::triggered, this, &Window::save_action_slot);
  file->addAction(save_action);

  QAction * save_as_action = new QAction(tr("&Save As"));
  connect(save_as_action, &QAction::triggered, this, &Window::save_as_action_slot);
  file->addAction(save_as_action);

  QAction * import_action = new QAction(tr("&Import"));
  connect(import_action, &QAction::triggered, this, &Window::import_action_slot);
  file->addAction(import_action);

  QAction * export_action = new QAction(tr("&Export"));
  connect(export_action, &QAction::triggered, this, &Window::export_action_slot);
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

  QAction * math_action = new QAction(tr("&Math Channel"));
  connect(math_action, &QAction::triggered, this, &Window::math_action_slot);
  tools->addAction(math_action);

  QAction * range_cursor_action = new QAction(tr("&Cursor"));
  connect(range_cursor_action, &QAction::triggered, this, &Window::range_cursor_action_slot);
  tools->addAction(range_cursor_action);
}


void Window::open_action_slot()
{
  QString filename = QFileDialog::getOpenFileName(this, tr("&Open File"),"/home/avs-es/Documents/C++/live-plotter-4000/data", tr("*.analyzer"));
  if(filename != "")
    std::cout << "Not supported yet! Use Import.";
}


void Window::save_action_slot()
{
}


void Window::save_as_action_slot()
{
}


void Window::import_action_slot()
{
  std::string filename = QFileDialog::getOpenFileName(this, tr("Open File"),"/home/avs-es/Documents/C++/live-plotter-4000/data", tr("*.txt *csv")).toStdString();
  if(filename != "")
    {
      AskForHeader dialog;
      int headers = dialog.get_value();

      load_file(filename,headers);
    }
  timePlot->replot();
}


void Window::load_file(std::string filename, int headers)
{
  std::ifstream file(filename);
  std::string line;

  getline(file,line);
  std::stringstream ss1(line);
  std::string number1;
  int columnCount = 0;
  while(getline(ss1,number1,'\t'))
    columnCount++;

  file.clear();
  file.seekg(0);



  int rowCount = 0;
  while(getline(file,line))
    rowCount++;

  for(int j=0;j<columnCount;j++)
    timePlot->addGraph();

  file.clear();
  file.seekg(0);

  int counter = 0;
  for(int i=0;i<headers;i++)
    getline(file, line);

  while(getline(file, line) && counter < 1000000000)
    {
      std::stringstream ss(line);
      std::string number;
      int j = 0;
      while(getline(ss,number,'\t'))
        {
          timePlot->graph(j)->addData((double)counter,std::stod(number));
          timePlot->graph(j)->setPen(QPen(QColor((Qt::GlobalColor)(7+j))));
          timePlot->graph(j)->setSelectable(QCP::stWhole);
          j++;
        }
      counter++;
    }
}


void Window::export_action_slot()
{
}


void Window::math_action_slot()
{
}


void Window::range_cursor_action_slot()
{
  if(lowerLine->realVisibility())
    {
      lowerLine->setVisible(false);
      upperLine->setVisible(false);
    }
  else
    {
      lowerLine->setVisible(true);
      upperLine->setVisible(true);
    }
  timePlot->replot();
}


void Window::mousePress_slot(QMouseEvent * event)
{
  if((event->pos().x() > lowerLine->point1->pixelPosition().x()-3) &&
     (event->pos().x() < lowerLine->point1->pixelPosition().x()+3) &&
     lowerLine->realVisibility())
    {
      connect(timePlot, &QCustomPlot::mouseMove, this, &Window::change_lowerLine_position_slot);
      timePlot->setInteraction(QCP::iRangeDrag, false);
    }
  else if((event->pos().x() > upperLine->point1->pixelPosition().x()-3) &&
          (event->pos().x() < upperLine->point1->pixelPosition().x()+3) &&
          upperLine->realVisibility())
    {
      connect(timePlot, &QCustomPlot::mouseMove, this, &Window::change_upperLine_position_slot);
      timePlot->setInteraction(QCP::iRangeDrag, false);
    }
}


void Window::change_lowerLine_position_slot(QMouseEvent * event)
{
  if(event->pos().x() >= upperLine->point1->pixelPosition().x())
    {
      lowerLine->point1->setCoords(timePlot->xAxis->pixelToCoord(upperLine->point1->pixelPosition().x()), 0);
      lowerLine->point2->setCoords(timePlot->xAxis->pixelToCoord(upperLine->point1->pixelPosition().x()),100);
    }
  else
    {
      lowerLine->point1->setCoords(timePlot->xAxis->pixelToCoord(event->pos().x()), 0);
      lowerLine->point2->setCoords(timePlot->xAxis->pixelToCoord(event->pos().x()),100);
    }
  timePlot->replot();
}


void Window::change_upperLine_position_slot(QMouseEvent * event)
{
  if(event->pos().x() <= lowerLine->point1->pixelPosition().x())
    {
      upperLine->point1->setCoords(timePlot->xAxis->pixelToCoord(lowerLine->point1->pixelPosition().x()), 0);
      upperLine->point2->setCoords(timePlot->xAxis->pixelToCoord(lowerLine->point1->pixelPosition().x()),100);
    }
  else
    {
      upperLine->point1->setCoords(timePlot->xAxis->pixelToCoord(event->pos().x()), 0);
      upperLine->point2->setCoords(timePlot->xAxis->pixelToCoord(event->pos().x()),100);
    }
  timePlot->replot();
}


void Window::mouseRelease_slot()
{
  disconnect(timePlot, &QCustomPlot::mouseMove, this, &Window::change_lowerLine_position_slot);
  disconnect(timePlot, &QCustomPlot::mouseMove, this, &Window::change_upperLine_position_slot);
  timePlot->setInteraction(QCP::iRangeDrag, true);
}


void Window::plottableClick_slot(QCPAbstractPlottable * plottable,
                                 int                    dataIndex,
                                 QMouseEvent *          event)
{
  switch(event->button())
    {
    case Qt::RightButton :
      if(plottable->selected())
        {
          plotcontextmenu->set_plottable(plottable);
          plotcontextmenu->move(event->globalPos());
          plotcontextmenu->show();
        }
      break;
    default:
      break;
    }
}


void Window::mouseDoubleClick_slot(QMouseEvent * event)
{
  if(event->button() == Qt::RightButton)
    for(int i=0; i<timePlot->graphCount();i++)
      timePlot->graph(i)->setVisible(true);
  timePlot->replot();
}


void Window::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  menu.exec(event->globalPos());
}


AskForHeader::AskForHeader()
{
  QLabel *        label = new QLabel(this);
  label->setText("How many headers?");
  QLineEdit *     enter = new QLineEdit(this);
  connect(enter, &QLineEdit::editingFinished, [this,enter](){headers_value = enter->text().toInt();done(1);});
  enter->setText("1");
  QVBoxLayout *   layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(enter);
  setLayout(layout);
  exec();
}


int AskForHeader::get_value()
{
  return headers_value;
}


PlotContextMenu::PlotContextMenu(QCustomPlot * p, Window * w)
  : parent(p)
  , window_parent(w)
  , xyz(&(w->xyz))
{
  QAction * copy_action = new QAction(tr("&Copy"));
  addAction(copy_action);
  connect(copy_action, &QAction::triggered, this, &PlotContextMenu::copy_action_slot);

  QAction * truncate_action = new QAction(tr("&Truncate"));
  addAction(truncate_action);
  connect(truncate_action, &QAction::triggered, this, &PlotContextMenu::truncate_action_slot);

  QAction * truncate_all_action = new QAction(tr("&Truncate All"));
  addAction(truncate_all_action);
  connect(truncate_all_action, &QAction::triggered, this, &PlotContextMenu::truncate_all_action_slot);

  QAction * truncate_at_cursor_action = new QAction(tr("&Truncate At Cursor"));
  addAction(truncate_at_cursor_action);
  connect(truncate_at_cursor_action, &QAction::triggered, this, &PlotContextMenu::truncate_at_cursor_action_slot);

  QAction * truncate_all_at_cursor_action = new QAction(tr("&Truncate All At Cursor"));
  addAction(truncate_all_at_cursor_action);
  connect(truncate_all_at_cursor_action, &QAction::triggered, this, &PlotContextMenu::truncate_all_at_cursor_action_slot);

  QAction * filter_action = new QAction(tr("&Filter"));
  addAction(filter_action);
  connect(filter_action, &QAction::triggered, this, &PlotContextMenu::filter_action_slot);

  QAction * hide_action = new QAction(tr("&Hide"));
  addAction(hide_action);
  connect(hide_action, &QAction::triggered, [this](){plottable->setVisible(false);plottable->parentPlot()->replot();});

  QAction * remove_action = new QAction(tr("&Remove"));
  addAction(remove_action);
  connect(remove_action, &QAction::triggered, [this](){plottable->parentPlot()->removePlottable(plottable);plottable->parentPlot()->replot();});

  QAction * set_as_x_action = new QAction(tr("&Set As X"));
  addAction(set_as_x_action);
  connect(set_as_x_action, &QAction::triggered, this, &PlotContextMenu::set_as_x_action_slot);

  QAction * set_as_y_action = new QAction(tr("&Set As Y"));
  addAction(set_as_y_action);
  connect(set_as_y_action, &QAction::triggered, this, &PlotContextMenu::set_as_y_action_slot);

  QAction * set_as_z_action = new QAction(tr("&Set As Z"));
  addAction(set_as_z_action);
  connect(set_as_z_action, &QAction::triggered, this, &PlotContextMenu::set_as_z_action_slot);
}


void PlotContextMenu::set_plottable(QCPAbstractPlottable * p)
{
  plottable = p;
}


void PlotContextMenu::copy_action_slot()
{
}


void PlotContextMenu::truncate_action_slot()
{
  QCPSelectionRect * truncate_selection_rect = new QCPSelectionRect(parent);
  connect(truncate_selection_rect, &QCPSelectionRect::accepted, this, &PlotContextMenu::truncate_selection_accepted_slot);
  parent->setSelectionRect(truncate_selection_rect);
  parent->setSelectionRectMode(QCP::srmSelect);
  for(int i=0;i<plottable->parentPlot()->graphCount();i++)
    {
      if(plottable->parentPlot()->graph(i) == plottable)
        plottable->setSelectable(QCP::stDataRange);
      else
        plottable->parentPlot()->graph(i)->setSelectable(QCP::stNone);
    }
}


void PlotContextMenu::truncate_all_action_slot()
{
  QCPSelectionRect * truncate_all_selection_rect = new QCPSelectionRect(parent);
  connect(truncate_all_selection_rect, &QCPSelectionRect::accepted, this, &PlotContextMenu::truncate_all_selection_accepted_slot);
  parent->setSelectionRect(truncate_all_selection_rect);
  parent->setSelectionRectMode(QCP::srmSelect);
  for(int i=0;i<plottable->parentPlot()->graphCount();i++)
    plottable->parentPlot()->graph(i)->setSelectable(QCP::stDataRange);
}


void PlotContextMenu::truncate_selection_accepted_slot()
{
  int b = (int)plottable->parentPlot()->selectionRect()->range(plottable->parentPlot()->xAxis).lower;
  int e = (int)plottable->parentPlot()->selectionRect()->range(plottable->parentPlot()->xAxis).upper;

  ((QCPGraph *)plottable)->data()->removeBefore(b);
  ((QCPGraph *)plottable)->data()->removeAfter(e);

  plottable->parentPlot()->setSelectionRectMode(QCP::srmNone);

  for(int i=0;i<plottable->parentPlot()->graphCount();i++)
    plottable->parentPlot()->graph(i)->setSelectable(QCP::stWhole);

  plottable->parentPlot()->replot();
}


void PlotContextMenu::truncate_all_selection_accepted_slot()
{
  int b = (int)plottable->parentPlot()->selectionRect()->range(plottable->parentPlot()->xAxis).lower;
  int e = (int)plottable->parentPlot()->selectionRect()->range(plottable->parentPlot()->xAxis).upper;

  for(int i=0; i<plottable->parentPlot()->graphCount();i++)
    {
      plottable->parentPlot()->graph(i)->data()->removeBefore(b);
      plottable->parentPlot()->graph(i)->data()->removeAfter(e);
    }

  plottable->parentPlot()->setSelectionRectMode(QCP::srmNone);

  for(int i=0;i<plottable->parentPlot()->graphCount();i++)
    plottable->parentPlot()->graph(i)->setSelectable(QCP::stWhole);

  plottable->parentPlot()->replot();
}


void PlotContextMenu::truncate_at_cursor_action_slot()
{
  int b = window_parent->lowerLine->point1->key();
  int e = window_parent->upperLine->point1->key();

  ((QCPGraph *)plottable)->data()->removeBefore(b);
  ((QCPGraph *)plottable)->data()->removeAfter(e);

  parent->replot();
}


void PlotContextMenu::truncate_all_at_cursor_action_slot()
{
  int b = window_parent->lowerLine->point1->key();
  int e = window_parent->upperLine->point1->key();

  for(int i=0; i<parent->graphCount();i++)
    {
      parent->graph(i)->data()->removeBefore(b);
      parent->graph(i)->data()->removeAfter(e);
    }

  parent->replot();
}


void PlotContextMenu::filter_action_slot()
{
}


void PlotContextMenu::set_as_x_action_slot()
{
  window_parent->xyz.X = ((QCPGraph *)plottable)->data();
}


void PlotContextMenu::set_as_y_action_slot()
{
  window_parent->xyz.Y = ((QCPGraph *)plottable)->data();
}


void PlotContextMenu::set_as_z_action_slot()
{
  bool foundXRange, foundYRange;
  QCPRange xRange = xyz->X->valueRange(foundXRange);
  QCPRange yRange = xyz->Y->valueRange(foundYRange);

  if(foundXRange && foundYRange)
    window_parent->colorMap->data()->setRange(xRange, yRange);
  else
    std::cout << "PlotContextMenu::set_as_z_action_slot(): No Range was found for xy-plot.\n";

  window_parent->colorMap->data()->fill(0.0);
  xyz->Z = ((QCPGraph *)plottable)->data();

  QCPDataContainer<QCPGraphData>::const_iterator startX = xyz->X->findBegin(window_parent->lowerLine->point1->key());
  QCPDataContainer<QCPGraphData>::const_iterator endX   = xyz->X->findEnd(window_parent->upperLine->point1->key());
  QCPDataContainer<QCPGraphData>::const_iterator startY = xyz->Y->findBegin(window_parent->lowerLine->point1->key());
  QCPDataContainer<QCPGraphData>::const_iterator startZ = xyz->Z->findBegin(window_parent->lowerLine->point1->key());

  int xInd, yInd;

  for(auto itr = startX; itr!=endX; itr++)
    {
      double x = itr->value;
      double y = startY->value;
      double z = startZ->value;

      window_parent->colorMap->data()->coordToCell(x,y,&xInd,&yInd);
      window_parent->colorMap->data()->setCell(xInd,yInd,z);

      startY++;
      startZ++;
    }
    window_parent->xyPlot->replot();
}
