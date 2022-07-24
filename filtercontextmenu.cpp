#include "window.hpp"
#include <vector>
#include <iostream>
#include <cstdio>
#include <kfr/all.hpp>



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
  int _size_ = (((QCPGraph *)plottable)->data())->size();

  std::vector<double> result;
  result.reserve(_size_);

  QVector<double> key_vec;
  key_vec.reserve(_size_);

  kfr::univector<double> data;
  data.reserve(_size_);

  kfr::univector<double, 7> taps;

  kfr::expression_pointer<double> window_function_pointer;

  AskForWindowFunction window_function_dialog;
  int window_int = window_function_dialog.get_value();

  switch(window_int)
    {
    case 1: window_function_pointer   = kfr::to_pointer(kfr::window_bartlett(taps.size())); break;
    case 2: window_function_pointer  = kfr::to_pointer(kfr::window_bartlett_hann(taps.size())); break;
    case 3: window_function_pointer  = kfr::to_pointer(kfr::window_blackman(taps.size())); break;
    case 4: window_function_pointer  = kfr::to_pointer(kfr::window_blackman_harris(taps.size())); break;
    case 5: window_function_pointer  = kfr::to_pointer(kfr::window_bohman(taps.size())); break;
    case 6: window_function_pointer  = kfr::to_pointer(kfr::window_cosine(taps.size())); break;
    case 7: window_function_pointer  = kfr::to_pointer(kfr::window_flattop(taps.size())); break;
    case 8: window_function_pointer  = kfr::to_pointer(kfr::window_gaussian(taps.size())); break;
    case 9: window_function_pointer  = kfr::to_pointer(kfr::window_hamming(taps.size())); break;
    case 10: window_function_pointer = kfr::to_pointer(kfr::window_hann(taps.size())); break;
    case 11: window_function_pointer = kfr::to_pointer(kfr::window_kaiser(taps.size())); break;
    case 12: window_function_pointer = kfr::to_pointer(kfr::window_lanczos(taps.size())); break;
    case 13: window_function_pointer = kfr::to_pointer(kfr::window_rectangular(taps.size())); break;
    case 14: window_function_pointer = kfr::to_pointer(kfr::window_triangular(taps.size())); break;
    default: printf("Not a valid number.\n");
    }

  AskForFirFilter fir_filter_dialog;
  int fir_filter_int = fir_filter_dialog.get_value();

  kfr::filter_fir<double> filter(taps);

  switch(fir_filter_int)
    {
    case 1: filter = fir_lowpass_setup(window_function_pointer); break;
    case 2: filter = fir_highpass_setup(window_function_pointer); break;
    case 3: filter = fir_bandpass_setup(window_function_pointer); break;
    case 4: filter = fir_bandstop_setup(window_function_pointer); break;
    default: printf("Not a valid number.\n");
    }

  auto begin = (((QCPGraph *)plottable)->data())->constBegin();
  auto end   = (((QCPGraph *)plottable)->data())->constEnd();

  for(auto itr = begin; itr != end; itr++)
    {
      data.push_back(itr->value);
      key_vec.push_back(itr->key);
    }

  filter.apply(data);

  std::vector<double> vec;
  vec.reserve(_size_);

  for(auto i : data)
    vec.push_back(i);

  ((QCPGraph *)plottable)->setData(key_vec, QVector<double>::fromStdVector(vec));
  parent->replot();
}


kfr::filter_fir<double> FilterContextMenu::fir_lowpass_setup(kfr::expression_pointer<double> w)
{
  kfr::univector<double, 7> taps;

  AskForFrequency dialog;
  double frequency = dialog.get_value();

  fir_lowpass(taps, frequency, w, true);

  kfr::filter_fir<double> filter(taps);
  return filter;
}


kfr::filter_fir<double>FilterContextMenu::fir_highpass_setup(kfr::expression_pointer<double> w)
{
  kfr::univector<double, 7> taps;

  AskForFrequency dialog;
  double frequency = dialog.get_value();

  fir_highpass(taps, 0.1, w, true);

  kfr::filter_fir<double> filter(taps);
  return filter;
}


