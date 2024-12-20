#pragma once
class VectG {
private:
	double values[3];

public:
	VectG();
	VectG(double x, double y, double z);
	~VectG();
	
	// Operators
	double& operator[](unsigned int i) { return values[i]; }
	double const& operator[](unsigned int i) const { return values[i]; }

	VectG& operator =(VectG const& rhs);
	VectG& operator +=(VectG const& rhs);
	VectG& operator -=(VectG const& rhs);
	VectG& operator *=(double a);

	VectG operator +(VectG const& rhs) const { VectG result(*this); result += rhs; return result; }
	VectG operator -(VectG const& rhs) const { VectG result(*this); result -= rhs; return result; }
	VectG operator *(double rhs) const { VectG result(*this); result *= rhs; return result; }
	double operator %(VectG const& rhs) const;  // dot product

	double length() const;
};

