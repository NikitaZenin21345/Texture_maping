#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Texture_maping.h"

class Texture_maping : public QMainWindow
{
    Q_OBJECT

public:
    Texture_maping(QWidget *parent = nullptr);
    ~Texture_maping();

private:
    Ui::Texture_mapingClass ui;
};