kfr::filter_fir<double> FilterContextMenu::fir_bandpass_setup(kfr::expression_pointer<double> w)
{
  kfr::univector<double, 7> taps;

  AskForLowerFrequency dialog_lower;
  double lower_freq = dialog_lower.get_value();

  AskForUpperFrequency dialog_upper;
  double upper_freq = dialog_upper.get_value();

  fir_bandpass(taps, lower_freq, upper_freq, w, true);

  kfr::filter_fir<double> filter(taps);
  return filter;
}


kfr::filter_fir<double> FilterContextMenu::fir_bandstop_setup(kfr::expression_pointer<double> w)
{
  kfr::univector<double, 7> taps;

  AskForLowerFrequency dialog_lower;
  double lower_freq = dialog_lower.get_value();

  AskForUpperFrequency dialog_upper;
  double upper_freq = dialog_upper.get_value();

  fir_bandstop(taps, lower_freq, upper_freq, w, true);

  kfr::filter_fir<double> filter(taps);
  return filter;
}




void FilterContextMenu::iir_action_slot()
{
  int _size_ = (((QCPGraph *)plottable)->data())->size();

  std::vector<double> result;
  result.reserve(_size_);

  QVector<double> key_vec;
  key_vec.reserve(_size_);

  kfr::univector<double> data;
  data.reserve(_size_);

  kfr::univector<double, 7> taps;

  kfr::expression_pointer<double> window_function_pointer;

  AskForWindowFunction window_function_dialog;
  int window_int = window_function_dialog.get_value();

  switch(window_int)
    {
    case 1: window_function_pointer  = kfr::to_pointer(kfr::window_bartlett(taps.size())); break;
    case 2: window_function_pointer  = kfr::to_pointer(kfr::window_bartlett_hann(taps.size())); break;
    case 3: window_function_pointer  = kfr::to_pointer(kfr::window_blackman(taps.size())); break;
    case 4: window_function_pointer  = kfr::to_pointer(kfr::window_blackman_harris(taps.size())); break;
    case 5: window_function_pointer  = kfr::to_pointer(kfr::window_bohman(taps.size())); break;
    case 6: window_function_pointer  = kfr::to_pointer(kfr::window_cosine(taps.size())); break;
    case 7: window_function_pointer  = kfr::to_pointer(kfr::window_flattop(taps.size())); break;
    case 8: window_function_pointer  = kfr::to_pointer(kfr::window_gaussian(taps.size())); break;
    case 9: window_function_pointer  = kfr::to_pointer(kfr::window_hamming(taps.size())); break;
    case 10: window_function_pointer = kfr::to_pointer(kfr::window_hann(taps.size())); break;
    case 11: window_function_pointer = kfr::to_pointer(kfr::window_kaiser(taps.size())); break;
    case 12: window_function_pointer = kfr::to_pointer(kfr::window_lanczos(taps.size())); break;
    case 13: window_function_pointer = kfr::to_pointer(kfr::window_rectangular(taps.size())); break;
    case 14: window_function_pointer = kfr::to_pointer(kfr::window_triangular(taps.size())); break;
    default: printf("Not a valid number.\n");
    }

  AskForFirFilter fir_filter_dialog;
  int fir_filter_int = fir_filter_dialog.get_value();

  kfr::filter_fir<double> filter(taps);

  switch(fir_filter_int)
    {
    case 1: filter = fir_lowpass_setup(window_function_pointer); break;
    case 2: filter = fir_highpass_setup(window_function_pointer); break;
    case 3: filter = fir_bandpass_setup(window_function_pointer); break;
    case 4: filter = fir_bandstop_setup(window_function_pointer); break;
    default: printf("Not a valid number.\n");
    }

  auto begin = (((QCPGraph *)plottable)->data())->constBegin();
  auto end   = (((QCPGraph *)plottable)->data())->constEnd();

  for(auto itr = begin; itr != end; itr++)
    {
      data.push_back(itr->value);
      key_vec.push_back(itr->key);
    }

  filter.apply(data);

  std::vector<double> vec;
  vec.reserve(_size_);

  for(auto i : data)
    vec.push_back(i);

  ((QCPGraph *)plottable)->setData(key_vec, QVector<double>::fromStdVector(vec));
  parent->replot();
}


void FilterContextMenu::convolution_action_slot()
{
}


void FilterContextMenu::dft_action_slot()
{
}


