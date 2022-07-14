#include "window.hpp"
#include <unistd.h>
#include <kfr/all.hpp>
#include <vector>


void PlotContextMenu::filter_action_slot()
{
  QVector<double> key_vec;
  key_vec.reserve((((QCPGraph *)plottable)->data())->size());
  kfr::univector<double> uni_vec;
  std::vector<double> vec;
  vec.reserve((((QCPGraph *)plottable)->data())->size());
  uni_vec.reserve((((QCPGraph *)plottable)->data())->size());
  auto begin = (((QCPGraph *)plottable)->data())->constBegin();
  auto end   = (((QCPGraph *)plottable)->data())->constEnd();
  for(auto itr = begin; itr != end; itr++)
    {
      uni_vec.push_back(itr->value);
      key_vec.push_back(itr->key);
    }
  kfr::expression_pointer<double> rectangular = kfr::to_pointer(kfr::window_rectangular(uni_vec.size()));
  kfr::fir_lowpass(uni_vec, 0.00008 ,rectangular,false);
  for(auto i : uni_vec)
    vec.push_back(i);
  ((QCPGraph *)plottable)->setData(key_vec, QVector<double>::fromStdVector(vec));
  parent->replot();
}
