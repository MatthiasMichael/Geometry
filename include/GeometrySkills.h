#pragma once

#include "named_type.h"

template<typename T>
struct ElementAccessible : crtp<T, ElementAccessible>
{
	const auto & operator()(int i) const { return this->underlying().get()(i); }
	auto & operator()(int i) { return this->underlying().get()(i); }
};


template<typename Derived>
struct ScalarTypeSkills : Addable<Derived>, Subtractable<Derived>, Comparable<Derived>, Hashable<Derived> {};


template<typename Derived>
struct VectorTypeSkills : Addable<Derived>, Subtractable<Derived>, Hashable<Derived>, Equatable<Derived>, ElementAccessible<Derived> {};
