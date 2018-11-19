#include  <cstdlib>
#include  "Cell.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include "XmlUtil.h"
#include "Shape.h"

namespace Netlist {
  Instance::Instance( Cell* owner, Cell* model, const std::string& name)
    : owner_(owner) , masterCell_(model) , name_(name) , terms_()
  {
    const std::vector<Term*>& terms = masterCell_->getTerms();
    for(std::vector<Term*>::const_iterator iterm=terms.begin(); iterm != terms.end(); ++iterm)
      new Term(this, *iterm);
    owner->add(this);
  }
  Instance::~Instance()
  {
    owner_->remove(this);
  } //TODO

  const std::string&        Instance::getName       () const { return name_; }
  Cell*                     Instance::getMasterCell () const { return masterCell_; }
  Cell*                     Instance::getCell       () const { return owner_; }
  const std::vector<Term*>& Instance::getTerms      () const { return terms_; }

  Term*                     Instance::getTerm       ( const std::string& s ) const {
    for ( std::vector<Term*>::const_iterator iterm = terms_.begin() ; iterm != terms_.end() ; ++iterm )
      if ( (*iterm)->getName() == s )
        return *iterm;
    return NULL;
  }

  Point Instance::getPosition() const { return position_; }

  bool Instance::connect( const std::string& name , Net* net ) {
    Term* term= getTerm(name);
    if (term == NULL) return false;
    term->setNet(net);
    return true;
  }

  void Instance::add( Term* term ) {
    if ( getTerm( term->getName() ) ) {
      std::cerr << "[ERROR] Attemp to add duplicated terminal <" << term->getName() << ">." << std::endl;
      exit( 1 );
    }
    terms_.push_back( term );
  }

  void Instance::remove( Term* term ) {
    for ( std::vector<Term*>::iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm )
      if (*iterm == term)
        terms_.erase( iterm );
  }
  void Instance::setPosition( const Point& p ) {
    setPosition(p.getX(), p.getY());
  } // TODO

  void Instance::setPosition( int x , int y ) {
        position_ = Point(x,y);
        Symbol* symbol = getMasterCell()->getSymbol();

        std::vector<Term*>::const_iterator it = terms_.begin();

        for(it = terms_.begin(); it != terms_.end(); ++it){
          TermShape* termshape = symbol->getTermShape((*it));
          if(termshape){
            (*it)->setPosition( termshape->getPosition().translate(position_));
          }
          else{
            (*it)->setPosition(position_);
          }
        }
  }

  void Instance::toXml( std::ostream& stream ){
    stream << indent << "<instance name=\"" << name_ << "\" mastercell=\"" << masterCell_->getName()
      << "\" x=\"" << position_.getX()
      << "\" y=\"" << position_.getY()
      << "\"/>\n";
  }
  Instance* Instance::fromXml ( Cell* cell, xmlTextReaderPtr reader ){
    std::string instanceName = xmlCharToString( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "name"));
    std::string instanceMC = xmlCharToString( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "mastercell"));
    int instanceX = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x"));
    int instanceY = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y"));
    Instance* inst = new Instance ( cell, Cell::find(instanceMC), instanceName);
    inst->setPosition(instanceX, instanceY);
    return inst;

    //return NULL;
  }
}
