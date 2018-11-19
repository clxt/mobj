
#include "CellsModel.h"

namespace Netlist{
  
  CellsModel::CellsModel( QObject* parent )
    : QAbstractListModel(parent)
  { }

  CellsModel::~CellsModel()
  { }

  int CellsModel::rowCount(const QModelIndex& parent) const
  { return Cell::getAllCells().size(); }

  int CellsModel::columnCount(const QModelIndex& parent) const
  { return  1; }

  QVariant CellsModel::data(const QModelIndex& index, int role) const
  {
    if(not index.isValid()) return QVariant();
    if(role == Qt::DisplayRole) {
      int  row = index.row();
       return  Cell::getAllCells()[row]->getName().c_str();
    }
    return  QVariant();
  }

  QVariant CellsModel::headerData(int section, Qt::Orientation orientation, int  role ) const
  {
    if(orientation == Qt::Vertical) return QVariant();
    if(role != Qt::DisplayRole) return QVariant();
    switch( section ) {
      case 0:  return "Library";
    }
    return  QVariant();
  }

  Cell* CellsModel::getCell( int row )
  { // Selection de la Cell
    if(row >= (int)Cell::getAllCells().size()) return NULL;
    return Cell::getAllCells()[ row ];
  }
  void CellsModel::updateDatas() 
  {
    emit layoutChanged();
  }

} // Namespace

