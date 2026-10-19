#include "LineShape.h"

namespace Netlist{
    LineShape::LineShape(Symbol * s, int x1 , int y1 , int x2 , int y2) 
    : Shape(s), x1_(x1), y1_(y1), x2_(x2), y2_(y2)
    {}
    
    LineShape::~LineShape(){}

    Box LineShape::getBoundingBox() const {
        return Box(std::min(x1_, x2_), std::min(y1_, y2_), std::max(x1_, x2_), std::max(y1_, y2_));
    }

    void LineShape::toXml(std::ostream& stream) const{
        stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\" />\n";
    }

    LineShape* LineShape::fromXml (Symbol* s, xmlTextReaderPtr reader){
        
        if(!s || !reader){return NULL;}

        std::string x1=xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        if(x1.empty()){return NULL;}

        std::string y1=xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        if(y1.empty()){return NULL;}

        std::string x2=xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        if(x2.empty()){return NULL;}

        std::string y2=xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
        if(x2.empty()){return NULL;}


        return new LineShape(s,stoi(x1),stoi(y1),stoi(x2),stoi(y2));
    }
}