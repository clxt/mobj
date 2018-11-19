// -*- explicit-buffer-name: "InstancesWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_INSTANCES_WIDGET_H
#define NETLIST_INSTANCES_WIDGET_H

//#include <QWidget>
//#include <QPixmap>
//#include <QPainter>
//#include <QRect>
//#include <QPoint>
//class QPainter;
//#include "Box.h"
//#include "CellViewer.h"
//#include <QPushButton>
//#include <QMainWindow>
//class QT

#include <QWidget>
#include <QTableView>
#include "InstancesModel.h"

class QPushButton;
class QModelIndex;

namespace Netlist {

  class CellViewer;

  class InstancesWidget : public QWidget {
      Q_OBJECT;
    public:
                    InstancesWidget ( QWidget* parent=NULL );
              void  setCellViewer   ( CellViewer* );
              int   getSelectedRow  () const;
      inline  void  setCell         ( Cell* );
    public slots:
              void  load            ();
    private:
              CellViewer*     cellViewer_;
              InstancesModel* baseModel_;
              QTableView*     view_;
              QPushButton*    load_;
  };

  inline void InstancesWidget::setCell(Cell* cell) { baseModel_->setCell(cell); }

}  // Netlist namespace.

#endif  // NETLIST_INSTANCES_WIDGET_H
