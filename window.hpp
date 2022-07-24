#ifndef WINDOW_H
#define WINDOW_H


#include <QMainWindow>
#include "qcustomplot.h"
#include <exprtk.hpp>
#include <string>
#include <kfr/all.hpp>


typedef enum
  {
    OFF, X, Y, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, Z8, Z9
  }MODE;


class Window;
class MathWindow;
class VideoRunner;
class FilterContextMenu;

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
  MathWindow *                 parent;
  Window *                     window_parent;
  QHash<QString, double>       parameters;

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
  FilterContextMenu *     filtercontextmenu;
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



class FilterContextMenu : public QMenu
{
  Q_OBJECT

public:
  FilterContextMenu(QCustomPlot *, PlotContextMenu *);

private:
  PlotContextMenu *       window_parent;
  QCustomPlot *           parent;
  QCPAbstractPlottable *  plottable;

  kfr::filter_fir<double> fir_lowpass_setup(kfr::expression_pointer<double>);
  kfr::filter_fir<double> fir_highpass_setup(kfr::expression_pointer<double>);
  kfr::filter_fir<double> fir_bandpass_setup(kfr::expression_pointer<double>);
  kfr::filter_fir<double> fir_bandstop_setup(kfr::expression_pointer<double>);

public slots:
  void                    set_plottable(QCPAbstractPlottable *);
  void                    moving_average_action_slot();
  void                    biquad_action_slot();
  void                    fir_action_slot();
  void                    iir_action_slot();
  void                    convolution_action_slot();
  void                    dft_action_slot();

};


class AskForMovingAverageRange : public QDialog
{
  Q_OBJECT

public:
  AskForMovingAverageRange();
  int get_value();

private:
  int range;
};


class AskForWindowFunction : public QDialog
{
  Q_OBJECT

public:
  AskForWindowFunction();
  int get_value();

private:
  int entry;
};


class AskForFirFilter : public QDialog
{
  Q_OBJECT

public:
  AskForFirFilter();
  int get_value();

private:
  int entry;
};


class AskForFrequency : public QDialog
{
  Q_OBJECT

public:
  AskForFrequency();
  double  get_value();

private:
  double entry;
};


class AskForLowerFrequency : public QDialog
{
  Q_OBJECT

public:
  AskForLowerFrequency();
  double  get_value();

private:
  double entry;
};


class AskForUpperFrequency : public QDialog
{
  Q_OBJECT

public:
  AskForUpperFrequency();
  double  get_value();

private:
  double entry;
};




class AskForIirOptions : public QDialog
{
  Q_OBJECT

public:
  AskForIirOptions();
  std::string get_value();

private:
  int entry;
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





class VideoWindow : public QWidget
{
  Q_OBJECT

public:
  VideoWindow(Window *);
  QCPItemStraightLine *   video_line;

  QPushButton *           play_button;


private:
  Window *                parent;
  QCustomPlot *           parent_plot;
  QGridLayout *           layout;

public slots:
  void                    show_slot();
  void                    play_button_slot();
  void                    fast_forward_button_slot();
  void                    rewind_button_slot();
  void                    picture_button_slot();
  void                    video_button_slot();
  void                    mousePress_slot(QMouseEvent *);
  void                    mouseRelease_slot();
  void                    change_video_line_position_slot(QMouseEvent *);
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

  XYZ                                          xyz;
  QHash<QString, double>                       data_holder;
  QHash<QString, exprtk::expression<double>*>  expression_holder;
  QHash<QString, QString>                      name_to_equation;
  QHash<QString, QString>                      equation_to_name;

  VideoWindow *                 videowindow;
  VideoRunner *                 videorunner;

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
  void                    recalculate_math_graphs_action_slot();
  void                    data(double, double, double, double);
  void                    replot_slot();
  void                    sizebox_slot(int);
  void                    set_greyscale();
  void                    greyscale_rect_action_slot();
  void                    set_greyscale_from_selection_rect_start(QMouseEvent *);
  void                    set_greyscale_from_selection_rect_accepted(QRect, QMouseEvent *);

private:
  void                    actions();
  void                    load_file(std::string, int);
  void                    get_names_from_header_POLAND(std::string);

  QToolBar *                    toolbar;
  QSpinBox *                    sizebox;
  QDoubleSpinBox *              greyscale_offset_box;
  QDoubleSpinBox *              greyscale_amplitude_box;
  double                        greyscale_raw_value_1, greyscale_raw_value_2;
  PlotContextMenu *             plotcontextmenu;
  MathWindow *                  mathwindow;

protected:
  void contextMenuEvent(QContextMenuEvent *event) override;

};


class VideoRunner : public QThread
{
  Q_OBJECT

public:
  VideoRunner(Window *);
  void  run() override;
  bool video_is_running;

private:
  Window *                        parent;
  QCPDataContainer<QCPGraphData>  x;
  QCPDataContainer<QCPGraphData>  y;
  QCPDataContainer<QCPGraphData>  z;

signals:
  void data(double, double, double, double);
  void replot_signal();

};



#endif //WINDOW_H
