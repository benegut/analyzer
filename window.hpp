#ifndef WINDOW_H
#define WINDOW_H


#include <QMainWindow>
#include "qcustomplot.h"




class Window : public QMainWindow
{
  Q_OBJECT

public:
  Window();

  QSplitter *             splitter;
  QCustomPlot *           timePlot;
  QCustomPlot *           xyPlot;
  QCPColorMap *           colorMap;

  //QMenu *                 view;

private:
  void                    actions();

protected:
  void contextMenuEvent(QContextMenuEvent *event) override;

};



#endif //WINDOW_H
