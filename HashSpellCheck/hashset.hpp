/**
 * \file hashset.hpp
 *
 * \author What are your CS70 aliases?
 *
 * \brief Provides HashSet<T>, a set class template, using hash tables
 */

#ifndef HASHSET_HPP_INCLUDED
#define HASHSET_HPP_INCLUDED 1

// Any header files that are needed to typecheck the class
// declaration should be #included here.

// Templated interfaces (e.g., the HashSet class declarations) go here

// Most users of this header file would be interested in interfaces: how does
// one use a HashSet<T>? What member functions does it provide? What parameters
// do they take?
//
// Because the hashset code is templated, it must appear in this header file
// instead of a normal .cpp file. We do this by moving the nasty implementation
// details into hashset-private.hpp, and recursively including that here.  This
// way, readers don't see the actual code unless they want to, and aren't forced
// to manually include two separate header files every time they want to use
// this hash table.

#include "hashset-private.hpp"

#endif
