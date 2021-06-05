#ifndef WEBSERV_USABLE_HPP
# define WEBSERV_USABLE_HPP

template <class T>
struct usable
{
	public :
		typedef T		value_type;
		typedef bool	state_type;

		value_type	value;
		state_type	state;

		usable()
		: value(), state(false)
		{}

		template<class U>
		usable (const usable<U>& pr)
		: value(pr.value), state(pr.state)
		{}

		usable (const value_type& a)
		: value(a), state(true)
		{}

		usable& operator= (const usable& pr)
		{
			if (this == &pr)
				return (*this);
			value = pr.value;
			state = pr.state;
			return (*this);
		}
};

#endif
