// -*- explicit-buffer-name: "SaveCellDialogt.h<M1-MOBJ/8-10>" -*-
  
#ifndef NETLIST_SAVE_CELL_DIALOG_H
#define NETLIST_SAVE_CELL_DIALOG_H

#include <QDialog>
class QCheckBox;
class QLineEdit;

namespace Netlist{


  class SaveCellDialog : public QDialog {
      Q_OBJECT;
    public:
                     SaveCellDialog( QWidget* parent=NULL );
      bool           run            ( QString& name );
      const QString  getCellName    ()  const;
      void           setCellName    ( const  QString& );
    protected:
      QLineEdit*     lineEdit_;
};
}
#endif // NETLIST_SAVE_CELL_DIALOG_H
