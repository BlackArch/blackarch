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

#ifndef EXPRESSION_20070402_H_
#define EXPRESSION_20070402_H_

#include <QString>
#include <boost/function.hpp>

struct ExpressionError {
public:
	enum ERROR_MSG {
		NONE,
		SYNTAX,
		UNBALANCED_PARENS,
		UNBALANCED_BRACES,
		DIVIDE_BY_ZERO,
		INVALID_NUMBER,
		UNKNOWN_VARIABLE,
		CANNOT_READ_MEMORY,
		UNEXPECTED_OPERATOR,
		UNEXPECTED_NUMBER
	};

public:
	ExpressionError() : error_(NONE) {
	}

	explicit ExpressionError(ERROR_MSG type) : error_(type) {
	}

	~ExpressionError() throw() {
	}

	const char *what() const throw() {
		switch(error_) {
		case SYNTAX:
			return "Syntax Error";
		case UNBALANCED_PARENS:
			return "Unbalanced Parenthesis";
		case DIVIDE_BY_ZERO:
			return "Divide By Zero";
		case INVALID_NUMBER:
			return "Invalid Numerical Constant";
		case UNKNOWN_VARIABLE:
			return "Unknown Variable";
		case UNBALANCED_BRACES:
			return "Unbalanced Braces";
		case CANNOT_READ_MEMORY:
			return "Cannot Read Memory At the Effective Address";
		case UNEXPECTED_OPERATOR:
			return "Unexpected Operator";
		case UNEXPECTED_NUMBER:
			return "Unexpected Numerical Constant";
		default:
			return "Unknown Error";
		}
	}
private:
	ERROR_MSG error_;
};


template <class T>
class Expression {
public:
	typedef boost::function<T(const QString&, bool&, ExpressionError&)> variable_getter_t;
	typedef boost::function<T(T, bool&, ExpressionError&)>              memory_reader_t;

public:
	Expression(const QString &s, variable_getter_t vg, memory_reader_t mr);
	~Expression() {}

private:
	struct Token {
		Token()                   : operator_(NONE), type_(UNKNOWN) { }
		Token(const Token& other) : data_(other.data_), operator_(other.operator_), type_(other.type_) { }

		QString data_;

		enum Operator {
			NONE,
			AND,
			OR,
			XOR,
			LSHFT,
			RSHFT,
			PLUS,
			MINUS,
			MUL,
			DIV,
			MOD,
			CMP,
			LPAREN,
			RPAREN,
			LBRACE,
			RBRACE,
			NOT,
			LT,
			LE,
			GT,
			GE,
			EQ,
			NE,
			LOGICAL_AND,
			LOGICAL_OR
		} operator_;

		enum Type {
			UNKNOWN,
			OPERATOR,
			NUMBER,
			VARIABLE
		} type_;

		void set(const QString &data, Operator oper, Type type) {
			data_     = data;
			operator_ = oper;
			type_     = type;
		}
	};

private:
	T evaluate_expression() {
		T result;

		get_token();
		eval_exp(result);

		return result;
	}

public:
	T evaluate_expression(bool &ok, ExpressionError &error) throw() {
		try {
			ok = true;
			return evaluate_expression();
		} catch(const ExpressionError &e) {
			ok = false;
			error = e;
			return T();
		}
	}
private:
	void eval_exp(T &result);
	void eval_exp0(T &result);
	void eval_exp1(T &result);
	void eval_exp2(T &result);
	void eval_exp3(T &result);
	void eval_exp4(T &result);
	void eval_exp5(T &result);
	void eval_exp6(T &result);
	void eval_exp7(T &result);
	void eval_atom(T &result);
	void get_token();

	static bool is_delim(QChar ch) {
		return QString("[]!()=+-*/%&|^~<>\t\n\r ").contains(ch);
	}

private:
	QString                 expression_;
	QString::const_iterator expression_ptr_;
	Token                   token_;
	variable_getter_t       variable_reader_;
	memory_reader_t         memory_reader_;
};

#include "Expression.tcc"

#endif

