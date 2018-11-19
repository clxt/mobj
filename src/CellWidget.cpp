// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-
 //#include <QGraphicsAnchorLayout> 
#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"


namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  CellWidget::CellWidget ( QWidget* parent )
    : QWidget(parent)
    , cell_  (NULL)
    , viewport_(Box(0,0,500,500))
  {
    setAttribute    ( Qt::WA_OpaquePaintEvent );
    setAttribute    ( Qt::WA_NoSystemBackground );
    setAttribute    ( Qt::WA_StaticContents );
    setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy  ( Qt::StrongFocus );
    setMouseTracking( true );
  }


  CellWidget::~CellWidget()
  { }


  void  CellWidget::setCell( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }


  QSize  CellWidget::minimumSizeHint() const
  { return QSize(500,500); }


  void  CellWidget::resizeEvent( QResizeEvent* event )
  { 
    const QSize& size = event->size();
    viewport_.setX2( viewport_.getX1() + size.width() );
    viewport_.setY1( viewport_.getY2() - size.height() );

    cerr << "CellWidget::resizeEvent() viewport_:" << viewport_ << endl;
    repaint();
  }

  void CellWidget::keyPressEvent(QKeyEvent* event)
  {
    event->ignore();
    if (event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier) )
      return;

    switch( event->key() ){
      case Qt::Key_Up:
      case Qt::Key_K:    goUp    (); break;
      case Qt::Key_J:
      case Qt::Key_Down:  goDown  (); break;
      case Qt::Key_H:
      case Qt::Key_Left:  goLeft  (); break;
      case Qt::Key_L:
      case Qt::Key_Right: goRight (); break;
      default: return;
    }
    event->accept();
  }

  void CellWidget::goRight()
  {
    viewport_.translate( Point(20,0) );
    repaint();
  }
  void CellWidget::goLeft()
  {
    viewport_.translate( Point(-20,0) );
    repaint();
  }
  void CellWidget::goUp()
  {
    viewport_.translate( Point(0,20) );
    repaint();
  }
  void CellWidget::goDown()
  {
    viewport_.translate( Point(0,-20) );
    repaint();
  }

  void  CellWidget::paintEvent( QPaintEvent* event )
  {
    QFont  bigFont = QFont( "URW Bookman L", 12 );

    QString cellName = "NULL";
    if (cell_) cellName = cell_->getName().c_str();

    QPainter painter(this);
    painter.setFont      ( bigFont );
    painter.setBackground( QBrush( Qt::black ) );
    painter.eraseRect    ( QRect( QPoint(0,0), size() ) );

    painter.setPen  ( QPen  ( Qt::blue, 1 ) );
    query(4, painter);

    painter.setPen  ( QPen  ( Qt::darkGreen, 1 ) );

    query(1, painter);

    painter.setPen  ( QPen  ( Qt::darkRed, 1 ) );
    painter.setBrush( QBrush( Qt::darkRed ) );

    query(2, painter);

  }

  void   CellWidget::query(unsigned  int flags , QPainter& painter) {
    int InstanceShapes = 1; 
    int InstanceConnectors = 2;
    int CellConnectors = 2;
    int Cables = 4;

    if ((not  cell_) or (not  flags ))  return;

    const  vector <Instance*>& instances = cell_ ->getInstances ();
    const  vector <Term*>&     terms     = cell_ ->getTerms     ();
    const  vector <Net*>&      nets      = cell_ ->getNets      ();

    for (size_t i=0 ; i < instances.size() ; ++i) {

      Point          instPos = instances[i]->getPosition ();
      const  Symbol* symbol  = instances[i]->getMasterCell()->getSymbol ();
      if (not  symbol) continue;



      const  vector<Shape*>& shapes = symbol->getShapes();
      if (flags & InstanceShapes)
        painter.drawText( pointToScreenPoint( instPos ) + QPoint(0,20), instances[i]->getName().c_str() );
      for (size_t j=0 ; j < shapes.size() ; ++j) {
        if (flags & InstanceShapes) {
          BoxShape* boxShape = dynamic_cast<BoxShape*>(shapes[j]);
          if (boxShape) {
            Box    box   = boxShape ->getBoundingBox ();
            QRect  rect = boxToScreenRect(box.translate(instPos ));
            painter.drawRect(rect);
          }

          LineShape* lineShape = dynamic_cast<LineShape*>(shapes[j]);
          if (lineShape) {
            Box    line = lineShape->getBoundingBox ();
            QRect  rect = boxToScreenRect(line.translate(instPos ));
            painter.drawRect(rect);
            //painter.drawLine(line.getX1(), line.getY1(), line.getX2(), line.getY2());
          }
          ArcShape* arcShape = dynamic_cast<ArcShape*>(shapes[j]);
          if (arcShape) {
            Box    arc = arcShape->getBoundingBox ();
            QRect  rect = boxToScreenRect(arc.translate(instPos ));
            painter.drawArc(rect, arcShape->getStart() * 16, arcShape->getSpan() *16 );
          }

          EllipseShape* ellipseShape = dynamic_cast<EllipseShape*>(shapes[j]);
          if (ellipseShape) {
            Box    box = ellipseShape->getBoundingBox ();
            QRect  rect = boxToScreenRect(box.translate(instPos ));
            painter.drawEllipse(rect);
          }
        }
        if (flags & InstanceConnectors ){
          TermShape* termShape = dynamic_cast<TermShape*>(shapes[j]);
          if (termShape) {
            Box    term = termShape->getBoundingBox().translate(-1,-1).inflate(5,5);
            QRect  rect = boxToScreenRect(term.translate(instPos ));
            switch(termShape->getAlign()){
              case TermShape::TopRight    : painter.drawText( rect.bottomLeft()                               , termShape-> getTerm() ->getName().c_str()); break;
              case TermShape::TopLeft     : painter.drawText( rect.bottomRight() + QPoint(rect.width()-5,-1 ) , termShape-> getTerm() ->getName().c_str()); break;
              case TermShape::BottomRight : painter.drawText( rect.topRight()    + QPoint(-10,10)              , termShape-> getTerm() ->getName().c_str()); break;
              case TermShape::BottomLeft  : painter.drawText( rect.topRight()    + QPoint(10,10)              , termShape-> getTerm() ->getName().c_str()); break;
            }
            painter.drawRect(rect);
          }
        }
      }
    }

    for (size_t i=0 ; i < terms.size() ; ++i) {
      Point          termPos = terms[i]->getPosition ();
      if (flags & CellConnectors ){
        if (terms[i]){
          int size = 8;
          if(terms[i]->getDirection() == Term::In){
            Point  p = terms[i]->getPosition ();
            QPoint qp = pointToScreenPoint(p );
            QPoint points[5] = {
              QPoint(qp) + QPoint(-size,size),
              QPoint(qp) + QPoint(-size,-size),
              QPoint(qp) + QPoint(size,-size),
              QPoint(qp) + QPoint(2*size,0),
              QPoint(qp) + QPoint(size,size)
            };
            painter.drawText( qp + QPoint(3*-size,0), terms[i]->getName().c_str()); 
            painter.drawPolygon(points, 5, Qt::WindingFill);
          }
          if(terms[i]->getDirection() == Term::Out){
            Point  p = terms[i]->getPosition ();
            QPoint qp = pointToScreenPoint(p );
            QPoint points[5] = {
              QPoint(qp) + QPoint(-size, size),
              QPoint(qp) + QPoint(-2*size,  0),
              QPoint(qp) + QPoint(-size,-size),
              QPoint(qp) + QPoint( size,-size),
              QPoint(qp) + QPoint( size, size)
            };
            painter.drawText( qp + QPoint(2* size,0), terms[i]->getName().c_str()); 
            painter.drawPolygon(points, 5, Qt::WindingFill);
          }
        }
      }
    }
    if (flags & Cables ) {
      for (size_t i=0 ; i < nets.size() ; ++i) {
        const vector<Line*>& lines = nets[i]->getLines();
        for (size_t j=0 ; j < lines.size() ; ++j) {
          painter.drawLine(pointToScreenPoint( lines[j]->getSourcePosition() )
                          ,pointToScreenPoint( lines[j]->getTargetPosition() )
              );
        }
      }
    }




  }

}  // Netlist namespace.
