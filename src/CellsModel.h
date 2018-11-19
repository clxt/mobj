// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELLS_MODEL_H
#define NETLIST_CELLS_MODEL_H

#include <QAbstractListModel>
#include "Cell.h"

namespace Netlist{

  class  CellsModel : public  QAbstractListModel {
    Q_OBJECT;
    public:
                  CellsModel ( QObject* parent=NULL );
                 ~CellsModel ();

      int         rowCount       ( const QModelIndex& parent=QModelIndex () ) const;
      int         columnCount    ( const QModelIndex& parent=QModelIndex () ) const;
      QVariant    data           ( const QModelIndex& index , int  role=Qt:: DisplayRole ) const;
      QVariant    headerData     ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
      Cell*       getCell        ( int  row );
    public slots:
      void        updateDatas    ();
  };

} //Netlist namespace

#endif  // NETLIST_CELLS_MODEL_H
