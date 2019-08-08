/********************************************************************************
** Form generated from reading UI file 'Samples_Qt_TriggerCount.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMPLES_QT_TRIGGERCOUNT_H
#define UI_SAMPLES_QT_TRIGGERCOUNT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <mvscrollarea.h>

QT_BEGIN_NAMESPACE

class Ui_Samples_Qt_TriggerCountClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *lxcj;
    QPushButton *rcf;
    QPushButton *wcf;
    QPushButton *tz;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QComboBox *displayScale;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *Set;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *saveImage;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QLabel *pssl;
    QSpacerItem *horizontalSpacer;
    MVScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QLabel *showImage;

    void setupUi(QWidget *Samples_Qt_TriggerCountClass)
    {
        if (Samples_Qt_TriggerCountClass->objectName().isEmpty())
            Samples_Qt_TriggerCountClass->setObjectName(QStringLiteral("Samples_Qt_TriggerCountClass"));
        Samples_Qt_TriggerCountClass->resize(888, 760);
        Samples_Qt_TriggerCountClass->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        verticalLayout = new QVBoxLayout(Samples_Qt_TriggerCountClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(2, 6, 2, 2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lxcj = new QPushButton(Samples_Qt_TriggerCountClass);
        lxcj->setObjectName(QStringLiteral("lxcj"));

        horizontalLayout->addWidget(lxcj);

        rcf = new QPushButton(Samples_Qt_TriggerCountClass);
        rcf->setObjectName(QStringLiteral("rcf"));

        horizontalLayout->addWidget(rcf);

        wcf = new QPushButton(Samples_Qt_TriggerCountClass);
        wcf->setObjectName(QStringLiteral("wcf"));

        horizontalLayout->addWidget(wcf);

        tz = new QPushButton(Samples_Qt_TriggerCountClass);
        tz->setObjectName(QStringLiteral("tz"));
        tz->setEnabled(false);

        horizontalLayout->addWidget(tz);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        label_2 = new QLabel(Samples_Qt_TriggerCountClass);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        displayScale = new QComboBox(Samples_Qt_TriggerCountClass);
        displayScale->setObjectName(QStringLiteral("displayScale"));

        horizontalLayout->addWidget(displayScale);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        Set = new QPushButton(Samples_Qt_TriggerCountClass);
        Set->setObjectName(QStringLiteral("Set"));

        horizontalLayout->addWidget(Set);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        saveImage = new QPushButton(Samples_Qt_TriggerCountClass);
        saveImage->setObjectName(QStringLiteral("saveImage"));
        saveImage->setEnabled(true);

        horizontalLayout->addWidget(saveImage);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label = new QLabel(Samples_Qt_TriggerCountClass);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        pssl = new QLabel(Samples_Qt_TriggerCountClass);
        pssl->setObjectName(QStringLiteral("pssl"));
        pssl->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(pssl);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        scrollArea = new MVScrollArea(Samples_Qt_TriggerCountClass);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 882, 715));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, 0, 0, 0);
        showImage = new QLabel(scrollAreaWidgetContents);
        showImage->setObjectName(QStringLiteral("showImage"));
        showImage->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(showImage, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(Samples_Qt_TriggerCountClass);

        displayScale->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Samples_Qt_TriggerCountClass);
    } // setupUi

    void retranslateUi(QWidget *Samples_Qt_TriggerCountClass)
    {
        Samples_Qt_TriggerCountClass->setWindowTitle(QApplication::translate("Samples_Qt_TriggerCountClass", "Samples_Qt_TriggerCount", Q_NULLPTR));
        lxcj->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "\350\277\236\347\273\255\351\207\207\351\233\206", Q_NULLPTR));
        rcf->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "\350\275\257\350\247\246\345\217\221", Q_NULLPTR));
        wcf->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "\345\244\226\350\247\246\345\217\221", Q_NULLPTR));
        tz->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "\345\201\234\346\255\242", Q_NULLPTR));
        label_2->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "\346\230\276\347\244\272\346\257\224\344\276\213:", Q_NULLPTR));
        displayScale->clear();
        displayScale->insertItems(0, QStringList()
         << QApplication::translate("Samples_Qt_TriggerCountClass", "20%", Q_NULLPTR)
         << QApplication::translate("Samples_Qt_TriggerCountClass", "25%", Q_NULLPTR)
         << QApplication::translate("Samples_Qt_TriggerCountClass", "50%", Q_NULLPTR)
         << QApplication::translate("Samples_Qt_TriggerCountClass", "100%", Q_NULLPTR)
        );
        Set->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "\345\217\202\346\225\260\350\256\276\347\275\256", Q_NULLPTR));
        saveImage->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "\344\277\235\345\255\230\345\233\276\347\211\207", Q_NULLPTR));
        label->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "\346\213\215\346\221\204\346\225\260\351\207\217:", Q_NULLPTR));
        pssl->setText(QApplication::translate("Samples_Qt_TriggerCountClass", "0", Q_NULLPTR));
        showImage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Samples_Qt_TriggerCountClass: public Ui_Samples_Qt_TriggerCountClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMPLES_QT_TRIGGERCOUNT_H
