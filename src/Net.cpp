#include "Net.h"
#include "Cell.h"
#include "Node.h"
#include <vector>



namespace Netlist{

    Net::Net(Cell* o, const std::string& n,Term::Type t):owner_(o),name_(n),type_(t),nodes_(),lines_()
    {
        o->add(this);
    };

    Net::~Net(){

        for(Node* n : nodes_){
            if(NodeTerm* nt = dynamic_cast<NodeTerm*>(n))
                nt->getTerm()->setNet(nullptr);
        }
        owner_->remove(this);

        //ancienne version
        /*for (Node* node : nodes_) {
            if (node && node->getTerm()) {
                node->getTerm()->setNet(nullptr);
            }
        }
        std::vector<Node*> toDelete = nodes_;
        nodes_.clear();

        owner_->remove(this); */
    
    

    }

    Cell* Net::getCell() const{
        return owner_;
    }

    const std::string& Net::getName () const{
        return name_;
    }

    Term::Type Net::getType() const{
        return type_;
    }

    Node* Net::getNode(size_t id) const{
        return nodes_[id];
    }

    const std::vector<Node*>& Net::getNodes() const{
        return nodes_;
    }

    size_t Net::getFreeNodeId () const{
        for(size_t i = 0; nodes_.begin()+i != nodes_.end(); i++){
            if(nodes_[i] == NULL){
                return i;
            }
        }
        return nodes_.size();

    }

    void Net::add(Node* n) {

        size_t id = n->getId();
        if (id != Node::noid){
            if(id >= nodes_.size()){
                nodes_.resize(id+1, nullptr);
                nodes_[id] = n;
                return;
            } else if (nodes_[id]==nullptr){ nodes_[id] = n; return;} 
            else {
                nodes_[id] = n;
                return;
            }
        }
        if (int ind = getFreeNodeId() < nodes_.size()){
            nodes_[ind] = n;
            n->setId(ind);
        }
        else{
            n->setId(nodes_.size());
            nodes_.push_back(n);
        }
        
    }

    bool Net::remove(Node* n){
        int i=n->getId();
        if(nodes_[i]==n){
            nodes_[i]=nullptr;
            return true;
        }
        return false;
    }

    void Net::toXml(std::ostream& stream){
        stream << indent++ << "<net name=\"" << name_ << "\" type =\""; 
        //print le type
        if(type_ == 1){stream<<"Internal";} 
        
        else{stream<<"External";}
        
        stream << "\">\n";
        //stream <<"avant boucle node\n";
        //print les node
        for(Node* no : nodes_){
            //stream<<"dans boucle toxml net\n";
            if(no!=nullptr){
                //stream<<"in\n";
                no->toXml(stream);

            }
        }

        for(Line* l: lines_){
            if(l!=nullptr){
                l->toXml(stream);
            }
        }
        stream << --indent << "</net>\n";
    }

    Net* Net::fromXml(Cell* c, xmlTextReaderPtr reader){

        if(!c || !reader)
            return NULL;

        std::string name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        if(name.empty()){return NULL;}

        std::string type = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ) );
        if(type.empty()){return NULL;}

        Term::Type t_type=Term::type_conv(type);
        Net* net=new Net(c,name,t_type);

        //construction des nodes

        while( (xmlTextReaderNodeType(reader) != XML_READER_TYPE_END_ELEMENT)){
            int status=xmlTextReaderRead(reader);

            if(status !=1){
                if(status!=0)
                    std::cerr<<" ERROR net::fromXml(): Unexpected termination of the XML parser." << std::endl;
                break;
            }

            //passe les inutiles pour la construction des nodes
            switch ( xmlTextReaderNodeType(reader) ) {
                case XML_READER_TYPE_COMMENT:
                case XML_READER_TYPE_WHITESPACE:
                case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }
            //construit les nodes
            Node::fromXml(net,reader);
            //construit les lines
            Line::fromXml(net,reader);

        }

        return net;

    }


    bool  Net::remove ( Line* line ){
        if (line) {
            for ( std::vector<Line*>::iterator il = lines_.begin(); il != lines_.end() ; ++il ) {
                if (*il == line) {
                lines_.erase( il );
                return true;
            }
        }
    }
        return false;
    }

    void  Net::add ( Line* line ){ 
        if (line) lines_.push_back( line );
    }

}