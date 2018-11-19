// -*- explicit-buffer-name: "Box.cpp<M1-MOBJ/7>" -*-

#include "Cell.h"
#include "Term.h"
#include "Shape.h"
#include "XmlUtil.h"
  
namespace Netlist{  

/////////////////////////////////////////////////////////////////////////////////
/*           Shape   OK                                                     */
/////////////////////////////////////////////////////////////////////////////////

  Shape::Shape( Symbol* owner )
    : owner_(owner)
  { owner->add( this ); }

  Shape::~Shape()
  { owner_->remove( this ); }

  Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
  {
    // Factory-like method.
    const xmlChar* boxTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
    const xmlChar* ellipseTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
    const xmlChar* arcTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
    const xmlChar* lineTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
    const xmlChar* termTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
    const xmlChar* nodeName
      = xmlTextReaderConstLocalName( reader );

    Shape* shape = NULL;
    if (boxTag == nodeName)
      shape = BoxShape::fromXml( owner, reader );
    if (ellipseTag == nodeName)
      shape = EllipseShape::fromXml( owner, reader );
    if (arcTag == nodeName)
      shape = ArcShape::fromXml( owner, reader );
    if (lineTag == nodeName)
      shape = LineShape::fromXml( owner, reader );
    if (termTag == nodeName)
      shape = TermShape::fromXml( owner, reader );

    if (shape == NULL)
      std::cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
        << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;

    return shape;
  }

/////////////////////////////////////////////////////////////////////////////////
/*           BoxShape   OK                                                     */
/////////////////////////////////////////////////////////////////////////////////

  BoxShape::BoxShape( Symbol* owner, const Box& box )
    : Shape(owner), box_(box)
  { }
  BoxShape::BoxShape( Symbol* owner, int x1, int y1, int x2, int y2 )
    : Shape(owner), box_( x1, y1, x2, y2 )
  { }

  BoxShape::~BoxShape()
  { }

  Box BoxShape::getBoundingBox() const
  { return box_; }

  void BoxShape::toXml( std::ostream& stream )
  {
      stream << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
                       << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
  }

  BoxShape* BoxShape::fromXml( Symbol* symbol, xmlTextReaderPtr reader )
  {
    
    int x1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x1"));
    int y1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y1"));
    int x2 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x2"));
    int y2 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y2"));
    BoxShape* shape = new BoxShape(symbol, x1, y1, x2, y2);
    return shape;
  }

/////////////////////////////////////////////////////////////////////////////////
/*          LineShape   OK                                                     */
/////////////////////////////////////////////////////////////////////////////////
  LineShape::LineShape( Symbol* owner, int x1, int y1, int x2, int y2 )
    : Shape( owner ), x1_( x1 ), y1_( y1 ), x2_( x2 ), y2_( y2 )
  { }

  LineShape::~LineShape()
  { }

  Box LineShape::getBoundingBox() const
  { return Box( x1_, y1_, x2_, y2_); }

  void LineShape::toXml( std::ostream& stream )
  {
      stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_
                       << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>\n";
  }

  LineShape* LineShape::fromXml( Symbol* symbol, xmlTextReaderPtr reader )
  {
    int x1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x1"));
    int y1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y1"));
    int x2 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x2"));
    int y2 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y2"));
    LineShape* shape = new LineShape(symbol, x1, y1, x2, y2);
    return shape;
  }

/////////////////////////////////////////////////////////////////////////////////
/*                             EllipseShape   OK                               */
/////////////////////////////////////////////////////////////////////////////////
  EllipseShape::EllipseShape( Symbol* owner, const Box& box )
    : Shape(owner), box_(box)
  { }
  EllipseShape::EllipseShape( Symbol* owner, int x1, int y1, int x2, int y2)
    : Shape(owner), box_(x1, y1, x2, y2)
  { }
  EllipseShape::~EllipseShape()
  {}
  Box EllipseShape::getBoundingBox() const
  { return box_; }

  void EllipseShape::toXml( std::ostream& stream )
  {
    stream << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
           << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
  }

  EllipseShape* EllipseShape::fromXml( Symbol* symbol, xmlTextReaderPtr reader )
  {
    int x1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x1"));
    int y1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y1"));
    int x2 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x2"));
    int y2 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y2"));

    EllipseShape* shape = new EllipseShape( symbol, x1, y1, x2, y2);
    return shape;
  }

/////////////////////////////////////////////////////////////////////////////////
/*                              ArcShape    OK                                 */
/////////////////////////////////////////////////////////////////////////////////

  ArcShape::ArcShape( Symbol* owner, const Box& box, int start, int span)
    : Shape(owner), box_(box), start_(start), span_(span)
  { }
  ArcShape::ArcShape( Symbol* owner, int x1, int y1, int x2, int y2, int start, int span)
    : Shape(owner), box_(x1, y1, x2, y2), start_(start), span_(span)
  { }
  ArcShape::~ArcShape()
  { }

  Box ArcShape::getBoundingBox() const
  { return box_; }

  void ArcShape::toXml( std::ostream& stream )
  {
    stream << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
           << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2()
           << "\" start=\"" << start_ << "\" span=\"" << span_ << "\"/>\n";
  }

  ArcShape* ArcShape::fromXml( Symbol* symbol, xmlTextReaderPtr reader )
  {
    int x1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x1"));
    int y1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y1"));
    int x2 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x2"));
    int y2 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y2"));
    int start = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "start"));
    int span = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "span"));

    ArcShape* shape = new ArcShape( symbol, x1, y1, x2, y2, start, span );
    return shape;
  }

/////////////////////////////////////////////////////////////////////////////////
/*  TermShape    OK!                                                           */
/////////////////////////////////////////////////////////////////////////////////

  TermShape::TermShape( Symbol * owner, std::string name, TermShape::NameAlign align, int x1, int y1 )
    : Shape ( owner ), term_( NULL ), x1_( x1 ), y1_( y1 )
  {
    Cell* cell = owner->getCell();
    term_= cell->getTerm( name );
    align_ = align;
  }

  TermShape::~TermShape()
  { }

  Box TermShape::getBoundingBox() const
  { return Box( x1_, y1_, x1_, y1_); }

  std::string TermShape::toString(TermShape::NameAlign align)
  {
    switch(align){
      case 1: return "top_left"; break;
      case 2: return "top_right"; break;
      case 3: return "bottom_left"; break;
      case 4: return "bottom_right"; break;
      default: return "unknown";
    }
  }

  TermShape::NameAlign TermShape::toNameAlign(std::string s)
  {
    if (s == "top_left")
      return TermShape::TopLeft;
    else if (s == "top_right")
      return TermShape::TopRight;
    else if (s == "bottom_left")
      return TermShape::BottomLeft;
    else 
      return TermShape::BottomRight;
  }

  void TermShape::toXml( std::ostream& stream )
  {
    stream << indent << "<term name=\"" << term_->getName() << "\" x1=\"" << x1_
           << "\" y1=\"" << y1_ << "\" align=\"" << toString(align_) << "\"/>\n";
  }

  TermShape* TermShape::fromXml( Symbol* symbol, xmlTextReaderPtr reader )
  {
    std::string name = xmlCharToString( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "name"));
    std::string align = xmlCharToString( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "align"));
    int x1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "x1"));
    int y1 = xmlCharToInt( xmlTextReaderGetAttribute ( reader,(const xmlChar*) "y1"));

    TermShape* shape = new TermShape( symbol, name, toNameAlign(align), x1, y1 );

    return shape;
  }

} // Netlist namespace
