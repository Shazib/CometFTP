#include "StatusArea.h"

#include <QtWidgets>

StatusArea::StatusArea(QWidget *parent) :
    QWidget(parent)
{
    // Init
    this->setObjectName("StatusArea");
    this->setFixedHeight(70);

    // Required Widgets
    file = new QLabel("Filename");
    numFiles = new QPushButton("00");
    cancel = new QPushButton(" ");
    pause = new QPushButton(" ");
    speed = new QLabel("KBs" );
    cancel->setFixedSize(23, 23);
    pause->setFixedSize(23, 23);
    cancel->setObjectName("Status_CancelButton");
    pause->setObjectName("Status_PauseButton");
    numFiles->setObjectName("Status_NumberFiles");

    // Layouts
    QVBoxLayout* main = new QVBoxLayout();
    QHBoxLayout* top = new QHBoxLayout();
    QHBoxLayout* topLeft = new QHBoxLayout();
    QHBoxLayout* topRight = new QHBoxLayout();
    QHBoxLayout* mid = new QHBoxLayout();
    QHBoxLayout* bottom = new QHBoxLayout();
    QHBoxLayout* bottomLeft = new QHBoxLayout();
    QHBoxLayout* bottomRight = new QHBoxLayout();

    // Top Half
    topLeft->addWidget(file);
    topLeft->setAlignment(Qt::AlignLeft);
    topRight->addWidget(speed);
    topRight->setAlignment(Qt::AlignRight);
    top->addLayout(topLeft);
    top->addLayout(topRight);

    // Setup progressbar
    progress = new QProgressBar();
    progress->setObjectName("Status_Progress");
    progress->setMaximum(100);
    progress->setMinimum(0);
    progress->setRange(0,100);
    progress->setValue(0);
    progress->setTextVisible(false);
    progress->setFixedHeight(12);
    mid->addWidget(progress);
    progress->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    // Bottom Half
    bottomRight->addWidget(numFiles);
    bottomRight->setAlignment(Qt::AlignRight);
    bottomLeft->addWidget(cancel);
    bottomLeft->addWidget(pause);
    bottomLeft->setAlignment(Qt::AlignLeft);
    bottomLeft->setSpacing(0);
    bottom->addLayout(bottomLeft);
    bottom->addLayout(mid);
    bottom->addLayout(bottomRight);


    main->addLayout(top);
    main->addLayout(bottom);

    this->setLayout(main);

    // Slots
    QObject::connect(numFiles, SIGNAL(clicked()),this->parent(), SLOT(switchSlides()));
}

// Enable StyleSheets
void StatusArea::paintEvent(QPaintEvent* event)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);

}

void StatusArea::setFileName(QString fileName)
{
   file->setText("File: " +fileName);
}

void StatusArea::setNumFiles(int _numFiles)
{
    QString s = QString::number(_numFiles);
    if (s.count() == 1){
        s = " " + s + " ";
    }
    numFiles->setText(s);
}

void StatusArea::setSpeed(QString _speed)
{
    speed->setText(_speed + " KB/s");
}

void StatusArea::setProgress(int progressValue)
{
    progress->setValue(progressValue);
}
