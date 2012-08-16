/*
Copyright (C) 2006 - 2011 Evan Teran
                          eteran@alum.rit.edu
				   
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EXPRESSION_20070402_TCC_
#define EXPRESSION_20070402_TCC_

//------------------------------------------------------------------------------
// Name: Expression(const QString &s, variable_getter_t vg, memory_reader_t mr)
// Desc: 
//------------------------------------------------------------------------------
template <class T>
Expression<T>::Expression(const QString &s, variable_getter_t vg, memory_reader_t mr) : 
		expression_(s), expression_ptr_(expression_.begin()), 
		variable_reader_(vg), memory_reader_(mr) {
}

//------------------------------------------------------------------------------
// Name: eval_exp(T &result)
// Desc: private entry point with sanity check
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp(T &result) {
	if(token_.type_ == Token::UNKNOWN) {
		throw ExpressionError(ExpressionError::SYNTAX);
	}
	
	eval_exp0(result);
	
	switch(token_.type_) {
	case Token::OPERATOR:
		switch(token_.operator_) {
		case Token::LPAREN:
		case Token::RPAREN:
			throw ExpressionError(ExpressionError::UNBALANCED_PARENS);
		case Token::LBRACE:
		case Token::RBRACE:
			throw ExpressionError(ExpressionError::UNBALANCED_BRACES);
		default:
			throw ExpressionError(ExpressionError::UNEXPECTED_OPERATOR);
		}
		break;
	case Token::NUMBER:
		throw ExpressionError(ExpressionError::UNEXPECTED_NUMBER);
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------
// Name: eval_exp0(T &result)
// Desc: logic
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp0(T &result) {
	eval_exp1(result);

	for(Token op = token_; op.operator_ == Token::LOGICAL_AND || op.operator_ == Token::LOGICAL_OR; op = token_) {
		T partial_value;

		get_token();
		eval_exp1(partial_value);

		// add or subtract
		switch(op.operator_) {
		case Token::LOGICAL_AND:
			result = result && partial_value;
			break;
		case Token::LOGICAL_OR:
			result = result || partial_value;
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: eval_exp1(T &result)
// Desc: binary logic
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp1(T &result) {
	eval_exp2(result);

	for(Token op = token_; op.operator_ == Token::AND || op.operator_ == Token::OR || op.operator_ == Token::XOR; op = token_) {
		T partial_value;

		get_token();
		eval_exp2(partial_value);

		// add or subtract
		switch(op.operator_) {
		case Token::AND:
			result &= partial_value;
			break;
		case Token::OR:
			result |= partial_value;
			break;
		case Token::XOR:
			result ^= partial_value;
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: eval_exp2(T &result)
// Desc: comparisons
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp2(T &result) {
	eval_exp3(result);

	for(Token op = token_; op.operator_ == Token::LT || op.operator_ == Token::LE || op.operator_ == Token::GT || op.operator_ == Token::GE || op.operator_ == Token::EQ || op.operator_ == Token::NE; op = token_) {
		T partial_value;

		get_token();
		eval_exp3(partial_value);

		// perform the relational operation
		switch(op.operator_) {
		case Token::LT:
			result = result < partial_value;
			break;
		case Token::LE:
			result = result <= partial_value;
			break;
		case Token::GT:
			result = result > partial_value;
			break;
		case Token::GE:
			result = result >= partial_value;
			break;
		case Token::EQ:
			result = result == partial_value;
			break;
		case Token::NE:
			result = result != partial_value;
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: eval_exp3(T &result)
// Desc: shifts
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp3(T &result) {
	eval_exp4(result);

	for(Token op = token_; op.operator_ == Token::RSHFT || op.operator_ == Token::LSHFT; op = token_) {
		T partial_value;

		get_token();
		eval_exp4(partial_value);

		// perform the shift operation
		switch(op.operator_) {
		case Token::LSHFT:
			result <<= partial_value;
			break;
		case Token::RSHFT:
			result >>= partial_value;
			break;
		default:
			break;
		}
	}	
}

//------------------------------------------------------------------------------
// Name: eval_exp4(T &result)
// Desc: addition/subtraction
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp4(T &result) {
	eval_exp5(result);

	for(Token op = token_; op.operator_ == Token::PLUS || op.operator_ == Token::MINUS; op = token_) {
		T partial_value;

		get_token();
		eval_exp5(partial_value);

		// add or subtract
		switch(op.operator_) {
		case Token::PLUS:
			result += partial_value;
			break;
		case Token::MINUS:
#ifdef _MSC_VER
#pragma warning(push)
/* disable warning about applying unary - to an unsigned type */
#pragma warning(disable : 4146)
#endif
			result -= partial_value;
