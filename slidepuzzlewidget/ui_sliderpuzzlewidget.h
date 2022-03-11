/********************************************************************************
** Form generated from reading UI file 'sliderpuzzlewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLIDERPUZZLEWIDGET_H
#define UI_SLIDERPUZZLEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "puzzlewidget.h"

QT_BEGIN_NAMESPACE

class Ui_SliderPuzzleWidget
{
public:
    QVBoxLayout *verticalLayout;
    PuzzleWidget *puzzlewidget;
    QSlider *horizontalSlider;

    void setupUi(QWidget *SliderPuzzleWidget)
    {
        if (SliderPuzzleWidget->objectName().isEmpty())
            SliderPuzzleWidget->setObjectName(QString::fromUtf8("SliderPuzzleWidget"));
        SliderPuzzleWidget->resize(717, 320);
        verticalLayout = new QVBoxLayout(SliderPuzzleWidget);
        verticalLayout->setSpacing(14);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        puzzlewidget = new PuzzleWidget(SliderPuzzleWidget);
        puzzlewidget->setObjectName(QString::fromUtf8("puzzlewidget"));

        verticalLayout->addWidget(puzzlewidget);

        horizontalSlider = new QSlider(SliderPuzzleWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalSlider);


        retranslateUi(SliderPuzzleWidget);

        QMetaObject::connectSlotsByName(SliderPuzzleWidget);
    } // setupUi

    void retranslateUi(QWidget *SliderPuzzleWidget)
    {
        SliderPuzzleWidget->setWindowTitle(QApplication::translate("SliderPuzzleWidget", "SliderPuzzleWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SliderPuzzleWidget: public Ui_SliderPuzzleWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLIDERPUZZLEWIDGET_H
