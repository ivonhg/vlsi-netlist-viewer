#pragma once
#include <QAbstractTableModel>
#include <vector>
#include "Cell.h"

namespace Netlist {


    class CellViewer;

    class CellsModel : public QAbstractTableModel {
        Q_OBJECT;

    public:
        // Constructeur et Destructeur
        CellsModel(QObject* parent = NULL);
        ~CellsModel();

        // Gestion des données
        void setCells(std::vector<Cell*> cells);
        Cell* getModel(int row);
        inline std::vector<Cell*> getCells();


        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        // Lien avec l'interface
        void setCellViewer(CellViewer* viewer);

    public slots:
        // Pour rafraîchir l'affichage si la liste change
        void updateDatas();

    private:
        std::vector<Cell*> cells_;      
        CellViewer* cellViewer_; 
    };

    inline std::vector<Cell*> CellsModel::getCells() { return cells_; }

} // Netlist namespace