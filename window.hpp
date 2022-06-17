#ifndef WINDOW_H
#define WINDOW_H


#include <QMainWindow>
#include "qcustomplot.h"




class Window : public QMainWindow
{
  Q_OBJECT

public:
  Window();

  QCustomPlot *           timePlot;
  QCustomPlot *           xyPlot;
  QCPColorMap *           colorMap;

private:
  void                    actions();

protected:
  void contextMenuEvent(QContextMenuEvent *event) override;

};



#endif //WINDOW_H
