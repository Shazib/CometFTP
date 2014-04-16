#include "StatusArea.h"

#include <QtWidgets>

StatusArea::StatusArea(QWidget *parent) :
    QWidget(parent)
{
    // Init
    this->setObjectName("StatusArea");
    this->setFixedHeight(70);

    // Required Widgets
    QLabel* file = new QLabel("Filename");
    QPushButton* numFiles = new QPushButton("255");
    QPushButton* cancel = new QPushButton(" ");
    QPushButton* pause = new QPushButton(" ");

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
    QProgressBar* progress = new QProgressBar();
    progress->setObjectName("Status_Progress");
    progress->setMaximum(99);
    progress->setMinimum(0);
    progress->setRange(0,99);
    progress->setValue(50);
    progress->setTextVisible(false);
    progress->setFixedHeight(12);
    mid->addWidget(progress);
    progress->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    // Bottom Half
    bottomRight->addWidget(numFiles);
    bottomRight->setAlignment(Qt::AlignRight);
    bottomLeft->addWidget(shape1);
    bottomLeft->addWidget(shape2);
    bottomLeft->setAlignment(Qt::AlignLeft);
    bottomLeft->setSpacing(0);
    bottom->addLayout(bottomLeft);
    bottom->addLayout(mid);
    bottom->addLayout(bottomRight);


    main->addLayout(top);
    main->addLayout(bottom);

    this->setLayout(main);

}

// Enable StyleSheets
void statusArea::paintEvent(QPaintEvent* event){
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);

}
