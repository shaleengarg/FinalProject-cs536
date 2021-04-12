#ifndef XML_H
#define XML_H

#include <iostream>
#include <string>
#include <stack>
#include <fstream>

using namespace std;

class XMLWriter{
public:
	XMLWriter(string outputPath);
	~XMLWriter();
	void startElement(string tag);
	void writeAttribute(string attr, string value);
	void writeElementValue(string value);
	void endElement();
	void endElementInline();
	void addXMLFile(string path, int indent);
	void close();

private:
	stack<string> tags;
	int indent;
	fstream output;
	bool inTag;
	string path;
};

#endif
