/********************************************************************************
** Form generated from reading UI file 'frmslidepuzzlewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMSLIDEPUZZLEWIDGET_H
#define UI_FRMSLIDEPUZZLEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "sliderpuzzlewidget.h"

QT_BEGIN_NAMESPACE

class Ui_FrmSlidePuzzleWidgetClass
{
public:
    QVBoxLayout *verticalLayout;
    SliderPuzzleWidget *sliderpuzzleWidget;

    void setupUi(QWidget *FrmSlidePuzzleWidgetClass)
    {
        if (FrmSlidePuzzleWidgetClass->objectName().isEmpty())
            FrmSlidePuzzleWidgetClass->setObjectName(QString::fromUtf8("FrmSlidePuzzleWidgetClass"));
        FrmSlidePuzzleWidgetClass->resize(600, 400);
        verticalLayout = new QVBoxLayout(FrmSlidePuzzleWidgetClass);
        verticalLayout->setSpacing(20);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        sliderpuzzleWidget = new SliderPuzzleWidget(FrmSlidePuzzleWidgetClass);
        sliderpuzzleWidget->setObjectName(QString::fromUtf8("sliderpuzzleWidget"));

        verticalLayout->addWidget(sliderpuzzleWidget);


        retranslateUi(FrmSlidePuzzleWidgetClass);

        QMetaObject::connectSlotsByName(FrmSlidePuzzleWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *FrmSlidePuzzleWidgetClass)
    {
        FrmSlidePuzzleWidgetClass->setWindowTitle(QApplication::translate("FrmSlidePuzzleWidgetClass", "FrmSlidePuzzleWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FrmSlidePuzzleWidgetClass: public Ui_FrmSlidePuzzleWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMSLIDEPUZZLEWIDGET_H
