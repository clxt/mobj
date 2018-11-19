#ifndef  NETLIST_NET_H   // Le define *doit* être différent dans
#define  NETLIST_NET_H   // chaque fichier (i.e. nom de classe).

// Les includes sont mis *à l'extérieur* du namespace.
#include <string>
#include <vector>
#include "Indentation.h"
#include "Term.h"
//#include "Instance.h"
#include  <libxml/xmlreader.h>

namespace Netlist {

  // Les *forward declarations* des autres objets de la
  // structure de données sont mis *à l'intérieur* du namespace.
  class Cell;
  class Node;
  class Line;
  //class Term;
  //class Instance;

  class Net {
    public:
                                Net (Cell*
                                    , const std::string&
                                    , Term::Type );
                                ~Net ();

             Cell*                     getCell       () const;
             const std::string&        getName       () const;
             unsigned int              getId         () const;
             Term::Type                getType       () const;
             Node*                     getNode       ( size_t id ) const;
             const std::vector<Node*>& getNodes      () const;
             size_t                    getFreeNodeId () const;
      inline const std::vector<Line*>& getLines      () const ;

             void                      add           ( Node* );
             bool                      remove        ( Node* );
             void                      add           ( Line* );
             bool                      remove        ( Line* );

             void                      toXml         ( std::ostream& );
      static Net*                      fromXml       ( Cell*, xmlTextReaderPtr );
    private:
      Cell*                     owner_;
      std::string               name_;
      unsigned int              id_;
      Term::Type                type_;
      std::vector<Node*>        nodes_;
      std::vector<Line*>        lines_;

  };
  inline const std::vector<Line*>& Net::getLines () const { return lines_; }

}

#endif  // NETLIST_NET_H

