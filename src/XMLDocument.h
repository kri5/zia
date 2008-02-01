#ifndef XMLDOCUMENT_H__
# define XMLDOCUMENT_H__

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "XMLNode.h"

namespace Xml
{
	class Document
	{
		public:
			/// Will construct the document by loading "filename".
			Document(const char* filename);
			~Document();
			Node*	getRootNode();
		private:
			xmlDocPtr		_doc;
			Node*			_rootNode;
	};
}

#endif // XMLDOCUMENT_H__