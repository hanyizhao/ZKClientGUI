#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ZKClientGUI.h"
#include "EditConnectionsModel.h"

class ZKClientGUI : public QMainWindow
{
    Q_OBJECT

public:
    ZKClientGUI(QWidget* parent = Q_NULLPTR);

private slots:

    void OnConnectButtonClicked(bool b = false);

private:
    Ui::ZKClientGUIClass ui;

    EditConnectionsModel* m_pConnectionsModel = nullptr;
};
