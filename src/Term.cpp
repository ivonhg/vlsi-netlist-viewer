#include "Term.h"
#include "Instance.h"
#include "Cell.h"
#include "Node.h"
#include "string"
#include "xmlUtil.h"


namespace Netlist{


    Term::Term(Cell* c, const std::string& name, Direction d)
    :owner_(c), name_(name), direction_(d),type_(External), net_(nullptr), node_(this, Node::noid){
        if(c != nullptr){
            c->add(this);
        }
    };

    /*Term::Term(Instance* instance, const Term* modelTerm):
    owner_(instance),name_(modelTerm->name_),direction_(modelTerm->direction_),type_(Internal),net_(nullptr),node_(this)
    {   
        // instance->add(this); a enlever
    }*/

       Term::Term ( Instance * instance,  const Term * modelTerm )
        : owner_(instance), name_(modelTerm->name_), direction_(modelTerm->direction_), type_(Internal), net_(nullptr), node_(this) 
    {
        this->setPosition(instance->getMasterCell()->getSymbol()->getTermShape(modelTerm)->getX(),
                          instance->getMasterCell()->getSymbol()->getTermShape(modelTerm)->getY());
    }

   

    Term::~Term(){
        if(net_ != nullptr && node_.getId()!=Node::noid){
            net_->remove(&node_);
        }
        
        if(owner_ != nullptr){
            if(isExternal() ){
                getCell()->remove(this);
            }else{
                getInstance()->remove(this);
            }
        }
    }


    bool Term::isInternal()const{
        return type_==1;
    }

    bool Term::isExternal()const{
        return type_==2;
    }

    const std::string& Term::getName() const {
        return name_;
    }

    NodeTerm* Term::getNode(){
        return &node_;
    };

    Net* Term::getNet()const{
        return net_;
    };

    Cell* Term::getCell()const{
        if(isExternal()){
            return static_cast<Cell*>(owner_);
        }else{
            return nullptr;
        }
    }

    Cell* Term::getOwnerCell()const{
        if(isInternal()){
            return (static_cast<Instance*>(owner_))->getCell();
        }else{
            return static_cast<Cell*>(owner_);
        }
    }

    Instance* Term::getInstance()const{
        if(isInternal()){
            return static_cast<Instance*>(owner_);
        }else{
            return nullptr;
        }
    }

    Term::Direction Term::getDirection() const{
        return direction_;
    }

    void Term::setNet(Net* n){
        if(n==nullptr && node_.getId()!=Node::noid){
            net_->remove(&node_);
            return;
        }
        net_ = n;
        net_->add(&node_);
    }

    void Term::setNet(const std::string& name){
      

        if(isExternal()){
            net_=getCell()->getNet(name);
        }else{
            net_ = getOwnerCell()->getNet(name);
        }
        net_->add(&node_);
    }


    Point Term::getPosition() const{
        return node_.getPosition();
    }

    Term::Type Term::getType()const{
        return type_;
    }

    void Term::setPosition(const Point& p){
        node_.setPosition(p);
    }

    void Term::setPosition(int x, int y){
        node_.setPosition(x,y);
    }

    void Term::setDirection(Term::Direction d){
        direction_=d;
    }

    std::string Term::direct_str(){
        switch (direction_)
        {
        case 1:
            return "In";
        
        case 2:
            return "Out";
        
        case 3:
        return "Inout";

        case 4:
            return "Tristate";

        case 5:
            return "Transcv";

        default:
            return "Unknown";
        }
    }

    Term::Direction Term::direction_conv(const std::string d){
        if(d=="In"){return Term::In;}
        if(d=="Out"){return Term::Out;}
        if(d=="Inout"){return Term::Inout;}
        if(d=="Tristate"){return Term::Tristate;}
        if(d=="Trancv"){return Term::Transcv;}
        return Term::Unknown;
    }

    Term::Type Term::type_conv(std::string s){
        if(s=="Internal"){return Term::Internal;}
        else{
            return Term::External;
        }

    }

    std::string Term::direction_to_string(Term::Direction d){
        if(d==Term::In){return "In";}
        if(d==Term::Out){return"Out";}
        if(d==Term::Inout){return "Inout";}
        if(d==Term::Tristate){return "Tristate";}
        if(d==Term::Transcv){return "Transcv";}
        return "Unknown";
    }

    std::string Term::type_to_string(Direction d){
        return ((d==1) ? "Internal" : "External");
    }

    void Term::toXml(std::ostream& stream){
        stream << indent << "<term name=\"" << name_ << "\" direction=\"" << direction_to_string(direction_) << "\" x=\"" << node_.getPosition().getX() << "\" y=\""<< node_.getPosition().getY() << "\"/>\n";    }

    Term* Term::fromXml(Cell* c, xmlTextReaderPtr reader ){
        if( reader == nullptr || c==nullptr){
            return NULL;
        }

        std::string name= xmlCharToString( xmlTextReaderGetAttribute(reader,(const xmlChar*)"name"));
        if(name.empty()){return NULL;}

        std::string direction= xmlCharToString( xmlTextReaderGetAttribute(reader,(const xmlChar*)"direction"));
        if(direction.empty()){ return NULL;}
        
        std::string x = xmlCharToString( xmlTextReaderGetAttribute(reader,(const xmlChar*)"x"));
        std::string y = xmlCharToString( xmlTextReaderGetAttribute(reader,(const xmlChar*)"y"));
        if(x.empty() || y.empty()){ return NULL;}

        Term* t=new Term(c,name,direction_conv(direction));
        int xi=std::stoi(x);
        int yi=std::stoi(y);
        t->setPosition(xi,yi);

        return t;
    }
    

}