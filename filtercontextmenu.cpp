#include "window.hpp"
#include <vector>
#include <iostream>


FilterContextMenu::FilterContextMenu(QCustomPlot * p, PlotContextMenu * w)
  : window_parent(w)
  , parent(p)
{
  QAction * moving_average_action = new QAction(tr("&Moving average"));
  addAction(moving_average_action);
  connect(moving_average_action, &QAction::triggered, this, &FilterContextMenu::moving_average_action_slot);

  QAction * biquad_action = new QAction(tr("&Biquad"));
  addAction(biquad_action);
  connect(biquad_action, &QAction::triggered, this, &FilterContextMenu::biquad_action_slot);

  QAction * fir_action = new QAction(tr("&FIR"));
  addAction(fir_action);
  connect(fir_action, &QAction::triggered, this, &FilterContextMenu::fir_action_slot);

  QAction * iir_action = new QAction(tr("&IIR"));
  addAction(iir_action);
  connect(iir_action, &QAction::triggered, this, &FilterContextMenu::iir_action_slot);

  QAction * convolution_action = new QAction(tr("&Convolution"));
  addAction(convolution_action);
  connect(convolution_action, &QAction::triggered, this, &FilterContextMenu::convolution_action_slot);

  QAction * dft_action = new QAction(tr("&DFT"));
  addAction(dft_action);
  connect(dft_action, &QAction::triggered, this, &FilterContextMenu::dft_action_slot);
}



void FilterContextMenu::set_plottable(QCPAbstractPlottable * p)
{
  plottable = p;
}


void FilterContextMenu::moving_average_action_slot()
{
  AskForMovingAverageRange dialog;
  int range = dialog.get_value();

  int _size_ = (((QCPGraph *)plottable)->data())->size();

  std::vector<double> result;
  result.reserve(_size_);

  QVector<double> key_vec;
  key_vec.reserve(_size_);

  std::vector<double> data;
  data.reserve(_size_);

  auto begin = (((QCPGraph *)plottable)->data())->constBegin();
  auto end   = (((QCPGraph *)plottable)->data())->constEnd();

  for(auto itr = begin; itr != end; itr++)
    {
      data.push_back(itr->value);
      key_vec.push_back(itr->key);
    }

  int backwards = range/2;
  int forwards  = range%2 == 0 ? range/2 - 1 : range/2;

  for(int i = 0; i< _size_; i++)
    {
      int lower = i - backwards < 0 ? 0 : i - backwards;
      int upper = i + forwards  >= _size_ ? _size_ : i + forwards;

      double count = 0;

      for(int j = lower; j<=upper; j++)
        {
          count += data[j];
        }
      result.push_back(count/((double)range));
    }

  ((QCPGraph *)plottable)->setData(key_vec, QVector<double>::fromStdVector(result));
  parent->replot();
}


void FilterContextMenu::biquad_action_slot()
{
  int entry;
  QDialog dialog;
  QDialog * p_dialog = &dialog;
  QLabel * label = new QLabel(this);
  label->setText("Select Filter:\n(1) \n(2)");
  QLineEdit * enter = new QLineEdit(this);
  connect(enter, &QLineEdit::editingFinished, [p_dialog, entry, enter]() mutable {entry = enter->text().toInt();p_dialog->done(1);});
  QVBoxLayout * layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(enter);
  dialog.setLayout(layout);
  dialog.exec();

  switch(entry)
    {
    case 1: std::cout << entry << std::endl;
    case 2: std::cout << entry << std::endl;
    default: std::cout << "default, " << entry << std::endl;
    }
}


void FilterContextMenu::fir_action_slot()
{
}


void FilterContextMenu::iir_action_slot()
{
}


void FilterContextMenu::convolution_action_slot()
{
}


void FilterContextMenu::dft_action_slot()
{
}