#ifdef _MSC_VER
#pragma warning(pop)
#endif
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: eval_exp5(T &result)
// Desc: multiplication/division
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp5(T &result) {
	eval_exp6(result);

	for(Token op = token_; op.operator_ == Token::MUL || op.operator_ == Token::DIV || op.operator_ == Token::MOD; op = token_) {
		T partial_value;

		get_token();
		eval_exp6(partial_value);

		// mul, div, or modulus
		switch(op.operator_) {
		case Token::MUL:
			result *= partial_value;
			break;
		case Token::DIV:
			if(partial_value == 0) {
				throw ExpressionError(ExpressionError::DIVIDE_BY_ZERO);
			}
			result /= partial_value;
			break;
		case Token::MOD:
			if(partial_value == 0) {
				throw ExpressionError(ExpressionError::DIVIDE_BY_ZERO);
			}
			result %= partial_value;
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: eval_exp6(T &result)
// Desc: unary expressions
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp6(T &result) {

	Token op = token_;
	if(op.operator_ == Token::PLUS || op.operator_ == Token::MINUS || op.operator_ == Token::CMP || op.operator_ == Token::NOT) {
		get_token();
	}

	eval_exp7(result);

	switch(op.operator_) {
	case Token::PLUS:
		// this may seems like a waste, but unary + can be overloaded for a type
		// to have a non-nop effect!
		result = +result;
		break;
	case Token::MINUS:
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4146)
#endif
		result = -result;
#ifdef _MSC_VER
#pragma warning(pop)
#endif
		break;
	case Token::CMP:
		result = ~result;
		break;
	case Token::NOT:
		result = !result;
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------
// Name: eval_exp7(T &result)
// Desc: sub-expressions
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_exp7(T &result) {

	switch(token_.operator_) {
	case Token::LPAREN:
		get_token();

		// get sub-expression
		eval_exp0(result);

		if(token_.operator_ != Token::RPAREN) {
			throw ExpressionError(ExpressionError::UNBALANCED_PARENS);
		}

		get_token();
		break;
	case Token::RPAREN:
		throw ExpressionError(ExpressionError::UNBALANCED_PARENS);
		break;
	case Token::LBRACE:
		do {
			get_token();

			// get sub-expression
			T effective_address;
			eval_exp0(effective_address);

			if(memory_reader_) {
				bool ok;
				ExpressionError error;
								
				result = memory_reader_(effective_address, ok, error);
				if(!ok) {
					throw error;
				}
			} else {
				throw ExpressionError(ExpressionError::CANNOT_READ_MEMORY);
			}

			if(token_.operator_ != Token::RBRACE) {
				throw ExpressionError(ExpressionError::UNBALANCED_BRACES);
			}

			get_token();
		} while(0);
		break;
	case Token::RBRACE:
		throw ExpressionError(ExpressionError::UNBALANCED_BRACES);
		break;
	default:
		eval_atom(result);
		break;
	
	}
}

//------------------------------------------------------------------------------
// Name: eval_atom(T &result)
// Desc: atoms (variables/constants)
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::eval_atom(T &result) {

	switch(token_.type_) {	
	case Token::VARIABLE:
		if(variable_reader_) {
			bool ok;
			ExpressionError error;
			result = variable_reader_(token_.data_, ok, error);
			if(!ok) {
				throw error;
			}			
		} else {
			throw ExpressionError(ExpressionError::UNKNOWN_VARIABLE);
		}
		get_token();	
		break;
	case Token::NUMBER:
		bool ok;
		result = token_.data_.toULongLong(&ok, 0);
		if(!ok) {
			throw ExpressionError(ExpressionError::INVALID_NUMBER);
		}
		get_token();	
		break;
	default:
		throw ExpressionError(ExpressionError::SYNTAX);
		break;
	}
}

//------------------------------------------------------------------------------
// Name: get_token()
// Desc: 
//------------------------------------------------------------------------------
template <class T>
void Expression<T>::get_token() {
	
	// clear previous token
	token_ = Token();

	// eat up white space
	while(expression_ptr_ != expression_.end() && expression_ptr_->isSpace()) {
		++expression_ptr_;
	}

	if(expression_ptr_ != expression_.end()) {

		// get the token
		switch(expression_ptr_->toAscii()) {
		case '(':
			++expression_ptr_;
			token_.set("(", Token::LPAREN, Token::OPERATOR);
			break;
		case ')':
			++expression_ptr_;
			token_.set(")", Token::RPAREN, Token::OPERATOR);
			break;
		case '[':
			++expression_ptr_;
			token_.set("[", Token::LBRACE, Token::OPERATOR);
			break;
		case ']':
			++expression_ptr_;
			token_.set("]", Token::RBRACE, Token::OPERATOR);
			break;
		case '!':
			++expression_ptr_;
			if(expression_ptr_ != expression_.end() && *expression_ptr_ == '=') {
				++expression_ptr_;
				token_.set("!=", Token::NE, Token::OPERATOR);
			} else {
				token_.set("!", Token::NOT, Token::OPERATOR);
			}
			break;
		case '+':
			++expression_ptr_;
			token_.set("+", Token::PLUS, Token::OPERATOR);
			break;
		case '-':
			++expression_ptr_;
			token_.set("-", Token::MINUS, Token::OPERATOR);
			break;
		case '*':
			++expression_ptr_;
			token_.set("*", Token::MUL, Token::OPERATOR);
			break;
		case '/':
			++expression_ptr_;
			token_.set("/", Token::DIV, Token::OPERATOR);
			break;
		case '%':
			++expression_ptr_;
			token_.set("%", Token::MOD, Token::OPERATOR);
			break;
		case '&':
			++expression_ptr_;
			if(expression_ptr_ != expression_.end() && *expression_ptr_ == '&') {
				++expression_ptr_;
				token_.set("&&", Token::LOGICAL_AND, Token::OPERATOR);
			} else {
				token_.set("&", Token::AND, Token::OPERATOR);
			}
			break;
		case '|':
			++expression_ptr_;
			if(expression_ptr_ != expression_.end() && *expression_ptr_ == '|') {
				++expression_ptr_;
				token_.set("||", Token::LOGICAL_OR, Token::OPERATOR);
			} else {
				token_.set("|", Token::OR, Token::OPERATOR);
			}
			break;
		case '^':
			++expression_ptr_;
			token_.set("^", Token::XOR, Token::OPERATOR);
			break;
		case '~':
			++expression_ptr_;
			token_.set("~", Token::CMP, Token::OPERATOR);
			break;
		case '=':
			++expression_ptr_;
			if(expression_ptr_ != expression_.end() && *expression_ptr_ == '=') {
				++expression_ptr_;
				token_.set("==", Token::EQ, Token::OPERATOR);
			} else {
				throw ExpressionError(ExpressionError::SYNTAX);
			}
			break;
		case '<':
			++expression_ptr_;
			if(expression_ptr_ != expression_.end() && *expression_ptr_ == '<') {
				++expression_ptr_;
				token_.set("<<", Token::LSHFT, Token::OPERATOR);
			} else if(expression_ptr_ != expression_.end() && *expression_ptr_ == '=') {
				++expression_ptr_;
				token_.set("<=", Token::LE, Token::OPERATOR);
			} else {
				token_.set("<", Token::LT, Token::OPERATOR);
			}
			break;
		case '>':
			++expression_ptr_;
			if(expression_ptr_ != expression_.end() && *expression_ptr_ == '>') {
				++expression_ptr_;
				token_.set(">>", Token::RSHFT, Token::OPERATOR);
			} else if(expression_ptr_ != expression_.end() && *expression_ptr_ == '=') {
				++expression_ptr_;
				token_.set(">=", Token::GE, Token::OPERATOR);
			} else {
				token_.set(">", Token::GT, Token::OPERATOR);
			}
			break;
		default:
			// is it a numerical constant?
			if(expression_ptr_->isDigit()) {
				QString temp_string;

				while(expression_ptr_ != expression_.end() && !is_delim(*expression_ptr_)) {
					temp_string += *expression_ptr_++;
				}

				token_.set(temp_string, Token::NONE, Token::NUMBER);
			} else {
				// it must be a variable, get its name
				QString temp_string;

				while(expression_ptr_ != expression_.end() && !is_delim(*expression_ptr_)) {
					temp_string += *expression_ptr_++;
				}

				token_.set(temp_string, Token::NONE, Token::VARIABLE);
			}
			break;
		}
	}		
}

#endif
