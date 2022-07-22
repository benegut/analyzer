#include "window.hpp"
#include <unistd.h>
#include <kfr/all.hpp>
#include <vector>


void PlotContextMenu::filter_action_slot()
{
  int size = (((QCPGraph *)plottable)->data())->size();

  QVector<double> key_vec;
  key_vec.reserve(size);

  kfr::univector<double> data;
  data.reserve(size);

  kfr::univector<double, 7> taps;

  kfr::expression_pointer<double> kaiser = kfr::to_pointer(kfr::window_kaiser(taps.size(), 3.0));
  fir_lowpass(taps, 0.00001, kaiser, true);
  kfr::filter_fir<double> filter(taps);

  auto begin = (((QCPGraph *)plottable)->data())->constBegin();
  auto end   = (((QCPGraph *)plottable)->data())->constEnd();

  for(auto itr = begin; itr != end; itr++)
    {
      data.push_back(itr->value);
      key_vec.push_back(itr->key);
    }

  filter.apply(data);

  std::vector<double> vec;
  vec.reserve(size);

  for(auto i : data)
    vec.push_back(i);

  ((QCPGraph *)plottable)->setData(key_vec, QVector<double>::fromStdVector(vec));
  parent->replot();
}
