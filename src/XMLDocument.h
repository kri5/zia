#ifndef XMLDOCUMENT_H__
# define XMLDOCUMENT_H__

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "XMLNode.h"

/// Encapsulate XML classes.
namespace Xml
{
	/// Represent a XML document.
	class Document
	{
		public:
			/// Will construct the document by loading "filename".
			Document(const char* filename);
			~Document();
			Node*			getRootNode();
			xmlDocPtr		getDocumentPtr() const;
		private:
			xmlDocPtr		_doc;
			Node*			_rootNode;
	};
}

#endif // XMLDOCUMENT_H__

