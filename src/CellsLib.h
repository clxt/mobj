// -*- explicit-buffer-name: "CellsLib.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELLS_LIB_H
#define NETLIST_CELLS_LIB_H

#include <QWidget>
#include <QListView>
#include "CellsModel.h"
class QPushButton;
class QModelIndex;

namespace Netlist {

  class CellViewer;

  class CellsLib : public QWidget {
      Q_OBJECT;
    public:
                         CellsLib       ( QWidget* parent=NULL );
             void        setCellViewer  ( CellViewer* );
             int         getSelectedRow () const;
      inline CellsModel* getBaseModel   ();
    public slots:
             void        load           ();
    private:
      CellViewer*  cellViewer_;
      CellsModel*  baseModel_;
      QListView*  view_;
      QPushButton* load_;
  };
  inline CellsModel* CellsLib::getBaseModel() { return baseModel_; }

}  // Netlist namespace.

#endif  // NETLIST_CELLS_LIB_H
