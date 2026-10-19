#include "CellViewer.h"
#include <QMenuBar>
#include <QAction>
#include "OpenCellDialog.h"

namespace Netlist{
  CellViewer :: CellViewer ( QWidget * parent )
  : QMainWindow ( parent )
  , cellWidget_ ( NULL )
  , saveCellDialog_ ( NULL )
  {
    cellWidget_ = new CellWidget ();
    saveCellDialog_ = new SaveCellDialog ( this );
    setCentralWidget ( cellWidget_ );
    QMenu * fileMenu = menuBar ()->addMenu ( "&File" );

    cellsLib_=new CellsLib();
    cellsLib_->setCellViewer(this);
    instancesWidget_ = new InstancesWidget();
    instancesWidget_->setCellViewer(this);

    //boutton save
    QAction * actionSave = new QAction ( " &Save As" , this );
    actionSave->setStatusTip ( "Save to disk (rename) the Cell" );
    actionSave->setShortcut ( QKeySequence ( "CTRL+S" ) );
    actionSave->setVisible ( true );
    fileMenu->addAction ( actionSave );
    connect ( actionSave , SIGNAL ( triggered ()) , this , SLOT ( saveCell ()) );


    //boutton open
    QAction* actionOpen= new QAction(" &Open",this);
    actionOpen->setStatusTip("open from disk");
    actionOpen->setShortcut(QKeySequence( "CRTL+O") );
    actionOpen->setVisible(true);
    fileMenu->addAction(actionOpen);
    connect(actionOpen,SIGNAL(triggered()),this, SLOT(openCell()));

    //boutton cell
    QAction* actionCell= new QAction(" &Cells",this);
    actionCell->setStatusTip("show cells");
    actionCell->setShortcut(QKeySequence( "CRTL+C") );
    actionCell->setVisible(true);
    fileMenu->addAction(actionCell);
    connect(actionCell,SIGNAL(triggered()),this, SLOT(showCellsLib()));

    //boutton instance
    QAction* actionIns= new QAction(" &Instances",this);
    actionIns->setStatusTip("show instances");
    actionIns->setShortcut(QKeySequence( "CRTL+I") );
    actionIns->setVisible(true);
    fileMenu->addAction(actionIns);
    connect(actionIns,SIGNAL(triggered()),this, SLOT(showInstancesWidget()));

    //boutton quitter
    QAction* actionQuit = new QAction ( "&Quit" , this );
    actionQuit->setStatusTip ( "Exit the Netlist Viewer" );
    actionQuit->setShortcut ( QKeySequence ( "CTRL+Q" ) );
    actionQuit->setVisible ( true );
    fileMenu->addAction ( actionQuit );
    connect ( actionQuit , SIGNAL ( triggered ()) , this , SLOT ( close ()) );
  }

  CellViewer::~CellViewer() {}

  void CellViewer :: saveCell ()
  {
    Cell * cell = getCell ();
    if ( cell == NULL ) return ;
    QString cellName = cell->getName (). c_str ();
    if ( saveCellDialog_->run ( cellName )) {
      cell->setName ( cellName . toStdString () );
      cell->save ();
    }
  }

  /*Fonction qui va changer le Cell de CellWidget*/
  void CellViewer::setCell(Cell* c) {cellWidget_->setCell(c);}
  /*Fonction qui va récupérer la Cell depui cellWidget*/
  Cell* CellViewer::getCell() const {return cellWidget_->getCell();}


  //-----------------------------------------------------------------------------
 void CellViewer::openCell() {
    OpenCellDialog* dialog=new OpenCellDialog(this);
    QString n= OpenCellDialog::run(dialog);
    //std::cout<<"1\n";
    

    if (n!=NULL) {
      //charge la bonne cell
      //std::cout<<"2\n";

        std::string name=n.toStdString();
        Cell* cell = Cell::find(name);
        //std::cout<<"3\n";
        
        if (not cell) {
            // Si pas en mémoire, on tente de charger le fichier XML
            if((cell = Cell::load(name))){
              //std::cout<<"4\n";
              this->cellWidget_->setCell(cell);
              emit cellLoaded();
            }else{
              // Petit message d'erreur si la cellule est introuvable
              std::cerr << "[ERROR] Cell not found: " << name << std::endl;
              return;
            }
            
        }else{
          this->cellWidget_->setCell(cell);
          //std::cout<<"5\n";
        }

    }
    //std::cout<<"fin\n";
 
}

  //TME 9+
  void CellViewer::showCellsLib(){
    if(cellsLib_)
      cellsLib_->show();


  }

  void CellViewer::showInstancesWidget(){
    instancesWidget_->setCell(this->cellWidget_->getCell());
    instancesWidget_->show();
  }
}