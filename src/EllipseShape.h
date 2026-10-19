#pragma once
#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "xmlUtil.h"

namespace Netlist{
    class EllipseShape : public Shape {
        public :
            EllipseShape ( Symbol *, const Box & );
            EllipseShape ( Symbol *, int x1 , int y1 , int x2 , int y2 );
            ~ EllipseShape ();
            Box getBoundingBox () const ;
            void toXml(std::ostream& stream) const;
            static EllipseShape* fromXml(Symbol* s, xmlTextReaderPtr reader);
        private :
            Box box_ ;
    };
}