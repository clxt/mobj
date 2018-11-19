// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QMenu>
#include  <QDockWidget>
#include  <QMenuBar>
#include  <QAction>
#include  <QKeySequence>
#include  "Cell.h"

#include  "CellWidget.h"
#include  "SaveCellDialog.h"
#include  "CellsLib.h"
#include  "InstancesWidget.h"
#include  "OpenCellDialog.h"

#include  "CellViewer.h"

namespace Netlist {

  CellViewer::CellViewer( QWidget* parent )
    : QMainWindow       (parent)
      , cellWidget_     (NULL)
      , saveCellDialog_ (NULL)
      , cellsLib_       (NULL)
      , instancesWidget_(NULL)
  {
    cellWidget_      = new CellWidget  ();
    saveCellDialog_  = new SaveCellDialog( this );
    cellsLib_        = new CellsLib( );
    instancesWidget_ = new InstancesWidget();

    instancesWidget_->setCellViewer(this);
    cellsLib_->setCellViewer(this);


    setCentralWidget( cellWidget_  );
    QMenu* fileMenu = menuBar()->addMenu( "&File" );
    QAction* action = new QAction( "&Save As", this );
    action->setStatusTip( "Save to disk (rename) the Cell" );
    action->setShortcut ( QKeySequence("CTRL+S") );
    action->setVisible   ( true );
    fileMenu ->addAction( action  );
    connect( action , SIGNAL(triggered ()), this , SLOT(saveCell ()) );

    action = new QAction( "&Open", this );
    action->setStatusTip( "Open from disk a Cell" );
    action->setShortcut ( QKeySequence("CTRL+O") );
    action->setVisible   ( true );
    fileMenu ->addAction( action  );
    connect( action , SIGNAL(triggered ()), this , SLOT(openCell ()) );



    action = new  QAction( "&Quit", this );
    action->setStatusTip( "Exit the Netlist Viewer" );
    action->setShortcut ( QKeySequence("CTRL+Q") );
    action->setVisible   ( true );
    fileMenu ->addAction( action  );
    connect( action , SIGNAL(triggered ()), this , SLOT(close ()) );

    fileMenu = menuBar()->addMenu( "&Tools" );
    QDockWidget* dock = new QDockWidget(tr("&Library"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(cellsLib_);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->toggleViewAction()->setShortcut( QKeySequence("CTRL+L") );
    fileMenu->addAction(dock->toggleViewAction());

    connect( this, SIGNAL(cellLoaded()), cellsLib_->getBaseModel(), SLOT(updateDatas()));

    QDockWidget* docki = new QDockWidget(tr("&Instances"), this);
    docki->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    docki->setWidget(instancesWidget_);
    addDockWidget(Qt::LeftDockWidgetArea, docki);
    docki->toggleViewAction()->setShortcut( QKeySequence("CTRL+I") );
    fileMenu->addAction(docki->toggleViewAction());

    //std::cerr << getCell()->getName().c_str() ;
     // setWindowTitle( yoshi );
  }

  CellViewer::~CellViewer()
  {}
  Cell* CellViewer::getCell() const
  { return cellWidget_->getCell(); }

  void CellViewer::setCell( Cell* cell )
  {
    cellWidget_->setCell(cell);
    instancesWidget_->setCell( cell);
    setWindowTitle( tr(cell->getName().c_str()) );
  
  }

  void CellViewer::saveCell()
  {
    Cell* cell = getCell ();
    if ( cell == NULL ) return;
    QString  cellName = cell->getName().c_str();
    if (saveCellDialog_->run(cellName)) {
      cell->setName( cellName.toStdString () );
      cell->save   ( cellName.toStdString () );
    }
  }

  void CellViewer::openCell()
  {
    QString  cellName;
    if (OpenCellDialog::run(this, cellName)) {
      Cell* cell = Cell::find(cellName.toStdString());
      if (not cell){
        cell = Cell::load(cellName.toStdString());
        emit cellLoaded();
      }
      if (cell)
        setCell(cell);
    }
  }

  void CellViewer::showCellsLib()
  {
    cellsLib_->show();
  }

  void CellViewer::showInstancesWidget(){
    instancesWidget_->setCell(getCell());
    instancesWidget_->show();
  }
 
}  // Netlist namespace.
