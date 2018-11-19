// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_VIEWER_H
#define NETLIST_CELL_VIEWER_H

#include <QMainWindow>

namespace Netlist {

  class Cell;
  class CellWidget;
  class SaveCellDialog;
  class InstancesWidget;
  class CellsLib;

  class CellViewer : public QMainWindow {
      Q_OBJECT;
    public:
                        CellViewer          ( QWidget* parent=NULL );
      virtual          ~CellViewer          ();
              Cell*     getCell             () const;
      inline  CellsLib* getCellsLib         ();  // TME9+.
    signals:
              void      cellLoaded          ();
    public slots:
              void      setCell             ( Cell* );
              void      saveCell            ();
              void      openCell            ();
              void      showCellsLib       ();  // TME9+.
              void      showInstancesWidget ();  // TME9+.
    private:
      CellWidget*      cellWidget_;
      SaveCellDialog*  saveCellDialog_;
      CellsLib*        cellsLib_;         // TME9+.
      InstancesWidget* instancesWidget_;  // TME9+.
  };

}  // Netlist namespace.

#endif  // NETLIST_CELL_VIEWER_H
