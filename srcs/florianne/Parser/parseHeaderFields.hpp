#ifndef PARSEHEADERFIELDS_HPP
#define PARSEHEADERFIELDS_HPP

#include <string>
#include <map>

class ParseHeaderFields
{
	public:
		enum State
		{
			S_FIELD,
			S_COLON,
			S_SPACES_BEFORE_VALUE,
			S_VALUE,
			S_SPACES_AFTER_VALUE,
			S_END_R,
			S_END_N,
			S_END_R2,
			S_END,
		};
	private:
		std::string _field;
		std::string _value;
		State _state;
		std::map<std::string, std::string> _headers;



	public:
		ParseHeaderFields();
		ParseHeaderFields(ParseHeaderFields const & copy);
		~ParseHeaderFields();
		ParseHeaderFields& operator=(ParseHeaderFields const & ope);

		void parse(char c);

		void add_header();

		State get_state()
		{
			return(_state);
		}

		std::string get_field()
		{
			return(_field);
		}

		std::map<std::string, std::string> get_headers_map()
		{
			return(_headers);
		}
};

//

#endif