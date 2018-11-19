// -*- explicit-buffer-name: "Box.cpp<M1-MOBJ/7>" -*-

#include  <cstdlib>
#include  <fstream>
#include  "XmlUtil.h"
#include "Symbol.h"
#include "Term.h"
#include "Cell.h"
#include "Shape.h"

namespace Netlist {


  Symbol::Symbol( Cell* cell)
    : owner_(cell)
  { }
  Symbol::~Symbol()
  { }

  Cell* Symbol::getCell() const
  { return owner_; }

  Box Symbol::getBoundingBox() const
  { // toutes les boundingBox mergees
    Box bb;
    std::vector<Shape*>::const_iterator ishape = shapes_.begin();
    for( ; ishape != shapes_.end() ; ++ishape)
      bb.merge(( *ishape)->getBoundingBox());
    return bb;
  }

  Point Symbol::getTermPosition( Term* term ) const
  { 
    TermShape* shape = getTermShape( term );
    if (shape) return shape->getPosition();
    
    return Point();
  
  }

  TermShape* Symbol::getTermShape( Term* term) const
  { 
//    Box bb;
//    std::vector<Shape*>
      for( size_t i = 0; i < shapes_.size(); ++i){
        TermShape* termShape = dynamic_cast<TermShape*>(shapes_[i]);
        if (termShape and termShape->getTerm()->getName() == term->getName())
          return termShape;
      }
      //      
      return NULL;
  }

  void Symbol::add( Shape* shape )
  { 
    if (shape){
      shapes_.push_back( shape ); 
    }
    else
      std::cerr << "[ERROR] In Symbol::add(Shape*): Tried to add a NULL shape\n";
  }

  void Symbol::remove( Shape* shape )
  {
    if(shape){
    std::vector<Shape*>::iterator ishape = shapes_.begin();
    for( ; ishape != shapes_.end() ; ++ishape)
      if ( *ishape == shape ) 
        shapes_.erase(ishape);
      else
        std::cerr << "[ERROR] In Symbol::remove(Shape*): Tried to remove a NULL shape\n";
    }
  }
  void Symbol::toXml( std::ostream& stream) const
  {
    stream << indent++ << "<symbol>\n";
    //if (shapes_.empty())
    //  std::cerr << "[ERROR] : shapes_ is empty\n";
    std::vector<Shape*>::const_iterator ishape = shapes_.begin();
    for (  ; ishape != shapes_.end() ; ++ishape ){
      (*ishape)->toXml(stream);
    }
    stream << --indent << "</symbol>\n";
  
  }


  Symbol* Symbol::fromXml( Cell* cell, xmlTextReaderPtr reader)
  {
    const xmlChar* symbolTag = xmlTextReaderConstString( reader, (const xmlChar*)"symbol"  );
   // const xmlChar* nodeTag = xmlTextReaderConstString( reader, (const xmlChar*)"node"  );
    //const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line"  );

    //Symbol* symbol = NULL;

    Symbol* symbol = cell->getSymbol();
    while(true){
      int status = xmlTextReaderRead(reader);
      if (status != 1) {
        if (status != 0) {
          std::cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser.\n" ;
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

      if (nodeName != symbolTag){
        Shape::fromXml(symbol, reader);
        //cell->getSymbol()->add(Shape::fromXml(cell->getSymbol(), reader));
        continue;
      }
      if ((nodeName == symbolTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) )
        break;
      else{
        std::cerr << "[ERROR] Symbol::fromXml(): Unknown or misplaced tag <" << nodeName
          << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
      }
    }
    return symbol;
  }



}  // Netlist namespace.
