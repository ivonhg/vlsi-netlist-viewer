#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "xmlUtil.h"

namespace Netlist{
    class LineShape : public Shape {
        public :
            LineShape ( Symbol *, int x1 , int y1 , int x2 , int y2 );
            ~ LineShape ();
            Box getBoundingBox () const ;
            void toXml(std::ostream& stream) const;
            static LineShape* fromXml( Symbol*, xmlTextReaderPtr );
        private :
            int x1_ , y1_ , x2_ , y2_ ;
    };
}