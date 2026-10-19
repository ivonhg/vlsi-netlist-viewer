#include <vector>
#include "Symbol.h"
#include "Shape.h"
#include "Term.h"
#include "TermShape.h"
#include "LineShape.h"
#include "BoxShape.h"
#include "Cell.h"
#include "Box.h"
#include "EllipseShape.h"
#include "ArcShape.h"

namespace Netlist {
    Symbol::Symbol(Cell* owner)
        :owner_(owner){}
    
    Symbol::~Symbol(){
        while(!shapes_.empty()){
            delete *shapes_.begin();
        }
    }

    Cell* Symbol::getCell() const{
        return owner_;
    }

    Box Symbol::getBoundingBox() const {
        Box b;
        for (size_t i =0; i<shapes_.size(); ++i){
             b.merge(shapes_[i]->getBoundingBox());
        }
        return b;    
    }

    Point Symbol::getTermPosition(Term* t) const{
        return t->getPosition();
    }

    TermShape* Symbol::getTermShape(const Term* t) const{
        for(size_t i=0; i<shapes_.size(); i++){
            TermShape * ts=dynamic_cast<TermShape*> (shapes_[i]);
            if( ts && (ts->getTerm() == t)){
                return ts;
            }
        }
    
        return nullptr;
    }

    void Symbol::add(Shape* s){
        if(s != nullptr){
            shapes_.push_back(s);
        }
    }

    void Symbol::remove(Shape* s) {
        if(s != nullptr){
            for(auto sh=shapes_.begin(); sh!=shapes_.end(); ++s){
                if(*sh==s){
                    shapes_.erase(sh);
                    return;
                }
            }
        }
    }

    void Symbol::toXml(std::ostream& stream) const{
        stream << indent++ <<"<symbol>\n";
        for (size_t i =0; i<shapes_.size(); ++i)
            shapes_[i]->toXml(stream);
        stream << --indent << "</symbol>\n";



    }

    Symbol* Symbol::fromXml(Cell* owner, xmlTextReaderPtr reader){
        if (!owner || !reader) return nullptr;

        const xmlChar* boxTag     = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
        const xmlChar* lineTag    = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
        const xmlChar* termTag    = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
        const xmlChar* ellipseTag = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
        const xmlChar* arcTag     = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
        while(true){
            int status = xmlTextReaderRead(reader);
            if (status != 1) {
                if (status != 0) {
                    std::cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << std::endl;
                }
                break;
            }

            switch ( xmlTextReaderNodeType(reader) ) {
                case XML_READER_TYPE_COMMENT:
                case XML_READER_TYPE_WHITESPACE:
                case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;

                case XML_READER_TYPE_END_ELEMENT: return owner->getSymbol();
            }

            const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

            if(boxTag == nodeName){
                BoxShape::fromXml(owner->getSymbol(),reader);
                continue;
            }
            if(lineTag==nodeName){
                LineShape::fromXml(owner->getSymbol(),reader);
                continue;
            }
            if(termTag==nodeName){
                TermShape::fromXml(owner->getSymbol(),reader);
                continue;
            }
            if(ellipseTag==nodeName){
                EllipseShape::fromXml(owner->getSymbol(),reader);
                continue;
            }
            if(arcTag==nodeName){
                ArcShape::fromXml(owner->getSymbol(),reader);
                continue;
            }
            else{
                std::cerr << "[ERROR] Cell::fromXml(): Unknown or misplaced tag <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
                break;
            }

        }

        return nullptr;
    }

}// namespace Netlist
