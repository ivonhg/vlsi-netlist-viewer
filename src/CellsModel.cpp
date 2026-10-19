#include "CellsModel.h"
#include "CellViewer.h" // Obligatoire ici pour accéder aux méthodes de CellViewer

namespace Netlist {

    CellsModel::CellsModel ( QObject* parent )
        : QAbstractTableModel(parent)
        , cells_()
        , cellViewer_(nullptr)
    { }

    CellsModel::~CellsModel () { }



   void CellsModel::setCellViewer(CellViewer* cv) {
        if (not cv) return;
        
        cellViewer_ = cv;
        setCells(cellViewer_->getCell()->getAllCells());

        connect( cv, SIGNAL(cellLoaded()), this, SLOT(updateDatas()) );
    }

    void CellsModel::updateDatas() {
        if (not cellViewer_) return;
        
        if (cellViewer_->getCell()) {
            setCells(cellViewer_->getCell()->getAllCells());
        }
    }

    void CellsModel::setCells(std::vector<Cell*> cells) {

        emit layoutAboutToBeChanged();
        cells_ = cells;
        emit layoutChanged();
    }


    int CellsModel::rowCount(const QModelIndex& parent) const {
        return (int)cells_.size();
    }

    int CellsModel::columnCount(const QModelIndex& parent) const {
        return 1; 
    }

    QVariant CellsModel::data(const QModelIndex& index, int role) const {
        if (not index.isValid()) return QVariant();
        
        if (role == Qt::DisplayRole) {
            int row = index.row();
            if (row < (int)cells_.size()) {
                
                return QString(cells_[row]->getName().c_str());
            }
        }
        return QVariant();
    }

    QVariant CellsModel::headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation == Qt::Horizontal and role == Qt::DisplayRole) {
            if (section == 0) return "Cellules Chargées";
        }
        return QVariant();
    }

    Cell* CellsModel::getModel(int row) {
        if (row < 0 or row >= (int)cells_.size()) return nullptr;
        return cells_[row];
    }

} // Netlist namespace