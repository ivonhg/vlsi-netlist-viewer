#pragma once
#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "xmlUtil.h"

namespace Netlist{
    class ArcShape : public Shape {
        public :
            ArcShape ( Symbol *, const Box &, int start, int span );
            ArcShape ( Symbol *, int x1 , int y1 , int x2 , int y2, int start, int span );
            ~ ArcShape ();
            Box getBoundingBox () const ;
            void toXml(std::ostream& stream) const;
            static ArcShape* fromXml(Symbol* s, xmlTextReaderPtr reader);
     
            inline int getStart() const { return start_; }
            inline int getSpan()  const { return span_; }
        private :
            Box box_ ;
            int start_, span_;
    };
}