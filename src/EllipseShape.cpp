#include "EllipseShape.h"

namespace Netlist{
    EllipseShape::EllipseShape( Symbol * s, const Box & b) 
    : Shape(s), box_(b)
    {}

    EllipseShape::EllipseShape ( Symbol * s, int x1 , int y1 , int x2 , int y2 )
    : Shape(s), box_(Box(std::min(x1, x2), std::min(y1, y2), std::max(x1, x2), std::max(y1, y2)))
    {}
    EllipseShape::~EllipseShape(){}

    Box EllipseShape::getBoundingBox() const {
        return box_;
    }

    void EllipseShape::toXml(std::ostream& stream) const{
        stream << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" />\n";
    }

    EllipseShape* EllipseShape::fromXml (Symbol* s, xmlTextReaderPtr reader){
   
        if(!s || !reader) return NULL;
        
        std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        if(x1.empty()){return NULL;}
        std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        if(y1.empty()){return NULL;}
        std::string x2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        if(x2.empty()){return NULL;}
        std::string y2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
        if(y2.empty()){return NULL;}


        return new EllipseShape(s, stoi(x1), stoi(y1), stoi(x2), stoi(y2));
    }
}