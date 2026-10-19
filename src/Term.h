#pragma once
#include <string>
#include <vector>
#include "Indentation.h"
#include "Point.h"
#include "Node.h"
#include "xmlUtil.h"
#include <libxml/xmlreader.h>



namespace Netlist{

    
    class Cell;
    class NodeTerm;
    class Instance;
    class Net;
    class Point;

    class Term{

        public:
            enum Type {Internal=1, External=2};
            enum Direction {In=1, Out=2, Inout=3, Tristate=4, Transcv=5, Unknown=6};

            Term ( Cell* c, const std::string& name, Direction);
            Term ( Instance*, const Term* modelTerm);
            ~Term ();
            bool    isInternal() const;
            bool    isExternal() const;
            const std::string&  getName() const;
            NodeTerm*   getNode();
            Net*    getNet() const;
            Cell*   getCell() const;
            Cell*   getOwnerCell () const;
            Instance* getInstance() const;
            Direction getDirection() const;
            Point     getPosition() const;
            Type      getType() const;
            void        setNet(Net*);
            void        setNet(const std::string&);//?
            void        setPosition(const Point&);
            void        setPosition(int x, int y);
            void        setDirection(Direction);
            void        toXml(std::ostream&); //
            std::string direct_str(); 
            static Term*       fromXml(Cell*, xmlTextReaderPtr);
            static Direction   direction_conv(const std::string); //convertie string en direction si correcte sinon unknown
            static std::string direction_to_string(Term::Direction d);
            static std::string type_to_string(Direction d);
            static Type        type_conv(const std::string); //convertie string en type si correcte sinon unknown
        
        private: 
            void*           owner_;
            std::string     name_;
            Direction       direction_;
            Type            type_;
            Net*            net_;
            NodeTerm        node_;
    };
}

