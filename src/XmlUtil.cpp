// -*- explicit-buffer-name: "XmlUtil.cpp<M1-MOBJ/6>" -*-


#include "XmlUtil.h"
#include <cstdlib>
#include <iostream>


namespace Netlist {

  using namespace std;


  std::string  xmlCharToString( xmlChar* xmls )
  {
    string s;

    if (xmls) {
      s = (char*)xmls;
      free( xmls );
    }

    return s;
  }
  int xmlCharToInt( xmlChar* xmls )
  {
    int i;

    if (xmls) {
      std::string s = (char*)xmls;
      i = atoi((char*)s.c_str());
      free( xmls );
    }

    return i;
  }


  bool  xmlGetIntAttribute ( xmlTextReaderPtr reader, const string& attribute, int& value )
  {
    xmlChar* textValue = xmlTextReaderGetAttribute( reader, (const xmlChar*)attribute.c_str() );

    if (textValue == NULL) {
      const xmlChar* nodeName = xmlTextReaderConstLocalName     ( reader );
      cerr << "[ERROR] \"" << attribute << "\" attribute missing in <"
           << (const char*)nodeName << "> tag (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
      return false;
    }
    value = atoi( (char*)textValue );
    return true;
  }


}   // Netlist namespace.
