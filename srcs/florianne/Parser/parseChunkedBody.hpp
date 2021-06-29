// #ifndef PARSECHUNKEDBODY_HPP
// #define PARSECHUNKEDBODY_HPP

// #include <string>
// #include <map>
// #include <iostream>

// class ParseChunkedBody
// {

// public:
// 	enum State
// 	{
// 		S_LENGTH,
// 		S_BEFORE_BODY,
// 		S_PARSE_BODY,
// 		S_AFTER_BODY,
// 		S_END_R,
// 		S_END_N,
// 		S_END,
// 	};
// private:
// 	std::string _body;
// 	State _state;
// 	int _count;
// 	std::string _nb;
// public:
//     ParseChunkedBody();
// 	ParseChunkedBody(std::string ParseChunkedBody);
//     ParseChunkedBody(ParseChunkedBody const & copy);
//     ~ParseChunkedBody();
//     ParseChunkedBody & operator=(ParseChunkedBody const & ope);

// public:
// 	void parse(char c);
// 	std::string get_Body()
// 	{
// 		return(_body);
// 	}
// 	State get_state()
// 	{
// 		return(_state);
// 	}

// };

// // GET 
// // POST
// // DELETE

// #endif