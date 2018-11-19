#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QHeaderView>
#include  <QApplication>
#include  "CellWidget.h"
#include  "CellViewer.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"
#include  <QHBoxLayout>
#include  <QVBoxLayout>

#include <QPushButton>
#include <QModelIndex>
#include "CellViewer.h"
#include "CellsLib.h"

namespace Netlist {

  CellsLib::CellsLib( QWidget* parent )
    : QWidget        (parent)
     , cellViewer_   (NULL)
     , baseModel_    (new  CellsModel(this))
     , view_          (new  QListView(this))
     , load_          (new  QPushButton(this))
  {
  
    setAttribute( Qt::WA_QuitOnClose  , false  );
    setAttribute( Qt::WA_DeleteOnClose, false );
    setContextMenuPolicy( Qt::ActionsContextMenu  );
    //view_->setShowGrid              ( false  );
    view_->setAlternatingRowColors( true );
    view_->setSelectionBehavior   ( QAbstractItemView::SelectRows  );
    view_->setSelectionMode        ( QAbstractItemView::SingleSelection  );
    //view_->setSortingEnabled      ( true );
    view_->setModel                 ( baseModel_  );

    //QHeaderView* horizontalHeader = view_->horizontalHeader();
    //horizontalHeader->setDefaultAlignment  ( Qt::AlignHCenter  );
    //horizontalHeader->setMinimumSectionSize( 300 );
    //horizontalHeader->setStretchLastSection( true );
    //QHeaderView* verticalHeader = view_->verticalHeader();
    //verticalHeader->setVisible( false  );
    load_->setText( "Load" );
    //load_->setFocus();
    //load_->setDefault( true ); // ca marche pas :(
    connect( load_, SIGNAL(clicked()), this, SLOT(load()) );
    //connect( view_, SIGNAL(clicked()), this, SLOT(load()) );

    QHBoxLayout* hLayout = new QHBoxLayout ();
    hLayout->addStretch();
    hLayout->addWidget( load_ );
    hLayout->addStretch();

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->addWidget( view_ );
    vLayout->addLayout( hLayout );
    setLayout( vLayout );
  
  }
  void CellsLib::setCellViewer( CellViewer* cv)
  {
    if(cellViewer_)
      disconnect(this,0,cellViewer_,0);
    cellViewer_ = cv;
  }
  int CellsLib::getSelectedRow() const
  {
    QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
    if(selecteds.empty())  return  -1;
    return  selecteds.first().row();
  }
  void CellsLib::load()
  {
    int  selectedRow = getSelectedRow();
    if(selectedRow  < 0)  return;
    cellViewer_->setCell( baseModel_->getCell(selectedRow) );
  }

}
