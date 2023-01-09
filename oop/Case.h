#ifndef CASE_H
#define CASE_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <vector>

class Case {
private:
	std::string _label;
	int _colour;

	static unsigned int numCases;
public:
	Case(std::string label, int colour) : 
		_label(label),
		_colour(colour)
	{
		numCases++;
	}

	Case(const Case& original) :
		_label(original._label),
		_colour(original._colour)
	{
		if (this != &original) {
			numCases++;
		}
	}

	virtual ~Case() {
		numCases--;
	}

	static unsigned int count() {
		return numCases;
	}

	void plusCase() {
		numCases++;
	}

	std::string getLabel() {
		return _label;
	}

	int getColour() {
		return _colour;
	}

	void setLabel(std::string label) {
		_label = label;
	}

	void setColour(int colour) {
		_colour = colour;
	}

	virtual double getCapacity() = 0;
};

class BrickCase : public Case {
private:
	double _length;
	double _width;
	double _height;
public:
	BrickCase(std::string label, int colour, double length, double width, double height) :
		Case(label, colour), _length(length), _width(width), _height(height)
	{
		plusCase();
	}

	BrickCase(const BrickCase& original) :
		Case(original),
		_length(original._length),
		_width(original._width),
		_height(original._height)
	{
		if (this != &original) {
			plusCase();
		}
	}

	double getCapacity() {
		return _length * _width * _length;
	}
};

class TubeCase : public Case {
private: 
	double _radius;
	double _height;
public:
	TubeCase(std::string label, int colour, double radius, double height) :
		Case(label, colour), _radius(radius), _height(height)
	{
		plusCase();
	}

	TubeCase(const TubeCase& original) :
		Case(original),
		_radius(original._radius),
		_height(original._height)
	{
		if (this != &original) {
			plusCase();
		}
	}

	double getCapacity() {
		double pi = 3.14;
		return pi * _radius * _radius * _height;
	}

};

class PrismCase : public Case {
private:
	double _side;
	double _height;
public:
	PrismCase(std::string label, int colour, double side, double height) :
		Case(label, colour), _side(side), _height(height)
	{
		plusCase();
	}

	PrismCase(const PrismCase& original) :
		Case(original),
		_side(original._side),
		_height(original._height)
	{
		if (this != &original) {
			plusCase();
		}
	}

	double getCapacity() {
		return sqrt(3 / 4) * _side * _side * _height;
	}
};

class Repository {
private:
	std::string _description;
	double _totalCapacity;
	std::vector<Case*> listCases;
public:
	class CapacityError : public std::logic_error {
	public:
		CapacityError() : logic_error("CapacityError") {}
		std::string what() {
			return "CapacityError";
		}
	};

	class NameError : public std::logic_error {
	public:
		NameError() : logic_error("NameError") {}
		std::string what() {
			return "NameError";
		}
	};

	class IndexError : public std::logic_error {
	public:
		IndexError() : logic_error("IndexError") {}
		std::string what() {
			return "IndexError";
		}
	};

	Repository(std::string description, double totalCapacity) :
		_description(description),
		_totalCapacity(totalCapacity)
	{}

	~Repository() {
		while (!listCases.empty()) {
			listCases.pop_back();
		}
	} 

	void add(Case* c) {
		if (summaryVolume() + c->getCapacity() > _totalCapacity) {
			throw CapacityError();
		}
		listCases.push_back(c);
	}

	void remove(std::string label) {
		size_t t = 0;
		for (size_t i = 0; i < listCases.size(); i++) {
			if (listCases[i]->getLabel() == label) {
				listCases.erase(listCases.begin() + i);
				break;
			}
			t = i;
		}
		if (t == listCases.size()) {
			throw NameError();
		}
	}

	void clear() {
		while (!listCases.empty()) {
			listCases.pop_back();
		}
	}

	double summaryVolume() {
		double sum = 0;
		for (size_t i = 0; i < listCases.size(); i++) {
			sum += listCases[i]->getCapacity();
		}
		return sum;
	}

	Case* operator[](int index) {
		if (index >= listCases.size() || index < 0) {
			throw IndexError();
		}
		return listCases[index];
	}

	friend std::ostream& operator<<(std::ostream& out, Repository& repo) {
		out << repo._description << ", total capacity: " << repo._totalCapacity
			<< ", free space: " << repo._totalCapacity - repo.summaryVolume()
			<< std::endl;
		for (size_t i = 0; i < repo.listCases.size(); i++) {
			out << i + 1 << ". " << repo[i]->getLabel() << ", "
				<< repo[i]->getColour() << ", "
				<< "volume: " << repo[i]->getCapacity() << std::endl;
		}
		return out;
	}
};

unsigned int Case::numCases = 0;

#endif