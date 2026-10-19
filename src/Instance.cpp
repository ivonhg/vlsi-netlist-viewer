#include "Instance.h"
#include "Cell.h"
#include <iostream>
#include "xmlUtil.h"


namespace Netlist {

    Instance::Instance (Cell* owner, Cell* model, const std::string& name):
    owner_(owner),masterCell_(model), name_(name), terms_(),position_(new Point())
    {
        //copie les terms du model
        for(Term* t: model->getTerms()){
            this->add(t);
        }
        //s'ajouter dans le vecteur d'instances d'owner_
        owner->add(this);
    };

    Instance::~Instance(){

        while(not terms_.empty()){
            delete *terms_.begin();
        }

        owner_->remove(this);
    };

    Cell* Instance::getMasterCell()const{
        return masterCell_;
    }

    Cell* Instance::getCell() const {
        return owner_;
    }

    const std::string& Instance::getName()const{
        return name_;
    }

    const std::vector<Term*>& Instance::getTerms(){
        return terms_;
    }

    Term* Instance::getTerm(const std::string& name )const{

        for(size_t i=0; i<terms_.size(); i++){
            if(terms_[i]->getName() == name){
                return terms_[i];
            }
        }

        return nullptr;
    }

    Point Instance::getPosition() const{
        return *(position_);
    }

    bool Instance::connect (const std::string& name, Net* n){
        Term* t = getTerm(name);
        if(t==nullptr){
            return false;
        }
        t->setNet(n);
        return true;

    }

    void Instance::add(Term* t){
        terms_.push_back(new Term(this,t));
    }

    void Instance::remove(Term* t){
        //enlève t du vecteur terms_ de l'instance
        for ( std::vector<Term*>::iterator term=terms_.begin() ; term != terms_.end() ; ++term ) {
            if (*term == t){
                terms_.erase(term);
                return;
            }
        }
    }

    /*void Instance::setPosition(const Point& p){
        int bx = position_->getX();
        int by = position_->getY();

        position_ = (Point*)&p;

        int dx = position_->getX()-bx;
        int dy = position_->getY()-by;

        Symbol* s = owner_->getSymbol();

        for(Term* t : terms_){
            TermShape* tShape = s->getTermShape(t);
            tShape->setX(tShape->getX()+dx);
            tShape->setY(tShape->getY()+dy);
        }
        
    }*/

        void Instance::setPosition ( const Point& p){
        int oldx = position_->getX();
        int oldy = position_->getY();

        position_ = (Point*)(&p);

        int distx = position_->getX()-oldx,
            disty = position_->getY()-oldy;


        for(Term* t : terms_){
           
            t->getPosition().translate(distx, disty);

        }
    }

    /*void Instance::setPosition(int x, int y){
        int bx = position_->getX();
        int by = position_->getY();

        position_->setX(x);
        position_->setY(y);

        int dx = position_->getX()-bx;
        int dy = position_->getY()-by;

        Symbol* s = owner_->getSymbol();

        for(Term* t : terms_){
            TermShape* tShape = s->getTermShape(t);
            if (tShape){
                tShape->setX(tShape->getX()+dx);
                tShape->setY(tShape->getY()+dy);
            }
        }


    }*/

        void Instance::setPosition ( int x , int y ){
        int oldx = position_->getX();
        int oldy = position_->getY();

        position_ = new Point(x, y);

        int distx = x-oldx, disty = y-oldy;


        for(Term* t : terms_){
            
            t->setPosition(t->getPosition().getX()+distx, t->getPosition().getY()+disty);    
           
        }
        
    }

    void Instance::toXml(std::ostream& stream){
        stream << indent <<"<instance name=\"" << name_ << "\" mastercell=\"" << masterCell_->getName() << "\"" << "x=\"" << position_->getX() <<"\" y=\""<<position_->getY()<< "\"/>\n";
    }

    Instance* Instance::fromXml(Cell* c, xmlTextReaderPtr reader){
        if(!c || !reader)
            return NULL;
        
        std::string name=xmlCharToString( xmlTextReaderGetAttribute(reader,(const xmlChar*)"name"));
        if(name.empty()){return NULL;}

        Cell* mastercell=Cell::find(xmlCharToString( xmlTextReaderGetAttribute(reader,(const xmlChar*)"mastercell")));
        if(!mastercell){return NULL;}

        std::string x=xmlCharToString( xmlTextReaderGetAttribute(reader,(const xmlChar*)"x"));
        std::string y=xmlCharToString( xmlTextReaderGetAttribute(reader,(const xmlChar*)"y"));
        if(x.empty() || y.empty()){return NULL;}

        Instance* res= new Instance(c,mastercell,name);
        res->setPosition(std::stoi(x),std::stoi(y));
        return res;
    }



}