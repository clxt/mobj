#ifndef  NETLIST_INSTANCE_H   // Le define *doit* être différent dans
#define  NETLIST_INSTANCE_H   // chaque fichier (i.e. nom de classe).

// Les includes sont mis *à l'extérieur* du namespace.
#include <string>
#include <vector>
#include "Indentation.h"
#include "Point.h"
#include "Point.h"
#include  <libxml/xmlreader.h>

namespace Netlist {

  // Les *forward declarations* des autres objets de la
  // structure de données sont mis *à l'intérieur* du namespace.
  class Cell;
  class Term;
  class Net;

  class Instance {
    public:
            Instance (Cell* owner, Cell* model, const std::string& );
           ~Instance ();

      const std::string&        getName       () const ;
      Cell*                     getMasterCell () const ;
      Cell*                     getCell       () const ;
      const std::vector<Term*>& getTerms      () const ;
      Term*                     getTerm       ( const std::string& ) const ;
      Point                     getPosition   () const ;

      bool                      connect       ( const std::string& name, Net* );
      void                      add           ( Term* );
      void                      remove        ( Term* );
      void                      setPosition   ( const Point& );
      void                      setPosition   ( int x , int y );

      void                      toXml         ( std::ostream& );
static      Instance*                 fromXml       ( Cell* , xmlTextReaderPtr );
    private:
      Cell*                     owner_ ;
      Cell*                     masterCell_ ;
      std::string               name_ ;
      std::vector<Term*>        terms_ ;
      Point                     position_ ;
  };
}
#endif  // NETLIST_INSTANCE_H
