
/*
Example:
https://atcoder.jp/contests/joi2020yo2/tasks/joi2020_yo2_e
<expression> ::= <term> | <expression> "+" <term> | <expression> "-" <term>
<term> ::= <factor> | <term> "*" <factor>
<factor> ::= "R" | "S" | "P" | "?" | "(" <expression> ")"

*/
typedef string::const_iterator State;
struct val{
    // something
    mint R,P,S;
	val operator+(const val &a) const{
		return{R*a.S+S*a.R+R*a.R,P*a.R+R*a.P+P*a.P,S*a.P+P*a.S+S*a.S};
	}
	val operator-(const val &a) const{
		return{R*a.R+P*a.R+R*a.P,P*a.P+P*a.S+S*a.P,S*a.S+S*a.R+R*a.S};
	}
	val operator*(const val &a) const{
		return{R*a.R+P*a.S+S*a.P,P*a.P+S*a.R+R*a.S,S*a.S+R*a.P+P*a.R};
	}
};


val expr(State &begin),term(State &begin),factor(State &begin);

val expr(State &begin){
	val ret=term(begin);
	while(1){
		if(*begin=='+'){
			begin++;
			ret=ret+term(begin);
		}
		else if(*begin=='-'){
			begin++;
			ret=ret-term(begin);
		}
		else{
			break;
		}
	}
	return ret;
}
val term(State &begin){
	val ret=factor(begin);
	while(1){
		if(*begin=='*'){
			begin++;
			ret=ret*factor(begin);
		}
		else break;
	}
	return ret;
}
val factor(State &begin){
	val ret={0,0,0};
	if(*begin=='('){
		begin++;
		ret=expr(begin);
		begin++;
	}
	else{
		if(*begin=='R')ret.R++;
		if(*begin=='S')ret.S++;
		if(*begin=='P')ret.P++;
		if(*begin=='?'){
			ret.R++;ret.S++;ret.P++;
		}
		begin++;
	}

	return ret;
}

void ll1_parser(string E){
    State B=E.begin();
    val ans=expr(B);
}