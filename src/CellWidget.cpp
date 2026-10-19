// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"
#include  "BoxShape.h"
#include  "LineShape.h"
#include  "ArcShape.h"
#include  "TermShape.h"
#include  "EllipseShape.h"

#define SCREENSIZE 500

namespace Netlist {

  using namespace std;

  int CellWidget::InstancesShapes = 1;

  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }


  CellWidget :: CellWidget ( QWidget * parent )
    : QWidget ( parent )
    , cell_ ( NULL )
    , viewport_ ( Box (0 ,0 ,500 ,500))
    {
    setAttribute ( Qt :: WA_OpaquePaintEvent );
    setAttribute ( Qt :: WA_NoSystemBackground );
    setSizePolicy ( QSizePolicy :: Expanding // X direction .
    , QSizePolicy :: Expanding ); // Y direction .
    setFocusPolicy ( Qt :: StrongFocus );
    setMouseTracking ( true );

  }


  CellWidget::~CellWidget ()
  { }


  void  CellWidget::setCell ( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }


  QSize CellWidget :: minimumSizeHint () const
  { return QSize (500 ,500); }


  void CellWidget :: resizeEvent ( QResizeEvent * event ) {
    const QSize & size = event -> size ();
    viewport_ . setX2 ( viewport_ . getX1 () + size . width () );
    viewport_ . setY1 ( viewport_ . getY2 () - size . height () );
    cerr << " CellWidget :: resizeEvent () ␣ viewport_ : " << viewport_ << endl;
  }


  int CellWidget::xToScreenX(int x) const {
    return x - viewport_.getX1(); 
  }

  int CellWidget::yToScreenY(int y) const{
    return viewport_.getY2() - y;
  }

  int CellWidget::screenXtoX(int x) const {
    return x + viewport_.getX1();
  }

  int CellWidget::screenYtoY(int y) const{
    return viewport_.getY2() - y;
  }

  void CellWidget :: paintEvent ( QPaintEvent * event ) {
    QPainter painter ( this );
    painter.setBackground ( QBrush ( Qt :: black ) );
    painter.eraseRect ( QRect ( QPoint (0 ,0) , size () ) );
    painter.setPen ( QPen ( Qt :: darkGreen , 3 ) );
    QRect rect = boxToScreenRect ( viewport_ );
    painter.drawRect ( rect );

    
    painter . drawRect ( rect );
    if (InstancesShapes)
      query( InstancesShapes, painter );
  }

  void CellWidget :: goRight () {
    viewport_.translate ( Point (20 ,0) );
    repaint ();
  }


  void CellWidget :: goUp () {
    viewport_ . translate ( Point (0 ,20) );
    repaint ();
  }

  void CellWidget :: goDown () {
    viewport_ . translate ( Point (0 , -20) );
    repaint ();
  }

  void CellWidget :: goLeft () {
    viewport_.translate ( Point (-20 ,0) );
    repaint ();
  }


  void CellWidget :: keyPressEvent ( QKeyEvent * event ) {
    event -> ignore ();
    if ( event -> modifiers () & ( Qt :: ControlModifier | Qt :: ShiftModifier ))
    return ;
    switch ( event -> key ()) {
    case Qt::Key_Up:goUp (); break ;
    case Qt::Key_Down:goDown (); break ;
    case Qt::Key_Left:goLeft (); break ;
    case Qt::Key_Right:goRight (); break ;
    default : return ;
    }
    event -> accept ();
  }

