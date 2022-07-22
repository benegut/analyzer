#include "window.hpp"

AskForHeader::AskForHeader()
{
  QLabel *        label = new QLabel(this);
  label->setText("How many headers?");
  QLineEdit *     enter = new QLineEdit(this);
  connect(enter, &QLineEdit::editingFinished, [this,enter](){headers_value = enter->text().toInt();done(1);});
  enter->setText("1");
  QVBoxLayout *   layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(enter);
  setLayout(layout);
  exec();
}


int AskForHeader::get_value()
{
  return headers_value;
}


AskForPolandFile::AskForPolandFile()
{
  QLabel *        label = new QLabel(this);
  label->setText("File from last Poland trip?");
  QPushButton * YesButton = new QPushButton("Yes", this);
  connect(YesButton, &QPushButton::clicked, [this](){result = true;done(1);});
  QPushButton * NoButton = new QPushButton("No", this);
  connect(NoButton, &QPushButton::clicked, [this](){result = false;done(1);});
  QVBoxLayout * layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(YesButton);
  layout->addWidget(NoButton);
  setLayout(layout);
  exec();
}


bool AskForPolandFile::get_value()
{
  return result;
}


AskForNewGraphName::AskForNewGraphName()
{
  QLabel *        label = new QLabel(this);
  label->setText("Graphs new name ([u-w][0-9]):");
  QLineEdit *     enter = new QLineEdit(this);
  connect(enter, &QLineEdit::editingFinished, [this,enter](){name = enter->text();done(1);});
  QVBoxLayout *   layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(enter);
  setLayout(layout);
  exec();
}


QString AskForNewGraphName::get_value()
{
  return name;
}


AskForMovingAverageRange::AskForMovingAverageRange()
{
  QLabel *        label = new QLabel(this);
  label->setText("Range for Moving Average: ");
  QLineEdit *     enter = new QLineEdit(this);
  connect(enter, &QLineEdit::editingFinished, [this,enter](){range = enter->text().toInt();done(1);});
  QVBoxLayout *   layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(enter);
  setLayout(layout);
  exec();
}


int AskForMovingAverageRange::get_value()
{
  return range;
}
