#pragma once
#include "CellViewer.h"


namespace Netlist{
    class CellsLib : public QWidget {
        Q_OBJECT;
    class CellViewer;
    public:
                        CellsLib       ( QWidget* parent=NULL );
            void        setCellViewer  ( CellViewer* );
            int         getSelectedRow () const;
        // inline CellsModel* getBaseModel   ();
    public slots:
            void        load           ();
    private:
        CellViewer*  cellViewer_;
        // CellsModel*  baseModel_;
        QTableView*  view_;
        QPushButton* load_;
    };
}

