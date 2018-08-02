#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <string>


template <typename T> class WeightedNumber
{
public:
	T value;
	T weight;

	inline WeightedNumber(const WeightedNumber &n)
	{
		value = n.value;
		weight = n.weight;
	}

	inline WeightedNumber(const T v, const T w)
	{
		value = v;
		weight = w;
	}

	inline WeightedNumber<T> & operator = (const WeightedNumber<T> &n)
	{
		value = n.value;
		weight = n.weight;
		return *this;
	}

	inline WeightedNumber operator + (const T n) const
	{
		return WeightedNumber(value + n, weight + 1);
	}

	inline WeightedNumber & operator += (const T n)
	{
		value += n;
		weight += 1;
		return *this;
	}

	inline WeightedNumber operator + (const WeightedNumber &n) const
	{
		return WeightedNumber(value + n.value, weight + n.weight);
	}

	inline WeightedNumber & operator += (const WeightedNumber &n)
	{
		value += n.value;
		weight += n.weight;
		return *this;
	}

	inline WeightedNumber operator - (const T n) const
	{
		return WeightedNumber(value - n, weight - 1);
	}

	inline WeightedNumber & operator -= (const T n)
	{
		value -= n;
		weight -= 1;
		return *this;
	}

	inline WeightedNumber operator - (const WeightedNumber &n) const
	{
		return WeightedNumber(value - n.value, weight - n.weight);
	}

	inline WeightedNumber & operator -= (const WeightedNumber &n)
	{
		value -= n.value;
		weight -= n.weight;
		return *this;
	}

	inline WeightedNumber operator - () const
	{
		return WeightedNumber(-value, -weight);
	}

	//

	inline WeightedNumber operator * (const T n) const
	{
		return WeightedNumber(value * n, weight * n);
	}

	inline WeightedNumber & operator *= (const T n)
	{
		value *= n;
		weight *= n;
		return *this;
	}

	inline WeightedNumber operator * (const WeightedNumber &n) const
	{
		return WeightedNumber(value * n.value, weight * n.weight);
	}

	inline WeightedNumber & operator *= (const WeightedNumber &n)
	{
		value *= n.value;
		weight *= n.weight;
		return *this;
	}

	inline WeightedNumber operator / (const T n) const
	{
		return WeightedNumber(value / n, weight / n);
	}

	inline WeightedNumber & operator /= (const T n)
	{
		value /= n;
		weight /= n;
		return *this;
	}

	inline WeightedNumber operator / (const WeightedNumber &n) const
	{
		return WeightedNumber(value / n.value, weight / n.weight);
	}

	inline WeightedNumber & operator /= (const WeightedNumber &n)
	{
		value /= n.value;
		weight /= n.weight;
		return *this;
	}

	std::string __str__() const
	{
		char buffer[100];
		if (weight == (int)weight)
			sprintf(buffer, "%f (%d)", value, (int)weight);
		else
			sprintf(buffer, "%f (%f)", value, weight);
		return buffer;
	}

	std::string __repr__() const
	{
		const char * class_name = get_weighted_class_name(value);
		char buffer[100];
		if (weight == (int)weight)
			sprintf(buffer, "Weighted%s(%f, %d)", class_name, value, (int)weight);
		else
			sprintf(buffer, "Weighted%s(%f, %d)", class_name, value, weight);
		return buffer;
	}

	//

	bool operator ==(const WeightedNumber &n) const
	{
		if (value == n.value && weight == n.weight)
			return true;
		return value * n.weight == weight * n.value;
	}

	bool operator !=(const WeightedNumber &n)const
	{
		return value * n.weight != weight * n.value;
	}

	bool operator >(const WeightedNumber &n)const
	{
		return value * n.weight > weight * n.value;
	}

	bool operator >=(const WeightedNumber &n)const
	{
		return value * n.weight >= weight * n.value;
	}

	bool operator <(const WeightedNumber &n)const
	{
		return value * n.weight < weight * n.value;
	}

	bool operator <=(const WeightedNumber &n)const
	{
		return value * n.weight <= weight * n.value;
	}
};

const char * get_weighted_class_name(double x);
const char * get_weighted_class_name(float x);
const char * get_weighted_class_name(int x);


#define WeightedDouble WeightedNumber<double>
#define WeightedFloat WeightedNumber<float>


#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
