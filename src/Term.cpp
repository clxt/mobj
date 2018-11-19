#include  <cstdlib>
#include  "Instance.h"
#include  "Net.h"
#include  "Cell.h"
#include  "Term.h"
//#include  "Point.h"
#include "XmlUtil.h"

namespace Netlist {

  std::string     Term::toString    ( Term::Type t ){ 
    std::string s;
    switch(t){
      case 1: s = "Internal"; break;
      case 2: s = "External"; break;
    }
    return s;
  }
  std::string     Term::toString    ( Term::Direction d){
    std::string s;
    switch(d){
      case 1: s = "In"; break;
      case 2: s = "Out"; break;
      case 3: s = "Inout"; break;
      case 4: s = "Tristate"; break;
      case 5: s = "Transcv"; break;
      case 6: s = "Unknown"; break;
    }
    return s;
  }
  Term::Direction Term::toDirection ( std::string s){
    if( s == "In" )
      return Term::In;
    else if ( s == "Out" )
      return Term::Out;
    else if ( s == "Inout" )
      return Term::Inout;
    else if ( s == "Tristate" )
      return Term::Tristate;
    else if ( s == "Transcv" )
      return Term::Transcv;
    else 
      return Term::Unknown;
  }
  Term::Type Term::toType ( std::string s){
    if( s == "External" )
      return Term::External;
    else 
      return Term::Internal;
  }
  

  // CTOR

  Term::Term( Cell* owner, const std::string& name, Term::Direction dir)
    : owner_ (owner)
    , name_ (name)
    , direction_ (dir)
    , type_ ( External )
    , net_ ()
    , node_ (this, Node::noid)
  {
    static_cast<Cell*>(owner_)->add(this);
  }

  Term::Term( Instance* owner, const Term* modelTerm)
    : owner_(owner)
      , name_ ( modelTerm->getName() )
      , direction_( modelTerm->getDirection() )
      , type_ ( Internal )
      , net_ ()
      , node_ (this)
  {
    static_cast<Instance*>(owner_)->add(this);
  }

  // DTOR
  Term::~Term() {
    if (type_ == External)
      static_cast<Cell*>(owner_)->remove(this);
    else 
      static_cast<Instance*>(owner_)->remove(this);
  }


  Cell* Term::getOwnerCell() const
  { return (type_ == External)  ? static_cast<Cell*>(owner_) : static_cast<Instance*>(owner_)->getCell(); }

  void Term::setNet( Net* net ){
    if (net == NULL)
      net->remove( this->getNode());
    else
      net->add(this->getNode());
    net_ = net;
}
  void Term::setNet       ( const std::string& name ) { setNet( getOwnerCell()->getNet(name) ); }

  void Term::setPosition  ( const Point& p )  { node_.setPosition(p);   }
  void Term::setPosition  ( int x , int y )          { node_.setPosition(x,y); }

  void Term::toXml ( std::ostream& stream )
  {
    stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_)
                     << "\" x=\"" << this->getPosition().getX()
                     << "\" y=\"" << this->getPosition().getY()
                     << "\"/>\n";
  }

  Term* Term::fromXml ( Cell* cell, xmlTextReaderPtr reader ){
    std::string termName = xmlCharToString( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "name"));
    std::string termDir = xmlCharToString( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "direction"));
    int termX = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x"));
    int termY = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y"));

    Term* term = new Term(cell, termName, toDirection(termDir));
    term->setPosition(termX, termY);
      return term;
  }
}
