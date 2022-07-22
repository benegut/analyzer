#include "window.hpp"

VideoWindow::VideoWindow(Window * p)
  : parent(p)
  , parent_plot(p->timePlot)
  , layout(new QGridLayout(this))
  , video_line(new QCPItemStraightLine(p->timePlot))
  , play_button(new QPushButton(tr("&Play")))
{
  video_line->point1->setCoords(0, 0);
  video_line->point2->setCoords(0, 100);
  video_line->setVisible(false);
  video_line->setSelectable(false);

  connect(play_button, &QPushButton::clicked, this, &VideoWindow::play_button_slot);
  layout->addWidget(play_button);

  QPushButton * fast_forward_button = new  QPushButton(tr("&>>"));
  connect(fast_forward_button, &QPushButton::clicked, this, &VideoWindow::fast_forward_button_slot);
  layout->addWidget(fast_forward_button);

  QPushButton * rewind_button = new  QPushButton(tr("&<<"));
  connect(rewind_button, &QPushButton::clicked, this, &VideoWindow::rewind_button_slot);
  layout->addWidget(rewind_button);

  QPushButton * picture_button = new  QPushButton(tr("&Picture"));
  connect(picture_button, &QPushButton::clicked, this, &VideoWindow::picture_button_slot);
  layout->addWidget(picture_button);

  QPushButton * video_button = new  QPushButton(tr("&Video"));
  connect(video_button, &QPushButton::clicked, this, &VideoWindow::video_button_slot);
  layout->addWidget(video_button);

  setLayout(layout);

  connect(parent->timePlot, &QCustomPlot::mousePress, this, &VideoWindow::mousePress_slot);
  connect(parent->timePlot, &QCustomPlot::mouseRelease, this, &VideoWindow::mouseRelease_slot);
}


void VideoWindow::show_slot()
{
  this->show();
  video_line->setVisible(true);
}


void VideoWindow::play_button_slot()
{
  if(!parent->videorunner->video_is_running)
    {
      play_button->setText(tr("&Stop"));
      parent->videorunner->start();
    }
  else
    {
      parent->videorunner->video_is_running = false;
      play_button->setText(tr("&Play"));
    }
}


void VideoWindow::fast_forward_button_slot()
{
}


void VideoWindow::rewind_button_slot()
{
}


void VideoWindow::picture_button_slot()
{
}


void VideoWindow::video_button_slot()
{
}


void VideoWindow::mousePress_slot(QMouseEvent * event)
{
  if((event->pos().x() > video_line->point1->pixelPosition().x()-3) &&
     (event->pos().x() < video_line->point1->pixelPosition().x()+3) &&
     video_line->realVisibility())
    {
      connect(parent->timePlot, &QCustomPlot::mouseMove, this, &VideoWindow::change_video_line_position_slot);
      parent->timePlot->setInteraction(QCP::iRangeDrag, false);
    }
}


void VideoWindow::change_video_line_position_slot(QMouseEvent * event)
{
  video_line->point1->setCoords(parent->timePlot->xAxis->pixelToCoord(event->pos().x()), 0);
  video_line->point2->setCoords(parent->timePlot->xAxis->pixelToCoord(event->pos().x()),100);
  parent->timePlot->replot();
}


void VideoWindow::mouseRelease_slot()
{
  disconnect(parent->timePlot, &QCustomPlot::mouseMove, this, &VideoWindow::change_video_line_position_slot);
  parent->timePlot->setInteraction(QCP::iRangeDrag, true);
}



VideoRunner::VideoRunner(Window * p)
  : parent(p)
  , video_is_running(false)
{
}


void VideoRunner::run()
{
  x.set(*(parent->xyz.X));
  y.set(*(parent->xyz.Y));
  z.set(*(parent->xyz.Z));

  QCPDataContainer<QCPGraphData>::const_iterator startX = x.findBegin(parent->videowindow->video_line->point1->key());
  QCPDataContainer<QCPGraphData>::const_iterator endX   = x.constEnd();
  QCPDataContainer<QCPGraphData>::const_iterator startY = y.findBegin(parent->videowindow->video_line->point1->key());
  QCPDataContainer<QCPGraphData>::const_iterator startZ = z.findBegin(parent->videowindow->video_line->point1->key());

  int counter = 0;

  auto itr = startX;

  video_is_running = true;

  while(video_is_running && itr != endX)
    {
      double k_val = itr->key;
      double x_val = itr->value;
      double y_val = startY->value;
      double z_val = startZ->value;

      emit(data(x_val,y_val,z_val,k_val));

      itr++;
      startY++;
      startZ++;

      if(counter%1000 == 0)
        emit(replot_signal());

      counter++;

      usleep(0.05);
    }
}
