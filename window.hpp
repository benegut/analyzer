#ifndef WINDOW_H
#define WINDOW_H


#include <QMainWindow>
#include "qcustomplot.h"
#include <string>


class PlotContextMenu : public QMenu
{
  Q_OBJECT

public:
  PlotContextMenu();
  void set_plottable(QCPAbstractPlottable *);

private:
  QCPAbstractPlottable *  plottable;

public slots:
  void                    copy_action_slot();
  void                    truncate_action_slot();
  void                    filter_action_slot();
};


class AskForHeader : public QDialog
{
  Q_OBJECT

public:
  AskForHeader();
  int get_value();

private:
  int headers_value;
};


class Window : public QMainWindow
{
  Q_OBJECT

public:
  Window();

  QCustomPlot *           timePlot;
  QCustomPlot *           xyPlot;
  QCPColorMap *           colorMap;


public slots:
  void                    open_action_slot();
  void                    save_action_slot();
  void                    save_as_action_slot();
  void                    import_action_slot();
  void                    export_action_slot();
  void                    math_action_slot();
  void                    plottableClick_slot(QCPAbstractPlottable *,
                                              int,
                                              QMouseEvent *);
  void                    mouseDoubleClick_slot(QMouseEvent *);

  void                    load_file(std::string, int);

private:
  void                    actions();

  PlotContextMenu *       plotcontextmenu;

protected:
  void contextMenuEvent(QContextMenuEvent *event) override;

};



#endif //WINDOW_H
