#ifndef WINDOW_H
#define WINDOW_H


#include <QMainWindow>
#include "qcustomplot.h"
#include <string>

class Window;

class PlotContextMenu : public QMenu
{
  Q_OBJECT

public:
  PlotContextMenu(QCustomPlot *, Window *);
  void set_plottable(QCPAbstractPlottable *);

private:
  Window *                window_parent;
  QCPAbstractPlottable *  plottable;
  QCustomPlot *           parent;

public slots:
  void                    copy_action_slot();
  void                    truncate_action_slot();
  void                    truncate_all_action_slot();
  void                    filter_action_slot();
  void                    truncate_selection_accepted_slot();
  void                    truncate_all_selection_accepted_slot();
  void                    truncate_at_cursor_action_slot();
  void                    truncate_all_at_cursor_action_slot();
  void                    set_as_z_action_slot();
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

  QCPItemStraightLine *           lowerLine;
  QCPItemStraightLine *           upperLine;

public slots:
  void                    open_action_slot();
  void                    save_action_slot();
  void                    save_as_action_slot();
  void                    import_action_slot();
  void                    export_action_slot();
  void                    math_action_slot();
  void                    range_cursor_action_slot();
  void                    mousePress_slot(QMouseEvent *);
  void                    change_lowerLine_position_slot(QMouseEvent *);
  void                    change_upperLine_position_slot(QMouseEvent *);
  void                    mouseRelease_slot();
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
