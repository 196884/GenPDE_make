/*
 * Exceptions.h
 *
 *  Created on: Oct 18, 2009
 *      Author: dupontr
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <iostream>
#include <sstream>
#include <string>

class Exception
{
public:
	Exception(
		const char* where_thrown,
		const char* what)
	:mWhereThrown(where_thrown)
	,mWhat(what)
	{}

	Exception(
		const char*        where_thrown,
		const std::string& what)
	:mWhereThrown(where_thrown)
	,mWhat(what)
	{}

	virtual void print() const
	{
		std::cout << "Exception thrown in ";
		std::cout << mWhereThrown;
		std::cout << ": ";
		std::cout << mWhat;
		std::cout << std::endl;
	}

	virtual std::string toString() const
	{
		std::ostringstream o;
		o << "Exception thrown in ";
		o << mWhereThrown;
		o << ": ";
		o << mWhat;
		return o.str();
	}

	static void raise(const char* where_thrown, const char* what)
	{
		throw Exception(where_thrown, what);
	}

	static void raise(const char* where_thrown, const std::string& what)
	{
		throw Exception(where_thrown, what);
	}
    
    static void check(bool b, const char* where_thrown, const char* what)
    {
        if( !b )
            raise(where_thrown, what);
    }

	static void notImplemented(const char* where_thrown)
	{
		throw Exception(where_thrown, "not implemented");
	}

private:
	/// The location where the exception was thrown
	std::string	mWhereThrown;

	/// The description of the exception
	std::string mWhat;
};

#endif /* EXCEPTIONS_H_ */
