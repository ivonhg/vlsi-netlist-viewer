#include "TermShape.h"
#include "Cell.h"

namespace Netlist{
    TermShape :: TermShape ( Symbol * owner , std::string name , int x1 , int y1, NameAlign a)
    : Shape ( owner ) , term_ ( NULL ), x_ ( x1 ), y_ ( y1 ),align_(a)
    {
        Cell * cell = owner->getCell ();
        term_ = cell -> getTerm ( name );
    }

    TermShape ::~ TermShape () { }

    Box TermShape :: getBoundingBox () const
    { return Box ( x_ , y_ , x_ , y_ ); }

    std::string TermShape::align_to_str(TermShape::NameAlign s) {
        switch (s) {
            case TopLeft:       return "top_left";
            case TopRight:      return "top_right";
            case BottomLeft:    return "bottom_left";
            case BottomRight:   return "bottom_right";
            default:            return "Unknown";
        }
    }

    TermShape::NameAlign TermShape::str_to_align(std::string s){
        if(s == "top_left")
            return NameAlign::TopLeft;
        if(s == "top_right")
            return TopRight;
        if(s == "bottom_left")
            return BottomLeft;
        else{
            return BottomRight;
        }
    }

    void TermShape::toXml(std::ostream& stream) const{
        stream << indent << "<term name=\"" << term_->getName() << "\" x1=\"" << x_ << "\" y1=\"" << y_ << "\" align=\"" << align_to_str(align_) << "\" />\n";
    }

    TermShape* TermShape::fromXml( Symbol* s, xmlTextReaderPtr reader ){
        if(!s || !reader){return NULL;}

        std::string name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        if(x1.empty() || y1.empty() || name.empty()){return NULL;}

        std::string align = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align" ) );
        if(align.empty() || (align != "top_right" && align != "top_left" && align != "bottom_right" && align != "bottom_left")){
            return NULL;
        }
        return new TermShape(s,name,stoi(x1),stoi(y1),str_to_align(align));

    }
}