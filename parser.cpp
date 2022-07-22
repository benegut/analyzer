#include "window.hpp"

#include <iostream>
#include <cstdio>


MathWindow::MathWindow(Window * parent)
  : parent(parent)
  , layout(new QGridLayout)
  , entryField(new QLineEdit)
  , eval_button(new QPushButton(tr("&Eval")))
  , map(new QMap<QString, Equation*>)
{
  layout->addWidget(entryField, 0, 0);
  layout->addWidget(eval_button, 0, 1);
  setLayout(layout);

  connect(eval_button, &QPushButton::clicked, this, &MathWindow::eval_slot);
}


void MathWindow::eval_slot()
{
  QString str = entryField->text().simplified().replace(" ","").toLower();
  if(!map->contains(str))
    map->insert(str, new Equation(str, this));

  close();
}



Equation::Equation(QString equation, MathWindow * p)
  : parent(p)
  , window_parent(p->parent)
  , layout(new QGridLayout)
  , box(new QGroupBox(p))
  , symbol_table(new exprtk::symbol_table<double>)
  , expression(new exprtk::expression<double>)
  , parser(new exprtk::parser<double>)
{
  if(!window_parent->expression_holder.contains(window_parent->equation_to_name[equation]))
    {
      parameters.reserve(100);

      // register parameters
      for(auto c: equation)
        {
          QString str(c);

          if(str.contains(QRegExp("[a-m]")) && !parameters.contains(str))
            {
              parameters.insert(str, 1.0);

              QSlider * slider = new QSlider(Qt::Vertical, this);
              slider->setMaximum(100);
              slider->setMinimum(0);
              slider->setSliderPosition(100);
              layout->addWidget(slider, 1, layout->columnCount());
              connect(slider, &QSlider::valueChanged, [this, slider, equation, str](){this->parameters[str] = (double)slider->value()/100.0;});

              QLabel *  label  = new QLabel(str, this);
              layout->addWidget(label, 0, layout->columnCount());

              if(!symbol_table->add_variable(str.toStdString(), parameters[str]))
                std::cout << "Error in symbol table\n";
            }
        }


      // register variables
      QRegExp rx("[u-z][0-9]");
      int pos = 0;
      while(((pos = rx.indexIn(equation, pos)) != -1) && !parameters.contains(rx.cap(0)))
        {
          parameters.insert(rx.cap(0), 0.0);
          QString str = rx.cap(0).toUpper();
          for(int i=0;i<window_parent->timePlot->graphCount();i++)
            {
              if(str == window_parent->timePlot->graph(i)->name().toUpper() &&
                 window_parent->data_holder.contains(str))
                symbol_table->add_variable(str.toStdString(), window_parent->data_holder[str]);
              else if(str == window_parent->timePlot->graph(i)->name().toUpper() &&
                      !window_parent->data_holder.contains(str))
                {
                  window_parent->data_holder.insert(str, 0.0);
                  symbol_table->add_variable(str.toStdString(), window_parent->data_holder[str]);
                }
            }
          pos += rx.matchedLength();
        }


      // compile expression
      expression->register_symbol_table(*symbol_table);
      if(!parser->compile(equation.toStdString(), *expression))
        printf("Error: %s\n", parser->error().c_str());


      // set name for expression, and calculate graph
      QString name;
      do
        {
          AskForNewGraphName dialog;
          name = dialog.get_value();
        }
      while(window_parent->data_holder.contains(name));

      window_parent->data_holder.insert(name, 0.0);
      window_parent->expression_holder.insert(name, expression);
      window_parent->name_to_equation.insert(name, equation);
      window_parent->equation_to_name.insert(equation, name);

      window_parent->timePlot->addGraph();
      window_parent->timePlot->graph()->setName(name);
      window_parent->recalculate_math_graphs_action_slot();
      window_parent->timePlot->replot();

      box->setLayout(layout);
      box->setTitle(equation + " / " + name);
      parent->layout->addWidget(box);
    }
}
