/********************************************************************************
** Form generated from reading UI file 'manualprintingsetup.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANUALPRINTINGSETUP_H
#define UI_MANUALPRINTINGSETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace lc {

class Ui_ManualPrintingSetup
{
public:
    QVBoxLayout *VLManualPrintingSetup;
    QPushButton *PBSelectFile;
    QLabel *LReceiptsHeader;
    QComboBox *CBReceiptsHeader;
    QCheckBox *ChBPrintAnonymousReceipts;
    QLabel *LReplaceParticipantNames;
    QComboBox *CBReplaceParticipantNames;
    QCheckBox *ChBReceiptsForLocalClients;
    QPushButton *PBPrint;
    QPushButton *PBCancel;

    void setupUi(QWidget *lc__ManualPrintingSetup)
    {
        if (lc__ManualPrintingSetup->objectName().isEmpty())
            lc__ManualPrintingSetup->setObjectName(QStringLiteral("lc__ManualPrintingSetup"));
        lc__ManualPrintingSetup->resize(256, 256);
        VLManualPrintingSetup = new QVBoxLayout(lc__ManualPrintingSetup);
        VLManualPrintingSetup->setObjectName(QStringLiteral("VLManualPrintingSetup"));
        PBSelectFile = new QPushButton(lc__ManualPrintingSetup);
        PBSelectFile->setObjectName(QStringLiteral("PBSelectFile"));
        PBSelectFile->setStyleSheet(QStringLiteral("background: cyan;"));

        VLManualPrintingSetup->addWidget(PBSelectFile);

        LReceiptsHeader = new QLabel(lc__ManualPrintingSetup);
        LReceiptsHeader->setObjectName(QStringLiteral("LReceiptsHeader"));

        VLManualPrintingSetup->addWidget(LReceiptsHeader);

        CBReceiptsHeader = new QComboBox(lc__ManualPrintingSetup);
        CBReceiptsHeader->setObjectName(QStringLiteral("CBReceiptsHeader"));
        CBReceiptsHeader->setStyleSheet(QStringLiteral("background: cyan;"));

        VLManualPrintingSetup->addWidget(CBReceiptsHeader);

        ChBPrintAnonymousReceipts = new QCheckBox(lc__ManualPrintingSetup);
        ChBPrintAnonymousReceipts->setObjectName(QStringLiteral("ChBPrintAnonymousReceipts"));
        ChBPrintAnonymousReceipts->setStyleSheet(QStringLiteral("background: cyan;"));

        VLManualPrintingSetup->addWidget(ChBPrintAnonymousReceipts);

        LReplaceParticipantNames = new QLabel(lc__ManualPrintingSetup);
        LReplaceParticipantNames->setObjectName(QStringLiteral("LReplaceParticipantNames"));
        LReplaceParticipantNames->setEnabled(false);

        VLManualPrintingSetup->addWidget(LReplaceParticipantNames);

        CBReplaceParticipantNames = new QComboBox(lc__ManualPrintingSetup);
        CBReplaceParticipantNames->addItem(QString());
        CBReplaceParticipantNames->addItem(QString());
        CBReplaceParticipantNames->addItem(QString());
        CBReplaceParticipantNames->addItem(QString());
        CBReplaceParticipantNames->setObjectName(QStringLiteral("CBReplaceParticipantNames"));
        CBReplaceParticipantNames->setEnabled(false);
        CBReplaceParticipantNames->setEditable(true);

        VLManualPrintingSetup->addWidget(CBReplaceParticipantNames);

        ChBReceiptsForLocalClients = new QCheckBox(lc__ManualPrintingSetup);
        ChBReceiptsForLocalClients->setObjectName(QStringLiteral("ChBReceiptsForLocalClients"));
        ChBReceiptsForLocalClients->setChecked(true);

        VLManualPrintingSetup->addWidget(ChBReceiptsForLocalClients);

        PBPrint = new QPushButton(lc__ManualPrintingSetup);
        PBPrint->setObjectName(QStringLiteral("PBPrint"));

        VLManualPrintingSetup->addWidget(PBPrint);

        PBCancel = new QPushButton(lc__ManualPrintingSetup);
        PBCancel->setObjectName(QStringLiteral("PBCancel"));

        VLManualPrintingSetup->addWidget(PBCancel);


        retranslateUi(lc__ManualPrintingSetup);
        QObject::connect(PBCancel, SIGNAL(clicked()), lc__ManualPrintingSetup, SLOT(deleteLater()));

        QMetaObject::connectSlotsByName(lc__ManualPrintingSetup);
    } // setupUi

    void retranslateUi(QWidget *lc__ManualPrintingSetup)
    {
        lc__ManualPrintingSetup->setWindowTitle(QApplication::translate("lc::ManualPrintingSetup", "Manual Printing Settings", nullptr));
        PBSelectFile->setText(QApplication::translate("lc::ManualPrintingSetup", "Select file to be printed", nullptr));
#ifndef QT_NO_TOOLTIP
        LReceiptsHeader->setToolTip(QApplication::translate("lc::ManualPrintingSetup", "Choose the LaTeX template which shall be used for receipts creation.", nullptr));
#endif // QT_NO_TOOLTIP
        LReceiptsHeader->setText(QApplication::translate("lc::ManualPrintingSetup", "Template for receipts:", nullptr));
#ifndef QT_NO_TOOLTIP
        CBReceiptsHeader->setToolTip(QApplication::translate("lc::ManualPrintingSetup", "Choose the LaTeX template which shall be used for receipts creation.", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        ChBPrintAnonymousReceipts->setToolTip(QApplication::translate("lc::ManualPrintingSetup", "Check this if you want the created receipts to be anonymous.", nullptr));
#endif // QT_NO_TOOLTIP
        ChBPrintAnonymousReceipts->setText(QApplication::translate("lc::ManualPrintingSetup", "Print anonymous receipts", nullptr));
#ifndef QT_NO_TOOLTIP
        LReplaceParticipantNames->setToolTip(QApplication::translate("lc::ManualPrintingSetup", "Choose a string which shall replace the participant name on the anonymous receipts.", nullptr));
#endif // QT_NO_TOOLTIP
        LReplaceParticipantNames->setText(QApplication::translate("lc::ManualPrintingSetup", "Substitute praticipants' names with:", nullptr));
        CBReplaceParticipantNames->setItemText(0, QApplication::translate("lc::ManualPrintingSetup", "\\hspace{5cm}", nullptr));
        CBReplaceParticipantNames->setItemText(1, QApplication::translate("lc::ManualPrintingSetup", "anonym", nullptr));
        CBReplaceParticipantNames->setItemText(2, QApplication::translate("lc::ManualPrintingSetup", "anonymous", nullptr));
        CBReplaceParticipantNames->setItemText(3, QApplication::translate("lc::ManualPrintingSetup", "nicht ausf\303\274llen", nullptr));

#ifndef QT_NO_TOOLTIP
        CBReplaceParticipantNames->setToolTip(QApplication::translate("lc::ManualPrintingSetup", "Choose a string which shall replace the participant name on the anonymous receipts.", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        ChBReceiptsForLocalClients->setToolTip(QApplication::translate("lc::ManualPrintingSetup", "This decides if receipts shall be printed for any z-Leaf instance running locally on the server.\n"
"\n"
"Warning: If this is disabled no receipts will be printed for ANY participant whose name contains the character string \"local\"!", nullptr));
#endif // QT_NO_TOOLTIP
        ChBReceiptsForLocalClients->setText(QApplication::translate("lc::ManualPrintingSetup", "Print receipts for local clients", nullptr));
#ifndef QT_NO_TOOLTIP
        PBPrint->setToolTip(QApplication::translate("lc::ManualPrintingSetup", "Starts a ReceiptsHandler instance to print the chosen file.", nullptr));
#endif // QT_NO_TOOLTIP
        PBPrint->setText(QApplication::translate("lc::ManualPrintingSetup", "Print", nullptr));
#ifndef QT_NO_TOOLTIP
        PBCancel->setToolTip(QApplication::translate("lc::ManualPrintingSetup", "Cancel the process without printing anything.", nullptr));
#endif // QT_NO_TOOLTIP
        PBCancel->setText(QApplication::translate("lc::ManualPrintingSetup", "Cancel", nullptr));
    } // retranslateUi

};

} // namespace lc

namespace lc {
namespace Ui {
    class ManualPrintingSetup: public Ui_ManualPrintingSetup {};
} // namespace Ui
} // namespace lc

#endif // UI_MANUALPRINTINGSETUP_H
