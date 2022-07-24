#include "window.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <kfr/all.hpp>


Window::Window()
  : timePlot(new QCustomPlot)
  , xyPlot(new QCustomPlot)
  , colorMap(new QCPColorMap(xyPlot->xAxis, xyPlot->yAxis))
  , plotcontextmenu(new PlotContextMenu(timePlot, this))
  , mathwindow(new MathWindow(this))
  , videowindow(new VideoWindow(this)) 
  , videorunner(new VideoRunner(this))
  , lowerLine(new QCPItemStraightLine(timePlot))
  , upperLine(new QCPItemStraightLine(timePlot))
  , toolbar(new QToolBar(this))
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

  colorMap->setDataRange(QCPRange(-1.0, 1.0));
  colorMap->data()->setSize(200, 200);
  colorMap->data()->fill(0.0);
  //colorMap->setGradient(QCPColorGradient::gpGrayscale);
  actions();
  timePlot->legend->setVisible(true);
  this->resize(1700, 800);
  this->show();
  timePlot->replot();
  xyPlot->replot();

  connect(videorunner, &VideoRunner::data, this, &Window::data);
  connect(videorunner, &VideoRunner::replot_signal, this, &Window::replot_slot);
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

  QAction * recalculate_math_graphs_action = new QAction(tr("&Recalculate"));
  connect(recalculate_math_graphs_action, &QAction::triggered, this, &Window::recalculate_math_graphs_action_slot);
  tools->addAction(recalculate_math_graphs_action);

  QAction * video_widget_action = new QAction(tr("&Video"));
  connect(video_widget_action, &QAction::triggered, videowindow, &VideoWindow::show_slot);
  tools->addAction(video_widget_action);

  addToolBar(toolbar);

  QLabel * sizebox_label = new QLabel(QString("Resolution:"), this);
  toolbar->addWidget(sizebox_label);
  sizebox = new QSpinBox(this);
  sizebox->setMaximum(400);
  sizebox->setMinimum(50);
  sizebox->setSingleStep(10);
  sizebox->setValue(200);
  QAction * sizebox_action = toolbar->addWidget(sizebox);
  connect(sizebox, qOverload<int>(&QSpinBox::valueChanged), this, &Window::sizebox_slot);

  QLabel * greyscale_offset_label = new QLabel(QString("BW-Offset:"), this);
  toolbar->addWidget(greyscale_offset_label);
  greyscale_offset_box = new QDoubleSpinBox(this);
  greyscale_offset_box->setMaximum(100000);
  greyscale_offset_box->setMinimum(-100000);
  greyscale_offset_box->setSingleStep(1);
  greyscale_offset_box->setValue(0);
  QAction * greyscale_offset_box_action = toolbar->addWidget(greyscale_offset_box);
  connect(greyscale_offset_box, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &Window::set_greyscale);

  QLabel * greyscale_range_label = new QLabel(QString("BW-Range:"), this);
  toolbar->addWidget(greyscale_range_label);
  greyscale_amplitude_box = new QDoubleSpinBox(this);
  greyscale_amplitude_box->setMaximum(100000);
  greyscale_amplitude_box->setMinimum(0);
  greyscale_amplitude_box->setSingleStep(1);
  greyscale_amplitude_box->setValue(1);
  QAction * greyscale_amplitude_box_action = toolbar->addWidget(greyscale_amplitude_box);
  connect(greyscale_amplitude_box, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &Window::set_greyscale);

  QAction * greyscale_rect_action = new QAction(tr("&BW-Rect"));
  connect(greyscale_rect_action, &QAction::triggered, this, &Window::greyscale_rect_action_slot);
  tools->addAction(greyscale_rect_action);

  connect(timePlot->selectionRect(), &QCPSelectionRect::started, this, &Window::set_greyscale_from_selection_rect_start);
  connect(timePlot->selectionRect(), &QCPSelectionRect::accepted, this, &Window::set_greyscale_from_selection_rect_accepted);

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

      AskForPolandFile dialog2;
      if(dialog2.get_value())
        get_names_from_header_POLAND(filename);
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

  int j = 0;
  while(getline(ss1,number1,'\t'))
    {
      timePlot->addGraph();
      timePlot->graph()->setPen(QPen(QColor((Qt::GlobalColor)(7+j))));
      timePlot->graph()->setSelectable(QCP::stWhole);
      j++;
    }

  file.clear();
  file.seekg(0);

  for(int i=0;i<headers;i++)
    getline(file, line);

  while(getline(file, line))
    {
      std::stringstream ss(line);
      std::string number;
      int j = 0;
      while(getline(ss,number,'\t'))
        timePlot->graph(j)->addData((double)timePlot->graph(j++)->dataCount(),std::stod(number));
    }
  file.close();
}


