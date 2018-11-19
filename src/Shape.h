// -*- explicit-buffer-name: "Box.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include <iostream>
#include <algorithm>
#include "Box.h"
#include "Symbol.h"

namespace Netlist{

////////////////// Shape /////////////////////////////////////////////////////
class Shape {
  public :
                    Shape          ( Symbol* );
    virtual        ~Shape          ();

    inline  Symbol* getSymbol      () const ;
    virtual Box     getBoundingBox () const = 0;

    virtual void    toXml          ( std::ostream& ) = 0;
    static  Shape*  fromXml        ( Symbol*, xmlTextReaderPtr );

  private :
    Symbol* owner_;
};

inline Symbol* Shape::getSymbol () const { return owner_; }

////////////////// BoxShape /////////////////////////////////////////////////////
class BoxShape : public Shape {
  public :
                 BoxShape       ( Symbol*, const Box& );
                 BoxShape       ( Symbol*, int x1, int y1, int x2, int y2 );
    virtual     ~BoxShape       ();

    virtual Box  getBoundingBox () const ;

    virtual void toXml          ( std::ostream& );
    static  BoxShape* fromXml   ( Symbol*, xmlTextReaderPtr );
  private :
    Box box_;
};

////////////////// LineShape /////////////////////////////////////////////////////
class LineShape : public Shape {
  public :
                LineShape      ( Symbol*, int x1, int y1, int x2, int y2 );
    virtual    ~LineShape      ();

    virtual Box getBoundingBox () const ;

    virtual void toXml          ( std::ostream& );
    static  LineShape* fromXml  ( Symbol*, xmlTextReaderPtr );
  private :
    int x1_, y1_, x2_, y2_;
};

////////////////// EllipseShape /////////////////////////////////////////////////////
class EllipseShape : public Shape {
  public :
                EllipseShape   ( Symbol*, const Box& );
                EllipseShape   ( Symbol*, int x1, int y1, int x2, int y2 );
    virtual    ~EllipseShape   ();

    virtual Box getBoundingBox () const ;

    virtual void          toXml    ( std::ostream& );
    static  EllipseShape* fromXml  ( Symbol*, xmlTextReaderPtr );
  private :
    Box box_;
};

////////////////// ArcShape /////////////////////////////////////////////////////
class ArcShape : public Shape {
  public :
                 ArcShape       ( Symbol*, const Box&, int start, int span );
                 ArcShape       ( Symbol*, int x1, int y1, int x2, int y2, int start, int span );
    virtual     ~ArcShape       ();

    virtual Box  getBoundingBox () const ;
    inline  int  getStart       () const ;
    inline  int  getSpan        () const ;

    virtual void      toXml     ( std::ostream& );
    static  ArcShape* fromXml   ( Symbol*, xmlTextReaderPtr );
  private :
                 ArcShape       ( const ArcShape& );
    Box box_;
    int start_, span_;
};

inline int ArcShape::getStart () const { return start_; }
inline int ArcShape::getSpan  () const { return span_;  }

////////////////// TermShape /////////////////////////////////////////////////////
class TermShape : public Shape {
  public :
    enum              NameAlign      { TopLeft=1, TopRight, BottomLeft, BottomRight };

                      TermShape      ( Symbol*, std::string name, NameAlign, int x, int y );
    virtual          ~TermShape      ();
    virtual Box       getBoundingBox () const ;
    inline  Term*     getTerm        () const ;
    inline  NameAlign getAlign       () const ;
    inline  Point     getPosition    () const ;
    inline  int       getX           () const ;
    inline  int       getY           () const ;
    static  std::string toString       ( NameAlign );
    static  NameAlign toNameAlign      ( std::string );

    virtual void       toXml     ( std::ostream& );
    static  TermShape* fromXml   ( Symbol*, xmlTextReaderPtr );
  private :
    NameAlign align_;
    Term*     term_;
    int       x1_, y1_;
};

inline Term*     TermShape::getTerm () const { return term_; }
inline TermShape::NameAlign TermShape::getAlign() const { return align_; }
inline int       TermShape::getX    () const { return x1_;}
inline int       TermShape::getY    () const { return y1_;}
inline Point     TermShape::getPosition() const { return Point(x1_, y1_);}

//////////////////////////////////////////////////////////////////////////////////
}

#endif  // NETLIST_SHAPE_H
