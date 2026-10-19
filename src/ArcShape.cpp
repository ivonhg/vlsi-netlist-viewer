#include "ArcShape.h"

namespace Netlist{
            
    
    ArcShape::ArcShape ( Symbol * s, const Box& b, int start, int span ):
    Shape(s), box_(b),start_(start),span_(span)
    {}


    ArcShape::ArcShape ( Symbol * s, int x1 , int y1 , int x2 , int y2, int start, int span )
        :Shape(s),box_(Box(x1, y1, x2, y2)), start_(start),span_(span)
    {
       
    }
            
    ArcShape::~ ArcShape (){}

    Box ArcShape::getBoundingBox () const{
        return box_;
    }
    
    void ArcShape::toXml(std::ostream& stream) const{
        stream << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" start=\"" << start_ << "\" span=\"" << span_ << "\" />\n";
    }
    
    
    ArcShape* ArcShape::fromXml(Symbol* s, xmlTextReaderPtr reader){
        if(!s || !reader){
            return NULL;
        }

        std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        std::string x2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        std::string y2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
        if(x1.empty() || x2.empty() || y1.empty() || y2.empty()){return NULL;}

        std::string start = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"start" ) );
        std::string span = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"span" ) );
        if(start.empty() || span.empty()){
            return NULL;
        }

        return new ArcShape(s,stoi(x1),stoi(y1),stoi(x2),stoi(y2),stoi(start),stoi(span));

    }



}