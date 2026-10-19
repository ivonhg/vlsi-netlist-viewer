#include "BoxShape.h"


namespace Netlist{
    BoxShape::BoxShape( Symbol * owner, const Box & b) 
    : Shape(owner), box_(b)
    {}

    BoxShape::BoxShape ( Symbol * s, int x1 , int y1 , int x2 , int y2 )
    : Shape(s), box_(Box(std::min(x1, x2), std::min(y1, y2), std::max(x1, x2), std::max(y1, y2)))
    {}
    BoxShape::~BoxShape(){}

    Box BoxShape::getBoundingBox() const {
        return box_;
    }

    void BoxShape::toXml(std::ostream& stream) const{
        stream << indent << box_ << "\n";
    }

    BoxShape* BoxShape::fromXml (Symbol* s, xmlTextReaderPtr reader){
        if(!s || !reader){return NULL;}
    
        std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        if(x1.empty()){return NULL;}

        std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        if(y1.empty()){return NULL;}

        std::string x2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        if(x2.empty()){return NULL;}

        std::string y2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
        if(y2.empty()){return NULL;}

        return new BoxShape(s,stoi(x1),stoi(y1),stoi(x2),stoi(y2));
    }
}