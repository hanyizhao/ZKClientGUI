#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ZKClientGUI.h"
#include "EditConnectionsModel.h"

class ZKClientGUI : public QMainWindow
{
    Q_OBJECT

public:
    ZKClientGUI(QWidget* parent, const int nLanguageIndex);

private slots:
    void OnLanguageChanged(int nIndex);
    void OnConnectButtonClicked(bool b = false);

private:
    Ui::ZKClientGUIClass ui;

    EditConnectionsModel* m_pConnectionsModel = nullptr;
    int m_nNowLanguageIndex = 0;
};