void CellWidget::query(unsigned int flags, QPainter& painter) {
    if ((not cell_) or (not flags)) return;

    // --- 1. NETS ET POINTS
    const vector<Net*>& nets = cell_->getNets();
    for (size_t i = 0; i < nets.size(); ++i) {
        painter.setPen(QPen(Qt::cyan, 1));
        const vector<Line*>& lines = nets[i]->getLines();
        for (size_t j = 0; j < lines.size(); ++j) {
            painter.drawLine(xToScreenX(lines[j]->getSourcePosition().getX()),
                             yToScreenY(lines[j]->getSourcePosition().getY()),
                             xToScreenX(lines[j]->getTargetPosition().getX()),
                             yToScreenY(lines[j]->getTargetPosition().getY()));
        }

        painter.setBrush(QBrush(Qt::cyan));
        const vector<Node*>& nodes = nets[i]->getNodes();
        for (size_t k = 0; k < nodes.size(); ++k) {
            if (nodes[k] && nodes[k]->getLines().size() >= 3) {
                painter.drawEllipse(xToScreenX(nodes[k]->getPosition().getX()) - 4, 
                                    yToScreenY(nodes[k]->getPosition().getY()) - 4, 8, 8);
            }
        }
        painter.setBrush(Qt::NoBrush);
    }

    // --- 2. INSTANCES 
    const vector<Instance*>& instances = cell_->getInstances();
    for (size_t i = 0; i < instances.size(); ++i) {
        Point instPos = instances[i]->getPosition();
        const Symbol* symbol = instances[i]->getMasterCell()->getSymbol();
        if (not symbol || !(flags & InstancesShapes)) continue;

        const vector<Shape*>& shapes = symbol->getShapes();
        for (size_t j = 0; j < shapes.size(); ++j) {
            painter.setPen(QPen(Qt::darkGreen, 3));
            
            if (BoxShape* box = dynamic_cast<BoxShape*>(shapes[j])) {
                painter.drawRect(boxToScreenRect(box->getBoundingBox().translate(instPos)));
            } else if (EllipseShape* ell = dynamic_cast<EllipseShape*>(shapes[j])) {
                painter.drawEllipse(boxToScreenRect(ell->getBoundingBox().translate(instPos)));
            } else if (ArcShape* arc = dynamic_cast<ArcShape*>(shapes[j])) {
                painter.drawArc(boxToScreenRect(arc->getBoundingBox().translate(instPos)), arc->getStart()*16, arc->getSpan()*16);
            } else if (LineShape* line = dynamic_cast<LineShape*>(shapes[j])) {
                Box b = line->getBoundingBox();
                painter.drawLine(xToScreenX(b.getX1() + instPos.getX()), yToScreenY(b.getY1() + instPos.getY()),
                                 xToScreenX(b.getX2() + instPos.getX()), yToScreenY(b.getY2() + instPos.getY()));
            }
            else if (TermShape* ts = dynamic_cast<TermShape*>(shapes[j])) {
                int tx = xToScreenX(ts->getX() + instPos.getX());
                int ty = yToScreenY(ts->getY() + instPos.getY());

                painter.setPen(Qt::NoPen);
                painter.setBrush(QBrush(Qt::red));
                painter.drawRect(tx - 3, ty - 3, 6, 6);

                painter.setPen(Qt::white);
                QString name = QString::fromStdString(ts->getTerm()->getName());
                switch (ts->getAlign()) {
                    case TermShape::TopLeft:     painter.drawText(tx - 25, ty - 10, name); break;
                    case TermShape::TopRight:    painter.drawText(tx + 10, ty - 10, name); break;
                    case TermShape::BottomLeft:  painter.drawText(tx - 25, ty + 20, name); break;
                    case TermShape::BottomRight: painter.drawText(tx + 10, ty + 20, name); break;
                }
            }
        }
        painter.setPen(Qt::white);
        painter.drawText(xToScreenX(instPos.getX()), yToScreenY(instPos.getY()) + 35, 
                         QString::fromStdString(instances[i]->getName()));
    }

    //  TERMINAUX EXTERNES
    const vector<Term*>& terms = cell_->getTerms();
    for (size_t i = 0; i < terms.size(); ++i) {
        int px = xToScreenX(terms[i]->getPosition().getX());
        int py = yToScreenY(terms[i]->getPosition().getY());
        
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::red));

        QPolygon poly;
       if (terms[i]->getDirection() == Term::In) {
     
            poly << QPoint(px - 15, py - 10) << QPoint(px, py - 10) 
                 << QPoint(px + 10, py)      << QPoint(px, py + 10) 
                 << QPoint(px - 15, py + 10);
            
            painter.setPen(Qt::white);
            painter.drawText(px - 25, py - 15, QString::fromStdString(terms[i]->getName()));
        } else {
            
 
            poly << QPoint(px,      py - 10) 
                 << QPoint(px - 15, py - 10) 
                 << QPoint(px - 25, py)      // La pointe est ici à gauche
                 << QPoint(px - 15, py + 10) 
                 << QPoint(px,      py + 10);
            
            painter.setPen(Qt::white);
            painter.drawText(px + 5, py - 15, QString::fromStdString(terms[i]->getName()));
        }
        painter.drawPolygon(poly);
    }
}
}  // Netlist namespace