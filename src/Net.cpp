#include  <cstdlib>
#include  "Cell.h"
#include  "Node.h"
//#include  "Term.h"
#include  "Line.h"
#include  "Net.h"
//#include  "Instance.h"
#include "XmlUtil.h"

namespace Netlist {

  // CTOR
  Net::Net( Cell* owner, const std::string& name, Term::Type ty)
    : owner_(owner), name_(name), type_(ty)
  {
    owner->add(this); 
  }

  // DTOR
  Net::~Net()
  {
    owner_->remove(this);
  }

  Cell*                     Net::getCell       () const{ return owner_; }

  const std::string&        Net::getName       () const{ return name_; }
  unsigned int              Net::getId         () const{ return id_; }
  Term::Type                Net::getType       () const{ return type_; }
  Node*                     Net::getNode       ( size_t id ) const{
    for (std::vector<Node*>::const_iterator it = nodes_.begin() ; it != nodes_.end() ; ++it)
      if ( (*it)->getId() == id ) return *it;
    return NULL;
  }
  const std::vector<Node*>& Net::getNodes      () const{ return nodes_; }

  size_t Net::getFreeNodeId () const{
    size_t position;
    std::vector<Node*>::const_iterator it = nodes_.begin();
    if (nodes_.begin() == nodes_.end()) return 0;
    for (it = nodes_.begin() ; it != nodes_.end() ; ++it)
      if (*it != NULL)
        position = (size_t) (it - nodes_.begin()); // mouais
    return position;
  }
  void Net::add( Node* node ){
    size_t size = getFreeNodeId();
    nodes_.insert(nodes_.begin() + size, node);
  }

  bool Net::remove( Node* node ){
    nodes_.erase( nodes_.begin() + node->getId() );
    delete node;
    return true;
  }
  void  Net::add ( Line* line )
  { if (line) lines_.push_back( line ); }

  bool  Net::remove ( Line* line ) {
    if (line) {
      for ( std::vector<Line*>::iterator il = lines_.begin() ; il != lines_.end() ; ++il ) {
        if (*il == line) {
          lines_.erase( il );
          return true;
        }
      }
    }
    return false;
  }


  void Net::toXml( std::ostream& stream )
  {
    stream << indent++ << "<net name=\"" << name_ << "\" type=\"" << Term::toString(type_) << "\">\n";
    for ( std::vector<Node*>::iterator inode=nodes_.begin() ; inode != nodes_.end() ; ++inode )
      (*inode)->toXml(stream);
    for ( std::vector<Line*>::iterator iline=lines_.begin() ; iline != lines_.end() ; ++iline )
      (*iline)->toXml(stream);
    stream << --indent << "</net>\n";
  }

  Net* Net::fromXml( Cell* cell , xmlTextReaderPtr reader )
  {
    const xmlChar* netTag = xmlTextReaderConstString( reader, (const xmlChar*)"net"  );
   // const xmlChar* nodeTag = xmlTextReaderConstString( reader, (const xmlChar*)"node"  );
    //const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line"  );

    Net* net = NULL;
    std::string netName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
    std::string netType = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ) );

    net = new Net ( cell, netName, Term::toType(netType));

    while(true){
      int status = xmlTextReaderRead(reader);
      if (status != 1) {
        if (status != 0) {
          std::cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser.\n";
        }
        break;
      }
      switch ( xmlTextReaderNodeType(reader) ) {
        case XML_READER_TYPE_COMMENT:
        case XML_READER_TYPE_WHITESPACE:
        case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
          continue;
      }
      const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

      if (Node::fromXml( net , reader) ){
        continue;
      }
      else if (Line::fromXml( net, reader ) ){
        continue; 
      }
      if ((nodeName == netTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) )
        break;
      else{
        std::cerr << "[ERROR] Net::fromXml(): Unknown or misplaced tag <" << nodeName
          << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
      }
    }
  return net;
  }
}
