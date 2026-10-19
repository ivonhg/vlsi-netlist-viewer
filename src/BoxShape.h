#pragma once
#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "xmlUtil.h"

namespace Netlist{
    class BoxShape : public Shape {
        public :
            BoxShape ( Symbol *, const Box & );
            BoxShape(Symbol* , int, int, int, int);
            ~ BoxShape ();
            virtual Box getBoundingBox () const ;
            void toXml(std::ostream& stream) const;
            static BoxShape* fromXml(Symbol* s, xmlTextReaderPtr reader);
        private :
        
            Box box_ ;
    };
}