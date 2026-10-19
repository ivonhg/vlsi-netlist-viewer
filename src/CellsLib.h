#pragma once
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include "CellsModel.h"

namespace Netlist {

    // Forward declaration : on dit que CellViewer existe sans inclure son .h ici
    class CellViewer;

    class CellsLib : public QWidget {
        Q_OBJECT;
    public:
        CellsLib(QWidget* parent = nullptr);
        void setCellViewer(CellViewer* cv);
        int getSelectedRow() const;
        inline CellsModel* getBaseModel() { return baseModel_; }

    public slots:
        void load();

    private:
        CellViewer* cellViewer_;
        CellsModel* baseModel_;
        QTableView* view_;
        QPushButton* load_;
    };
}