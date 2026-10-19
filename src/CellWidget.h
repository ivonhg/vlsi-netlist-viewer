// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"


namespace Netlist {

  class Cell;
  class NodeTerm;


  class CellWidget : public QWidget {
      Q_OBJECT;
      public :
        static int InstancesShapes; 
        static int InstanceShow;
        static int NodeShapes;


        CellWidget ( QWidget * parent = NULL );
        virtual ~ CellWidget ();
        void setCell ( Cell *);
        inline Cell * getCell () const ;
        int           xToScreenX(int) const;
        int           yToScreenY(int) const;
        int           screenXtoX(int) const; 
        int           screenYtoY(int) const;
        inline QRect boxToScreenRect ( const Box &) const ;
        inline QPoint pointToScreenPoint ( const Point &) const ;
        inline Box screenRectToBox ( const QRect &) const ;
        inline Point screenPointToPoint ( const QPoint &) const ;
        virtual QSize minimumSizeHint () const ;
        virtual void resizeEvent ( QResizeEvent *);
        protected :
        virtual void paintEvent ( QPaintEvent *);
        virtual void keyPressEvent ( QKeyEvent *);
        void        goRight();
        void        goUp();
        void        goDown();
        void        goLeft();
        void        query ( unsigned int flags , QPainter & painter );
      private :
        Cell * cell_;
        Box viewport_;
  };

  Cell*  CellWidget::getCell () const { return cell_; }
  QRect  CellWidget::boxToScreenRect( const Box& b) const 
  {return QRect( QPoint(xToScreenX(b.getX1()), yToScreenY(b.getY1())), QPoint(xToScreenX(b.getX2()), yToScreenY(b.getY2())) ).normalized();}

  QPoint CellWidget::pointToScreenPoint ( const Point& p) const 
  {return QPoint(xToScreenX(p.getX()), yToScreenY(p.getY()));}

  Box    CellWidget::screenRectToBox ( const QRect & rect) const{
    int x1 = screenXtoX( rect.left() );
    int y1 = screenYtoY( rect.top() );
    int x2 = screenXtoX( rect.right() );
    int y2 = screenYtoY( rect.bottom() );

    return Box( std::min(x1, x2), std::min(y1, y2), std::max(x1, x2), std::max(y1, y2) );
  }

  Point  CellWidget::screenPointToPoint ( const QPoint & qpoint) const
  { return Point( screenXtoX( qpoint.x() ), screenYtoY( qpoint.y() ) );}

}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H
