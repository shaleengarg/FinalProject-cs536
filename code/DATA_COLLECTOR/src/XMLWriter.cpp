/* XMLWriter
 * Simple XML writer
 */
#include "XML.h"

XMLWriter::XMLWriter(string outputPath){
	//cout << "[XMLWriter] Creating XMLWriter to output to "  + outputPath + "\n";
	inTag = false;
	indent = 0;
	path = outputPath;
	output.open(outputPath, fstream::trunc | fstream::out);
}

XMLWriter::~XMLWriter(){
	if(output.is_open()){
		this->close();
	}
}

void XMLWriter::startElement(string tag){
	if(inTag){
		output << ">\n";
	}	
	tags.push(tag);
	for(int x = 0; x < indent; x++){
		output << "\t";
	}
	indent++;
	output << "<" + tag;
	inTag = true;
}

void XMLWriter::writeAttribute(string attr, string value){
	if(!inTag){
		printf("[XMLWriter] (ERROR) Trying to write attr in place other than tag\n");
	}
	output << " " + attr + "=" + "\"" + value + "\"";
}

void XMLWriter::writeElementValue(string value){
	if(inTag){
		output << ">";
	}
	output << value;
	inTag = false;
}

void XMLWriter::endElement(){
	if(inTag){
		output << ">\n";
	}
	for(int x = 0; x < indent - 1; x++){
		output << "\t";
	}
	indent--;
	output << "</" + tags.top() + ">\n";
	tags.pop();
	inTag = false;
}

void XMLWriter::endElementInline(){
	if(inTag){
		output << ">";
	}
	output << "</" + tags.top() + ">\n";
	indent--;
	tags.pop();
	inTag = false;
}

void XMLWriter::addXMLFile(string path, int indent){
	fstream infile;
	string line;

	if(inTag){
		output << ">\n";
	}
	inTag = false;

	infile.open(path, fstream::in);
	while(getline(infile, line)){
		for(int x = 0; x < indent; x++) output << "\t";
		output << line + "\n";
	}
	infile.close();
}

void XMLWriter::close(){
	output.close();
	//cout << "[XMLWriter] Sucessfully written to " + path << "\n";
}


