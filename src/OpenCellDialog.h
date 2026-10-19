#include <QDialog>
#include <QDialog>
#include "Cell.h"

namespace Netlist{
    class OpenCellDialog : public QDialog {
        Q_OBJECT ;
        public :
                            OpenCellDialog ( QWidget * parent = NULL );
            static  QString&    run ( OpenCellDialog* );
            const   QString getCellName () const ;
            static void            setCellName ( const QString & );
        protected :
            QLineEdit * lineEdit_ ;
    };
}
