#include "VectG.h"
#include <functional>
#include <algorithm>
#include <cmath>

VectG::VectG(){
	values[0] = 0.0;
	values[1] = 0.0;
	values[2] = 0.0;
}

VectG::VectG(double x, double y, double z) {
	values[0] = x;
	values[1] = y;
	values[2] = z;
}

VectG::~VectG(){
}

VectG& VectG::operator =(VectG const& rhs) {
	values[0] = rhs.values[0];
	values[1] = rhs.values[1];
	values[2] = rhs.values[2];
	return *this;
}

VectG& VectG::operator +=(VectG const& rhs) {
	std::transform(values, values + 3, rhs.values, values, std::plus<double>());
	return *this;
}
VectG& VectG::operator -=(VectG const& rhs) {
	std::transform(values, values + 3, rhs.values, values, std::minus<double>());
	return *this;
}
VectG& VectG::operator *=(double a) {
	std::transform(values, values + 3, values, std::bind1st(std::multiplies<double>(), a));
	return *this;
}

double VectG::operator %(VectG const& rhs) const{
	double result = 0.0;
	result = values[0] * rhs.values[0] + values[1] * rhs.values[1] + values[2] * rhs.values[2];
	return result;
}

double VectG::length() const {
	return sqrt(values[0] * values[0] + values[1] * values[1] + values[2] * values[2]);
}