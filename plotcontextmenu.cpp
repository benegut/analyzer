#include "window.hpp"
#include <iostream>

PlotContextMenu::PlotContextMenu(QCustomPlot * p, Window * w)
  : parent(p)
  , window_parent(w)
  , xyz(&(w->xyz))
  , filtercontextmenu(new FilterContextMenu(p, this))
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
  connect(filter_action, &QAction::triggered, filtercontextmenu, &FilterContextMenu::show);

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


void PlotContextMenu::filter_action_slot()
{
  //filtercontextmenu->move(event->globalPos());
  filtercontextmenu->show();
  filtercontextmenu->set_plottable(plottable);
}
