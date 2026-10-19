#pragma once
#include <string>
#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "xmlUtil.h"
#include "Term.h"

namespace Netlist{
    class TermShape : public Shape {
        public :
            enum NameAlign { TopLeft=1, TopRight, BottomLeft, BottomRight };

            TermShape ( Symbol *, std::string name , int x , int y, NameAlign na );
            ~ TermShape ();
            Box getBoundingBox () const ;
            inline Term * getTerm () const ;
            inline int getX () const;
            inline int getY () const;
            inline void setX(int);
            inline void setY(int);
            inline void setPosition(int x, int y){
                x_=x;
                y_=y;
            }

            inline NameAlign getAlign() const { return align_; }
            void toXml(std::ostream& stream) const;
            static TermShape* fromXml( Symbol* s, xmlTextReaderPtr reader );
            static std::string align_to_str(NameAlign s);
            static TermShape::NameAlign str_to_align (std::string s);

        private :
            Term * term_ ;
            int x_ , y_ ;
            NameAlign align_;
    };
    inline Term * TermShape::getTerm () const{ return term_; }
    inline int TermShape::getX () const{ return x_; }
    inline int TermShape::getY () const{ return y_; }
    inline void TermShape::setX(int x){x_=x;}
    inline void TermShape::setY(int y){y_=y;}
    
}