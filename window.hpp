#ifndef WINDOW_H
#define WINDOW_H


#include <QMainWindow>
#include "qcustomplot.h"
#include <exprtk.hpp>
#include <string>
#include <vector>


typedef enum
  {
    OFF, X, Y, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, Z8, Z9
  }MODE;


class Window;
class MathWindow;

class Equation : public QWidget
{
  Q_OBJECT

public:
                                  Equation(QString, MathWindow *);
  QGridLayout *                   layout;
  QGroupBox *                     box;
  exprtk::symbol_table<double>  * symbol_table;
  exprtk::expression<double>    * expression;
  exprtk::parser<double>        * parser;


private:
  MathWindow *                parent;
  Window *                    window_parent;
  std::vector<double>         parameters;

};


class MathWindow : public QWidget
{
  Q_OBJECT

public:
  MathWindow(Window *);
  Window *                    parent;
  QGridLayout *               layout;
  QLineEdit *                 entryField;
  QPushButton *               eval_button;
  QMap<QString, Equation*> *  map;

public slots:
  void                        eval_slot();
};


typedef struct ColorMapDataHolder
{
  QSharedPointer< QCPGraphDataContainer > X;
  QSharedPointer< QCPGraphDataContainer > Y;
  QSharedPointer< QCPGraphDataContainer > Z;
}XYZ;


class PlotContextMenu : public QMenu
{
  Q_OBJECT

public:
        PlotContextMenu(QCustomPlot *, Window *);
  void  set_plottable(QCPAbstractPlottable *);

private:
  Window *                window_parent;
  QCPAbstractPlottable *  plottable;
  QCustomPlot *           parent;
  XYZ *                   xyz;

public slots:
  void                    copy_action_slot();
  void                    truncate_action_slot();
  void                    truncate_all_action_slot();
  void                    filter_action_slot();
  void                    truncate_selection_accepted_slot();
  void                    truncate_all_selection_accepted_slot();
  void                    truncate_at_cursor_action_slot();
  void                    truncate_all_at_cursor_action_slot();
  void                    set_as_x_action_slot();
  void                    set_as_y_action_slot();
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


class AskForPolandFile : public QDialog
{
  Q_OBJECT

public:
        AskForPolandFile();
  bool  get_value();

private:
  bool result;
};


class AskForNewGraphName : public QDialog
{
  Q_OBJECT

public:
          AskForNewGraphName();
  QString get_value();

private:
  QString name;
};


class Window : public QMainWindow
{
  Q_OBJECT

public:
  Window();

  QCustomPlot *           timePlot;
  QCustomPlot *           xyPlot;
  QCPColorMap *           colorMap;

  QCPItemStraightLine *   lowerLine;
  QCPItemStraightLine *   upperLine;

  XYZ                                           xyz;
  std::vector<double>                           data_holder;
  std::vector<exprtk::expression<double>*>      expression_holder;

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
  void                    plottableClick_slot(QCPAbstractPlottable *, int, QMouseEvent *);
  void                    mouseDoubleClick_slot(QMouseEvent *);

private:
  void                    actions();
  void                    load_file(std::string, int);
  void                    get_names_from_header_POLAND(std::string);

  PlotContextMenu *             plotcontextmenu;
  MathWindow *                  mathwindow;

protected:
  void contextMenuEvent(QContextMenuEvent *event) override;

};



#endif //WINDOW_H
