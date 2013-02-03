#ifndef __SERIALIZABLE_H__
#define __SERIALIZABLE_H__

#include <iostream>
#include <string>

class serializable {
public:
	serializable(char _name[]) {
		__name = new char[strlen(_name)+1];
		strcpy(__name, _name);
	}

	virtual ~serializable() {
		delete[] __name;
	}

	virtual std::ostream& toStream(std::ostream& out) const =0;
	virtual std::istream& fromStream(std::istream& in) =0;

	friend std::ostream& operator << (std::ostream& out, const serializable& ser) {
		out << ser.__name << " { ";
		ser.toStream(out);
		out << " }";

		return out;
	}

	friend std::istream& operator >> (std::istream& in, serializable& ser) {
		// osnyggt med fast storlek, menvafan
		char token[64];

		in >> token;
		ser.fromStream(in);
		//in >> token;

		return in;
	}

	char *__name;
};


/*
std::ostream& operator << (std::ostream& out, const Serializable& ser) {
	out << ser.name << " { ";
	ser.toStream(out);
	out << " }";

	return out;
}

std::istream& operator >> (std::istream& in, Serializable& ser) {
	// osnyggt med fast storlek, menvafan
	char token[64];

	in >> token >> token;
	ser.fromStream(in);
	in >> token;

	return in;
}
*/

#endif
