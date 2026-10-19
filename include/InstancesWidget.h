#pragma once
#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include "SaveCellDialog.h"


namespace Netlist{
    class InstancesWidget : public QWidget {
        Q_OBJECT;
    class CellViewer;
    public:
                    InstancesWidget ( QWidget* parent=NULL );
                void  setCellViewer   ( CellViewer* );
                int   getSelectedRow  () const;
        inline  void  setCell         ( Cell* );
    public slots:
                void  load            ();
    private:
                CellViewer*     cellViewer_;
                // InstancesModel* baseModel_; //TME 9+ 
                QTableView*     view_;
                QPushButton*    load_;
    };
}
