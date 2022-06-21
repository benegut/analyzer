#include "window.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdio>

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
        {
          timePlot->graph(j)->addData((double)timePlot->graph(j++)->dataCount(),std::stod(number));
          //j++;
        }
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
        std::string label;
        switch(number)
          {
          case "1" : label = "X"; break;
          case "2" : label = "Y"; break;
          case "3" : label = "Z0"; break;
          case "4" : label = "Z1"; break;
          case "5" : label = "Z2"; break;
          case "6" : label = "Z3"; break;
          case "7" : label = "Z4"; break;
          case "8" : label = "Z5"; break;
          case "9" : label = "Z6"; break;
          case "10" : label = "Z7"; break;
          case "11" : label = "Z8"; break;
          case "12" : label = "Z9"; break;
          }
        graph_label.insert(i, label);
        i++;
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


AskForPolandFile::AskForPolandFile()
{
  QLabel *        label = new QLabel(this);
  label->setText("File from last Poland trip?");
  QPushButton * YesButton = new QPushButton("Yes", this);
  connect(YesButton, &QPushButton::clicked, [this](){result = true;done(1);});
  QPushButton * NoButton = new QPushButton("No", this);
  connect(NoButton, &QPushButton::clicked, [this](){result = false;done(1);});
  QVBoxLayout * layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(YesButton);
  layout->addWidget(NoButton);
  setLayout(layout);
  exec();
}


bool AskForPolandFile::get_value()
{
  return result;
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


void MathWindow::eval_slot()
{
  if(!map->contains(entryField->text()))
    map->insert(entryField->text(), new Equation(entryField->text(), this));
}



Equation::Equation(QString e_str, MathWindow * parent)
  : parent(parent)
  , equation_str(e_str)
  , layout(new QGridLayout)
  , box(new QGroupBox(equation_str, this))
  , symbol_table(new exprtk::symbol_table<double>)
  , expression(new exprtk::expression<double>)
  , parser(new exprtk::parser<double>)
  , params(std::vector<double>(100))
{
  int i = 0;
  QVector<QString> tracker;
  for(auto c: equation_str)
    {
      QString str(c);

      if(str.contains(QRegExp("[a-m]")) && !tracker.contains(str))
        {
          tracker.push_back(str);

          QSlider * slider = new QSlider(Qt::Vertical, this);
          slider->setMaximum(100);
          slider->setMinimum(-100);
          slider->setSliderPosition(2);
          layout->addWidget(slider, 1, layout->columnCount());
          slider->show();
          connect(slider, &QSlider::valueChanged, [this, slider, e_str, i, c](){this->params[i] = (double)slider->value();});

          QLabel *  label  = new QLabel(str, this);
          layout->addWidget(label, 0, layout->columnCount());

          if(!symbol_table->add_variable(str.toStdString(), params[i]))
            std::cout << "Error in symbol table\n";
          i++;
        }
    }

  QRegExp rx("[x-z][0-7]");
  int pos = 0;
  QStringList list;
  while(((pos = rx.indexIn(equation_str, pos)) != -1) && !tracker.contains(rx.cap(0)))
    {
      tracker.push_back(rx.cap(0));
      std::string str = rx.cap(0).toStdString().c_str();
      // if(str=="x0"){symbol_table->add_variable(str, parent->parent->data_vec[X-1]);}
      // if(str=="y0"){symbol_table->add_variable(str, parent->parent->data_vec[Y-1]);}
      // if(str=="z0"){symbol_table->add_variable(str, parent->parent->data_vec[Z0-1]);}
      // if(str=="z1"){symbol_table->add_variable(str, parent->parent->data_vec[Z1-1]);}
      // if(str=="z2"){symbol_table->add_variable(str, parent->parent->data_vec[Z2-1]);}
      // if(str=="z3"){symbol_table->add_variable(str, parent->parent->data_vec[Z3-1]);}
      // if(str=="z4"){symbol_table->add_variable(str, parent->parent->data_vec[Z4-1]);}
      // if(str=="z5"){symbol_table->add_variable(str, parent->parent->data_vec[Z5-1]);}
      // if(str=="z6"){symbol_table->add_variable(str, parent->parent->data_vec[Z6-1]);}
      // if(str=="z7"){symbol_table->add_variable(str, parent->parent->data_vec[Z7-1]);}
      // if(str=="z8"){symbol_table->add_variable(str, parent->parent->data_vec[Z8-1]);}
      // if(str=="z9"){symbol_table->add_variable(str, parent->parent->data_vec[Z9-1]);}
      pos += rx.matchedLength();
    }

  expression->register_symbol_table(*symbol_table);
  if(!parser->compile(equation_str.toStdString(), *expression))
    printf("Error: %s\n", parser->error().c_str());

  parent->parent->timePlot->addGraph();
  // parent->parent->expression_vec.insert(expression, parent->parent->timePlot->graphCount()-1);
  // parent->parent->ColorMapDataChooser_Obj->expression_vec.insert(expression, equation_str);
  // parent->parent->ColorMapDataChooser_Obj->update_buttons();

  box->setLayout(layout);
  parent->layout->addWidget(box);
}
