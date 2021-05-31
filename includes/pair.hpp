#ifndef WEBSERV_PAIR_HPP
# define WEBSERV_PAIR_HPP

template <class T1, class T2>
struct pair
{
	public :
		typedef T1 first_type;
		typedef T2 second_type;

		first_type	first;
		second_type	second;

		pair()
		: first(), second()
		{}

		template<class U, class V>
		pair (const pair<U, V>& pr)
		: first(pr.first), second(pr.second)
		{}

		pair (const first_type& a, const second_type& b)
		: first(a), second(b)
		{}

		pair& operator= (const pair& pr)
		{
			if (this == &pr)
				return (*this);
			first = pr.first;
			second = pr.second;
			return (*this);
		}
};

#endif
