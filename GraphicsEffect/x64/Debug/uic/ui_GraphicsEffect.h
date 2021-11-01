/********************************************************************************
** Form generated from reading UI file 'GraphicsEffect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICSEFFECT_H
#define UI_GRAPHICSEFFECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphicsEffectClass
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QLabel *label_blur;
    QSlider *horizontalSlider_blur;
    QLabel *label_2;
    QLabel *label_3;
    QSlider *horizontalSlider_opacity;
    QLabel *label_4;
    QSlider *horizontalSlider_colorize;
    QLabel *label_5;
    QLabel *label_opacity;
    QLabel *label_7;
    QLabel *label_colorize;
    QLabel *label_6;
    QSlider *horizontalSlider_dropShadow;
    QLabel *label_8;
    QLabel *label_dropShadow;
    QLabel *label_9;

    void setupUi(QWidget *GraphicsEffectClass)
    {
        if (GraphicsEffectClass->objectName().isEmpty())
            GraphicsEffectClass->setObjectName(QString::fromUtf8("GraphicsEffectClass"));
        GraphicsEffectClass->resize(800, 741);
        verticalLayout = new QVBoxLayout(GraphicsEffectClass);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(GraphicsEffectClass);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
"font-size: 14px;\n"
"font-family: \345\276\256\350\275\257\351\233\205\351\273\221;\n"
"color: #000000;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_blur = new QLabel(frame);
        label_blur->setObjectName(QString::fromUtf8("label_blur"));
        label_blur->setGeometry(QRect(80, 50, 192, 192));
        label_blur->setStyleSheet(QString::fromUtf8("border-image: url(:/GraphicsEffect/qrc/1.png);"));
        horizontalSlider_blur = new QSlider(frame);
        horizontalSlider_blur->setObjectName(QString::fromUtf8("horizontalSlider_blur"));
        horizontalSlider_blur->setGeometry(QRect(140, 610, 251, 20));
        horizontalSlider_blur->setMaximum(30);
        horizontalSlider_blur->setPageStep(10);
        horizontalSlider_blur->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(100, 610, 31, 21));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(410, 610, 31, 21));
        horizontalSlider_opacity = new QSlider(frame);
        horizontalSlider_opacity->setObjectName(QString::fromUtf8("horizontalSlider_opacity"));
        horizontalSlider_opacity->setGeometry(QRect(450, 610, 251, 20));
        horizontalSlider_opacity->setMaximum(10);
        horizontalSlider_opacity->setPageStep(10);
        horizontalSlider_opacity->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(100, 660, 31, 21));
        horizontalSlider_colorize = new QSlider(frame);
        horizontalSlider_colorize->setObjectName(QString::fromUtf8("horizontalSlider_colorize"));
        horizontalSlider_colorize->setGeometry(QRect(140, 660, 251, 20));
        horizontalSlider_colorize->setMaximum(10);
        horizontalSlider_colorize->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(158, 10, 31, 21));
        label_opacity = new QLabel(frame);
        label_opacity->setObjectName(QString::fromUtf8("label_opacity"));
        label_opacity->setGeometry(QRect(530, 50, 192, 192));
        label_opacity->setStyleSheet(QString::fromUtf8("border-image: url(:/GraphicsEffect/qrc/2.png);"));
        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(610, 10, 31, 21));
        label_colorize = new QLabel(frame);
        label_colorize->setObjectName(QString::fromUtf8("label_colorize"));
        label_colorize->setGeometry(QRect(80, 330, 192, 192));
        label_colorize->setStyleSheet(QString::fromUtf8("border-image: url(:/GraphicsEffect/qrc/3.png);"));
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(160, 290, 31, 21));
        horizontalSlider_dropShadow = new QSlider(frame);
        horizontalSlider_dropShadow->setObjectName(QString::fromUtf8("horizontalSlider_dropShadow"));
        horizontalSlider_dropShadow->setGeometry(QRect(450, 660, 251, 20));
        horizontalSlider_dropShadow->setMaximum(100);
        horizontalSlider_dropShadow->setOrientation(Qt::Horizontal);
        label_8 = new QLabel(frame);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(410, 660, 31, 21));
        label_dropShadow = new QLabel(frame);
        label_dropShadow->setObjectName(QString::fromUtf8("label_dropShadow"));
        label_dropShadow->setGeometry(QRect(530, 330, 192, 192));
        label_dropShadow->setStyleSheet(QString::fromUtf8("border-image: url(:/GraphicsEffect/qrc/4.png);"));
        label_9 = new QLabel(frame);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(610, 290, 31, 21));

        verticalLayout->addWidget(frame);


        retranslateUi(GraphicsEffectClass);

        QMetaObject::connectSlotsByName(GraphicsEffectClass);
    } // setupUi

    void retranslateUi(QWidget *GraphicsEffectClass)
    {
        GraphicsEffectClass->setWindowTitle(QApplication::translate("GraphicsEffectClass", "GraphicsEffect", nullptr));
        label_blur->setText(QString());
        label_2->setText(QApplication::translate("GraphicsEffectClass", "\346\250\241\347\263\212", nullptr));
        label_3->setText(QApplication::translate("GraphicsEffectClass", "\351\200\217\346\230\216", nullptr));
        label_4->setText(QApplication::translate("GraphicsEffectClass", "\351\242\234\350\211\262", nullptr));
        label_5->setText(QApplication::translate("GraphicsEffectClass", "\346\250\241\347\263\212", nullptr));
        label_opacity->setText(QString());
        label_7->setText(QApplication::translate("GraphicsEffectClass", "\351\200\217\346\230\216", nullptr));
        label_colorize->setText(QString());
        label_6->setText(QApplication::translate("GraphicsEffectClass", "\351\242\234\350\211\262", nullptr));
        label_8->setText(QApplication::translate("GraphicsEffectClass", "\351\230\264\345\275\261", nullptr));
        label_dropShadow->setText(QString());
        label_9->setText(QApplication::translate("GraphicsEffectClass", "\351\230\264\345\275\261", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphicsEffectClass: public Ui_GraphicsEffectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICSEFFECT_H
