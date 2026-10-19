#pragma once
#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include "SaveCellDialog.h"
#include "InstancesModel.h"

namespace Netlist{

    class CellViewer;

    class InstancesWidget : public QWidget {
        Q_OBJECT;
    public:
                    InstancesWidget ( QWidget* parent=NULL );
                void  setCellViewer   ( CellViewer* );
                int   getSelectedRow  () const;
        inline  void  setCell         ( Cell* );
    public slots:
                void  load            ();
    private:
                CellViewer*     cellViewer_;
                InstancesModel* baseModel_; //TME 9+ 
                QTableView*     view_;
                QPushButton*    load_;
    };

        // InstancesWidget.h
    void InstancesWidget::setCell ( Cell* cell ) { baseModel_->setCell(cell); }
}