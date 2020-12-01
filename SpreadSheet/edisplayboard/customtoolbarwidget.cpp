#include "customtoolbarwidget.h"
#include "ui_customtoolbarwidget.h"

CustomToolBarWidget::CustomToolBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomToolBarWidget)
{
    ui->setupUi(this);
}

CustomToolBarWidget::~CustomToolBarWidget()
{
    delete ui;
}