void Window::get_names_from_header_POLAND(std::string filename)
{
  std::ifstream file(filename);
  std::string line;

  getline(file, line);

  std::stringstream ss(line);
  std::string number;

  int i = 0;
  while(getline(ss, number, '\t'))
      {
        switch(std::stoi(number))
          {
          case 0 : timePlot->graph(i)->setName("OFF"); break;
          case 1 : timePlot->graph(i)->setName("X"); break;
          case 2 : timePlot->graph(i)->setName("Y"); break;
          case 3 : timePlot->graph(i)->setName("Z0"); break;
          case 4 : timePlot->graph(i)->setName("Z1"); break;
          case 5 : timePlot->graph(i)->setName("Z2"); break;
          case 6 : timePlot->graph(i)->setName("Z3"); break;
          case 7 : timePlot->graph(i)->setName("Z4"); break;
          case 8 : timePlot->graph(i)->setName("Z5"); break;
          case 9 : timePlot->graph(i)->setName("Z6"); break;
          case 10 : timePlot->graph(i)->setName("Z7"); break;
          case 11 : timePlot->graph(i)->setName("Z8"); break;
          case 12 : timePlot->graph(i)->setName("Z9"); break;
          }
        i++;
      }
}


void Window::export_action_slot()
{
}


void Window::math_action_slot()
{
  mathwindow->show();
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


void Window::data(double x, double y, double z, double k)
{
  int xInd, yInd;

  colorMap->data()->coordToCell(x,y,&xInd,&yInd);
  colorMap->data()->setCell(xInd,yInd,z);

  videowindow->video_line->point1->setCoords(k, 0);
  videowindow->video_line->point2->setCoords(k,100);
}


void Window::replot_slot()
{
  timePlot->replot();
  xyPlot->replot();
}


void Window::set_greyscale()
{
  double amplitude = greyscale_amplitude_box->value();
  double offset    = greyscale_offset_box->value();
  colorMap->setDataRange(QCPRange(offset-amplitude, offset+amplitude));
  xyPlot->replot();
}


void Window::greyscale_rect_action_slot()
{
  timePlot->setSelectionRectMode(QCP::srmCustom);
}


void Window::set_greyscale_from_selection_rect_start(QMouseEvent * event)
{
  greyscale_raw_value_1 = timePlot->yAxis->pixelToCoord(event->y());
}


void Window::set_greyscale_from_selection_rect_accepted(QRect rect, QMouseEvent * event)
{
  greyscale_raw_value_2 = timePlot->yAxis->pixelToCoord(event->y());

  double amplitude = greyscale_raw_value_1 > greyscale_raw_value_2 ? (greyscale_raw_value_1 - greyscale_raw_value_2)/2.0 : (greyscale_raw_value_2 - greyscale_raw_value_1)/2.0;
  double offset = greyscale_raw_value_1 > greyscale_raw_value_2 ? greyscale_raw_value_2+amplitude : greyscale_raw_value_1+amplitude;
  greyscale_amplitude_box->setValue(amplitude);
  greyscale_offset_box->setValue(offset);
  colorMap->setDataRange(QCPRange(offset-amplitude, offset+amplitude));
  colorMap->data()->fill(offset);
  xyPlot->replot();
  timePlot->replot();
  timePlot->setSelectionRectMode(QCP::srmNone);
}



void Window::recalculate_math_graphs_action_slot()
{
  bool foundRange;
  double first_key = timePlot->graph(0)->getKeyRange(foundRange).lower;
  double last_key  = timePlot->graph(0)->getKeyRange(foundRange).upper;

  for(int i=1;i<timePlot->graphCount();i++)
    {
      first_key = timePlot->graph(i)->getKeyRange(foundRange).lower > first_key ? timePlot->graph(i)->getKeyRange(foundRange).lower : first_key;
      last_key  = timePlot->graph(i)->getKeyRange(foundRange).upper > last_key  ? timePlot->graph(i)->getKeyRange(foundRange).upper : last_key;
    }

  for(auto e : expression_holder.keys())
    for(int i = 0; i<timePlot->graphCount(); i++)
      if(timePlot->graph(i)->name() == e)
        timePlot->graph(i)->data()->clear();

  for(int k = first_key; k < last_key; k++)
    {
      for(int i = 0; i<timePlot->graphCount(); i++)
        data_holder[timePlot->graph(i)->name()] = timePlot->graph(i)->data()->findBegin(k)->value;

      for(auto e : expression_holder.keys())
        for(int i = 0; i<timePlot->graphCount(); i++)
          if(timePlot->graph(i)->name() == e)
            timePlot->graph(i)->addData(k,expression_holder[e]->value());
    }

  timePlot->replot();
}


void Window::sizebox_slot(int size)
{
  colorMap->data()->setSize(size, size);
  xyPlot->replot();
}
