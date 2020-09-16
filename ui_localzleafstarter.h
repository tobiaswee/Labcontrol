/********************************************************************************
** Form generated from reading UI file 'localzleafstarter.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCALZLEAFSTARTER_H
#define UI_LOCALZLEAFSTARTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace lc {

class Ui_LocalzLeafStarter
{
public:
    QVBoxLayout *VLLocalzLeafStarter;
    QLabel *LzLeafName;
    QLineEdit *LEzLeafName;
    QLabel *LzLeafVersion;
    QComboBox *CBzLeafVersion;
    QLabel *LzLeafPort;
    QSpinBox *SBzLeafPort;
    QLabel *LLocalzLeafSize;
    QLineEdit *LELocalzLeafSize;
    QFrame *line_Start;
    QPushButton *PBStartLocalzLeaf;
    QPushButton *PBCancel;

    void setupUi(QWidget *lc__LocalzLeafStarter)
    {
        if (lc__LocalzLeafStarter->objectName().isEmpty())
            lc__LocalzLeafStarter->setObjectName(QStringLiteral("lc__LocalzLeafStarter"));
        lc__LocalzLeafStarter->resize(371, 352);
        VLLocalzLeafStarter = new QVBoxLayout(lc__LocalzLeafStarter);
        VLLocalzLeafStarter->setObjectName(QStringLiteral("VLLocalzLeafStarter"));
        LzLeafName = new QLabel(lc__LocalzLeafStarter);
        LzLeafName->setObjectName(QStringLiteral("LzLeafName"));

        VLLocalzLeafStarter->addWidget(LzLeafName);

        LEzLeafName = new QLineEdit(lc__LocalzLeafStarter);
        LEzLeafName->setObjectName(QStringLiteral("LEzLeafName"));
        LEzLeafName->setStyleSheet(QStringLiteral(""));

        VLLocalzLeafStarter->addWidget(LEzLeafName);

        LzLeafVersion = new QLabel(lc__LocalzLeafStarter);
        LzLeafVersion->setObjectName(QStringLiteral("LzLeafVersion"));

        VLLocalzLeafStarter->addWidget(LzLeafVersion);

        CBzLeafVersion = new QComboBox(lc__LocalzLeafStarter);
        CBzLeafVersion->setObjectName(QStringLiteral("CBzLeafVersion"));
        CBzLeafVersion->setStyleSheet(QStringLiteral(""));
        CBzLeafVersion->setMaxVisibleItems(32);

        VLLocalzLeafStarter->addWidget(CBzLeafVersion);

        LzLeafPort = new QLabel(lc__LocalzLeafStarter);
        LzLeafPort->setObjectName(QStringLiteral("LzLeafPort"));

        VLLocalzLeafStarter->addWidget(LzLeafPort);

        SBzLeafPort = new QSpinBox(lc__LocalzLeafStarter);
        SBzLeafPort->setObjectName(QStringLiteral("SBzLeafPort"));
        SBzLeafPort->setCursor(QCursor(Qt::ArrowCursor));
        SBzLeafPort->setStyleSheet(QStringLiteral(""));
        SBzLeafPort->setMinimum(7000);
        SBzLeafPort->setMaximum(65535);
        SBzLeafPort->setValue(7000);

        VLLocalzLeafStarter->addWidget(SBzLeafPort);

        LLocalzLeafSize = new QLabel(lc__LocalzLeafStarter);
        LLocalzLeafSize->setObjectName(QStringLiteral("LLocalzLeafSize"));

        VLLocalzLeafStarter->addWidget(LLocalzLeafSize);

        LELocalzLeafSize = new QLineEdit(lc__LocalzLeafStarter);
        LELocalzLeafSize->setObjectName(QStringLiteral("LELocalzLeafSize"));

        VLLocalzLeafStarter->addWidget(LELocalzLeafSize);

        line_Start = new QFrame(lc__LocalzLeafStarter);
        line_Start->setObjectName(QStringLiteral("line_Start"));
        line_Start->setFrameShape(QFrame::HLine);
        line_Start->setFrameShadow(QFrame::Sunken);

        VLLocalzLeafStarter->addWidget(line_Start);

        PBStartLocalzLeaf = new QPushButton(lc__LocalzLeafStarter);
        PBStartLocalzLeaf->setObjectName(QStringLiteral("PBStartLocalzLeaf"));

        VLLocalzLeafStarter->addWidget(PBStartLocalzLeaf);

        PBCancel = new QPushButton(lc__LocalzLeafStarter);
        PBCancel->setObjectName(QStringLiteral("PBCancel"));

        VLLocalzLeafStarter->addWidget(PBCancel);


        retranslateUi(lc__LocalzLeafStarter);
        QObject::connect(PBCancel, SIGNAL(clicked()), lc__LocalzLeafStarter, SLOT(deleteLater()));

        QMetaObject::connectSlotsByName(lc__LocalzLeafStarter);
    } // setupUi

    void retranslateUi(QWidget *lc__LocalzLeafStarter)
    {
        lc__LocalzLeafStarter->setWindowTitle(QApplication::translate("lc::LocalzLeafStarter", "Local z-Leaf Starter", nullptr));
#ifndef QT_NO_TOOLTIP
        LzLeafName->setToolTip(QApplication::translate("lc::LocalzLeafStarter", "This sets the name with which this local z-Leaf will connect to z-Tree.", nullptr));
#endif // QT_NO_TOOLTIP
        LzLeafName->setText(QApplication::translate("lc::LocalzLeafStarter", "Set the name of the to be started z-Leaf", nullptr));
#ifndef QT_NO_TOOLTIP
        LEzLeafName->setToolTip(QApplication::translate("lc::LocalzLeafStarter", "This sets the name with which this local z-Leaf will connect to z-Tree.", nullptr));
#endif // QT_NO_TOOLTIP
        LEzLeafName->setText(QApplication::translate("lc::LocalzLeafStarter", "local", nullptr));
#ifndef QT_NO_TOOLTIP
        LzLeafVersion->setToolTip(QApplication::translate("lc::LocalzLeafStarter", "Choose of which version the started z-Leaf shall be.", nullptr));
#endif // QT_NO_TOOLTIP
        LzLeafVersion->setText(QApplication::translate("lc::LocalzLeafStarter", "Choose the version of the to be started z-Leaf", nullptr));
#ifndef QT_NO_TOOLTIP
        CBzLeafVersion->setToolTip(QApplication::translate("lc::LocalzLeafStarter", "Choose of which version the started z-Leaf shall be.", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        LzLeafPort->setToolTip(QApplication::translate("lc::LocalzLeafStarter", "Set the port on which the z-Tree which shall be connected to listens.", nullptr));
#endif // QT_NO_TOOLTIP
        LzLeafPort->setText(QApplication::translate("lc::LocalzLeafStarter", "Choose the port the to be started z-Leaf shall listen on", nullptr));
#ifndef QT_NO_TOOLTIP
        SBzLeafPort->setToolTip(QApplication::translate("lc::LocalzLeafStarter", "Set the port on which the z-Tree which shall be connected to listens.", nullptr));
#endif // QT_NO_TOOLTIP
        LLocalzLeafSize->setText(QApplication::translate("lc::LocalzLeafStarter", "Choose the size the local z-Leaf shall have", nullptr));
#ifndef QT_NO_TOOLTIP
        PBStartLocalzLeaf->setToolTip(QApplication::translate("lc::LocalzLeafStarter", "Start a local z-Leaf with the set parameters.", nullptr));
#endif // QT_NO_TOOLTIP
        PBStartLocalzLeaf->setText(QApplication::translate("lc::LocalzLeafStarter", "Start local z-Leaf", nullptr));
#ifndef QT_NO_TOOLTIP
        PBCancel->setToolTip(QApplication::translate("lc::LocalzLeafStarter", "Cancel the start of a new local z-Leaf.", nullptr));
#endif // QT_NO_TOOLTIP
        PBCancel->setText(QApplication::translate("lc::LocalzLeafStarter", "Cancel", nullptr));
    } // retranslateUi

};

} // namespace lc

namespace lc {
namespace Ui {
    class LocalzLeafStarter: public Ui_LocalzLeafStarter {};
} // namespace Ui
} // namespace lc

#endif // UI_LOCALZLEAFSTARTER_H
