#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QFrame>
#include "CellsLib.h"
#include "CellViewer.h"

namespace Netlist {

    CellsLib::CellsLib(QWidget* parent)
        : QWidget(parent)
        , cellViewer_(nullptr)
        , baseModel_(new CellsModel(this))
        , view_(new QTableView(this))
        , load_(new QPushButton(this))
    {
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose, false);
        setContextMenuPolicy(Qt::ActionsContextMenu);

        view_->setShowGrid(false);
        view_->setAlternatingRowColors(true);
        view_->setSelectionBehavior(QAbstractItemView::SelectRows);
        view_->setSelectionMode(QAbstractItemView::SingleSelection);
        view_->setSortingEnabled(true);
        view_->setModel(baseModel_);

        QHeaderView* horizontalHeader = view_->horizontalHeader();
        horizontalHeader->setDefaultAlignment(Qt::AlignHCenter);
        horizontalHeader->setMinimumSectionSize(300);
        horizontalHeader->setStretchLastSection(true);
        view_->verticalHeader()->setVisible(false);

        load_->setText(" Load ");

        QHBoxLayout* hLayout = new QHBoxLayout();
        hLayout->addStretch();
        hLayout->addWidget(load_);
        hLayout->addStretch();

        QFrame* separator = new QFrame();
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);

        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->addWidget(view_);
        vLayout->addWidget(separator);
        vLayout->addLayout(hLayout);
        setLayout(vLayout);

        connect(load_, SIGNAL(clicked()), this, SLOT(load()));
    }

    void CellsLib::setCellViewer(CellViewer* cv) {
        cellViewer_ = cv;
        baseModel_->setCellViewer(cellViewer_);
    }

    int CellsLib::getSelectedRow() const {
        QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
        if (selecteds.empty()) return -1;
        return selecteds.first().row();
    }

    void CellsLib::load() {
        int selectedRow = getSelectedRow();
        if (selectedRow < 0) return;

        Cell* selectedCell = baseModel_->getModel(selectedRow);
        if (selectedCell && cellViewer_) {
            //CellWidget::InstancesShapes = 1;
            //CellWidget::InstanceShow = 0;
            cellViewer_->setCell(selectedCell);
        }
    }


} // Netlist namespace