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


AskForWindowFunction::AskForWindowFunction()
{
  QLabel * label = new QLabel(this);
  label->setText("Select Window-Function:\n(1) Bartlett \n(2) Bartlett-Hann\n(3) Blackman\n(4) Blackman-Harris\n(5) Bohman\n(6) Cosine\n(7) Flattop\n(8) Gaussian\n(9) Hamming\n(10) Hann\n(11) Kaiser\n(12) Lanczos\n(13) Rectangular\n(14) Triangular");
  QLineEdit * enter = new QLineEdit(this);
  connect(enter, &QLineEdit::editingFinished, [this,enter](){entry = enter->text().toInt();done(1);});
  QVBoxLayout * layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(enter);
  setLayout(layout);
  exec();
}


int AskForWindowFunction::get_value()
{
  return entry;
}


AskForFirFilter::AskForFirFilter()
{
  QLabel * label = new QLabel(this);
  label->setText("Select Filter:\n(1) FIR LowPass\n(2) FIR HighPass\n(3) FIR BandPass\n(4) BandStop");
  QLineEdit * enter = new QLineEdit(this);
  connect(enter, &QLineEdit::editingFinished, [this,enter](){entry = enter->text().toInt();done(1);});
  QVBoxLayout * layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(enter);
  setLayout(layout);
  exec();
}


int AskForFirFilter::get_value()
{
  return entry;
}


AskForFrequency::AskForFrequency()
{
  QLabel * label = new QLabel(this);
  label->setText("Set Cut-Off Frequency: ");
  QLineEdit * enter = new QLineEdit(this);
  connect(enter, &QLineEdit::editingFinished, [this,enter](){entry = enter->text().toDouble();done(1);});
  QVBoxLayout * layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(enter);
  setLayout(layout);
  exec();
}


double AskForFrequency::get_value()
{
  return entry;
}
