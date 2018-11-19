// -*- explicit-buffer-name: "OpenCellDialogt.h<M1-MOBJ/8-10>" -*-
  
#ifndef NETLIST_OPEN_CELL_DIALOG_H
#define NETLIST_OPEN_CELL_DIALOG_H

#include <QDialog>
class QCheckBox;
class QLineEdit;

namespace Netlist{


  class OpenCellDialog : public QDialog {
      Q_OBJECT;
    public:
      static bool    run            ( QWidget* parent, QString& name );
      const QString  getCellName    ()  const;
      void           setCellName    ( const  QString& );
    private:
                     OpenCellDialog( QWidget* parent=NULL );
    protected:
      QLineEdit*     lineEdit_;
};
}
#endif // NETLIST_OPEN_CELL_DIALOG_H

