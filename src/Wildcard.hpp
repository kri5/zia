#ifndef WILDCARD_H__
# define WILDCARD_H__

/** Template interface which will force the developper to implement a method "compare", which will be overided
 * by templated class Wildcard<T>
 */
template <class C>
class	IWildcard
{
	public:
		virtual ~IWildcard(){}
		virtual bool	compare(const C&) const = 0;
		virtual bool	compare(const char *c) const = 0;
		virtual bool	operator==(const C&) const = 0;
		virtual bool	operator==(const char*) const = 0;
};

/// Template class which will force compare method to return true, in every case.
template <class C>
class	Wildcard
{
	public:
		/// Will only return true, since it's the only goal of this class.
		bool	compare(const C&) const {return true;}
		bool	compare(const char* c) const {return (c && c[0] == '*');}
		bool	operator==(const C&) const {return true;}
		bool	operator==(const char* c) const {return this->compare(c);}
};

#endif //WILDCARD_H__

