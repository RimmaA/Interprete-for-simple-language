#include <string>
#include <cctype>
#include <iostream>
#include <map>
#include <strstream>
#include <cstdlib>
#include <limits>
#include <algorithm>

using namespace std;

/*******************************Tokenizer**************************************/

// ------------Function Prototypes----------------------------------------------
void DFA_push(char); // Calls push() after performing certain validations
void DFA_pop(char); // Calls pop() after performing certain validations.
void process_string(string);
string transition(int, char); // return a value from the PDA Table
int return_col(char); //will return which column in the PDA table to go to.
bool status(); // Prints whether string is, Accepted or Rejected or Crashed
void push(char); // Pushes a CHAR into the stack
char pop(); // Simply pops the top most CHAR and returns the value
//------------------------------------------------------------------------------
struct stack{
       char ch;
       struct stack * top;
};
struct stack * head=NULL;

//Subsets of the Alphabet
char E_op[2]={'+','-'};
char E_ch[53]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char E_n[9]={'1','2','3','4','5','6','7','8','9'}; //1-9

const int accept_state=0;
int current_state=0;
bool state_crash=0;
/*---------------------TRANSITION TABLE-----------------------------------------
This is the TRANSITION Function of the Push-Down-Automota(PDA)
-NOTE: The symbol '~' means Epsilon
       -Rows are [States], Columns are subset of [symbols]
-A value in the table is an Instruction in the form [Move][Pop][Push]
*/
string PDA[9][9]={
//(Column) E_op  E_n    E_ch   (       )      ;     =      *      0     (ROW)
         {"5~~","5~~", "1~~", "5~~", "5~~", "5~~", "5~~", "5~~", "5~~"}, //q0
         {"5~~","1~~", "1~~", "5~~", "5~~", "5~~", "2~~", "5~~", "1~~"}, //q1
         {"2~~","7~~", "4~~", "2~(", "5~~", "5~~", "5~~", "5~~", "6~~"}, //q2
         {"3~~","7~~", "4~~", "2~(", "8(~", "5~~", "5~~", "5~~", "6~~"}, //q3
         {"3~~","4~~", "4~~", "5~~", "8(~", "0~~", "5~~", "3~~", "4~~"}, //q4
         {"5~~","5~~", "5~~", "5~~", "5~~", "5~~", "5~~", "5~~", "5~~"}, //q5
         {"5~~","5~~", "5~~", "5~~", "8(~", "0~~", "5~~", "3~~", "5~~"}, //q6
         {"3~~","7~~", "5~~", "5~~", "8(~", "0~~", "5~~", "3~~", "7~~"}, //q7
         {"3~~","5~~", "5~~", "5~~", "8(~", "0~~", "5~~", "3~~", "5~~"}  //q8
};
//----------------------------------Tokenizer()---------------------------------

bool Tokenizer(string word)
{
    int current_state=0;
    int state_crash=0; //resetting values
    process_string(word); // Process the String that was entered.
    //checks that there is no '(' on a stack and if exists it cleans stack for next iteration
    if(head!=NULL){
        while(head!=NULL)
            char ch=pop();
        return false;
        }
    else
        return status();
}

//----------------------------------process_string()-------------------------------

void process_string(string word){
    /* This function will read the input string CHAR by CHAR,
            - Process the instruction (MOVE-POP-PUSH)
    */
    string instruction; //MOVE-POP-PUSH
    current_state=0;
    for (int i=0; i<word.length(); i++){
        if (state_crash==1){
            break;
        }
        instruction=transition(current_state, word[i]);
        current_state=atoi(&instruction[0]);
        DFA_pop(instruction[1]);
        DFA_push(instruction[2]);
    }
}

//-----------------------------------transition()---------------------------------------

string transition(int current_state, char c){
/*
Returns an instruction (move-pop-push) fetched from the PDA Table.
*/
    return  PDA[current_state][return_col(c)];
}

//---------------------------------return_col()-----------------------------------------

int return_col(char c){
/*
This function will return the INDEX of the column a Symbol(Char) belongs to.
*/
       int i=0;

       if (c=='(' ) return 3;
       if (c==')' ) return 4;
       if (c==';' ) return 5;
       if (c=='=' ) return 6;
       if (c=='*' ) return 7;
       if (c=='0' ) return 8;

       for (i=0; i<2; i++){
           if (c==E_op[i]) return 0;
       }

       for (i=0; i<53; i++){
           if (c==E_ch[i]) return 2;
       }

       for (i=0; i<9; i++){
           if (c==E_n[i])return 1;
       }

       return 9;
}

//-----------------------------------DFA_pop()---------------------------------

void DFA_pop(char c){

     char ch;
     if (c=='~') return; // If (Epsilon), do nothing
     ch=pop();
     if (ch!=c)  state_crash=1;//If the popped symbol doesn't match, PDA Crashes
}

//------------------------------------DFA_push()---------------------------------

void DFA_push(char c){
     // Push a symbol only if symbol is not (Epsilon).
     if (c != '~') push(c);
}

//------------------------------------push()-------------------------------------
//this function push '(' into stack

void push(char c){
     struct stack * node;
     struct stack * current=head;
     node=new struct stack;
     node->ch=c;
     node->top=NULL;

     if (head==NULL){
         head=node;
     }else{
         while(current->top){
         current=current->top;
         }
         current->top=node;
     }
}

//-------------------------------------pop()--------------------------------------
//this function pop '(' from stack

char pop(){
     struct stack * current=head;
     struct stack * current_prev=head;
     char ch;

     if (head==NULL){
         state_crash=1;
         return '%';
     }else if (head->top==NULL){
         ch=head->ch;
         head=NULL;
         return ch;
     }else{
         while(current->top){
             current=current->top;
             if (current->top){
                 current_prev=current_prev->top;
             }
         }
         ch=current->ch;
         current_prev->top=NULL;
         return ch;
     }
}

//----------------------------------status()--------------------------------------

//This function returns true if input is valid and returns false if not
bool status(){
     if (state_crash==1){
         return false;
     }else if (current_state==accept_state){
         return true;
     }else{
         return false;
     }
}


// Pointer to input string
istream* input;

//-----------------------------------error()---------------------------------------

// Function to output error
double error(const char* s) {
    cerr << "Error!" << s << '\n';
    return 0.00;
}

// Different tokens
enum Tokens {
	NAME,		NUMBER,		END,    CONT,
	PLUS='+',	MINUS='-',	MUL='*',
    ASSIGN='=',	LP='(',		RP=')'
};

Tokens currentToken = END;
double numbers;
string strings;
// Create a map type consisting of string and double values
map <string, double> table;

int check=0;

/***********************************Assignment**************************************/

//----------------------------------getToken----------------------------------------

Tokens getToken() {
	char ch;
    do {
		if(!input->get(ch)) return currentToken = END;
	} while (ch!='\n' && isspace(ch));
	// Switch each character
	switch (ch) {
        // Print
       	case ';':
        case '\n':
            return currentToken = CONT;
        // Operators
    	case '*':
    	case '+':
    	case '-':
    	case '(':
    	case ')':
            return currentToken=Tokens(ch);
        case '=':
            check = 1;
    		return currentToken=Tokens(ch);
	    // Numbers
    	case '0': case '1': case '2': case '3': case '4':
    	case '5': case '6': case '7': case '8': case '9':
            input->putback(ch);
            *input >> numbers;
            return currentToken=NUMBER;
        // Variable declaration
       	default:			// NAME
    		if (isalpha(ch)) { // If ch is alphabetic
    			strings = ch;
                while (input->get(ch) && isalnum(ch))
    				strings += ch;
                //checks if current input has uninitialized variables
                for(map<string, double >::const_iterator it = table.begin(); it != table.end(); ++it)
                    if(it->first==strings and check==1){
                        input->putback(ch);
                        return currentToken=NAME;
                    }
                //if input doesn't have variables
                if(check==0){
                    input->putback(ch);
                    return currentToken=NAME;
                }
                else{
                    //if input has uninitialized variable
                    cout <<endl<< "Uninitialized variables " << strings << endl;
                    cout<< "To continue enter 'c'!" << endl<< endl;
                    input->clear();
                    //clean the rest of 'bad' input
                    input->ignore(std::numeric_limits<std::streamsize>::max(), 'c');
                    input->get(ch);  //get rid of last '/n'
                    cout<<endl<< "Enter new input: " <<endl<< endl;
                    return currentToken = CONT;
                }
        }
    }
}

// Expressions
double expr(bool);

//---------------------------------------prim()------------------------------------------

// Handle Primaries
double prim(bool get) {
	if (get) getToken();

	switch (currentToken) {
    	// Variables
    	case NAME:
    	{	double& v = table[strings];
    		if (getToken() == ASSIGN) v = expr(true);
    		return v;
    	}
    	// Unary Minus
    	case MINUS:
    		return -prim(true);
        // Unary Plus
    	case PLUS:
    		return +prim(true);
        // Left Parenthesis
    	case LP:
    	{	double e = expr(true);
    		if (currentToken != RP) return error(") expected");
    		getToken();		// eat ')'
    		return e;
    	}
    	case NUMBER:{
		        double v = numbers;
                getToken();
                return v;
            }
        }
}

//-------------------------------------term()---------------------------------

// Multiply
double term(bool get) {
	double left = prim(get);

	// Keep looping - that wont happen though as we HAVE to find a token
	for (;;) {
		switch (currentToken) {
    		case MUL:
    			left *= prim(true);
    			break;
   			default:
    			return left;
		}
    }
}

//------------------------------------expr()------------------------------------

// Add and subtract
double expr(bool get) {
	double left = term(get);

	// Keep looping - that wont happen though as we HAVE to find a token
	for (;;) {
		switch (currentToken) {
		// Plus
    		case PLUS:
    			left += term(true);
    			break;
		// Minus
    		case MINUS:
    			left -= term(true);
    			break;
    		default:
    			return left;
		}
    }
}

//-------------------------------------print()----------------------------------------

void print(){
    for(map<string, double >::const_iterator it = table.begin(); it != table.end(); ++it)
        cout << it->first << " = " << it->second << "\n";
}

/*********************************************main()*******************************/

int main() {
    cout << "Rimma Niyazova(Akhmetova)" << endl;
    cout << "Toy language implementation." << endl << endl;
    cout << "Enter your input:\nto quit and see total result inter 'q'\nto see current result enter 'p' \n" <<endl;
    input = &cin;
    string word;  //pass word in tokenizer to check if it is valid
    char ch;

    while(true){
        check=0;  //specify a new input
        //gets rid of spaces
        while (input->get(ch) && ch!='\n'){
            if(ch!=' ')
                word += ch;
        }
        //quits
        if(word=="q")
            break;
        //prints
        else if(word=="p"){
            cout<<endl;
            print();
            cout<<endl;
            word.clear();  //cleans word to put a new input inside
            cout << "Enter new input:\n"<<endl;
            continue;
        }
        //checks validity
        while(Tokenizer(word)!=true){
            //input is invalid, cleans word for a new input
            word.clear();
            cout << "\nIncorrect input!\nEnter new input:\nto quit and see total result inter 'q'\nto see current result enter 'p' \n" <<endl;
            while (input->get(ch) && ch!='\n'){
                if(ch!=' ')
                    word += ch;
            }
            if(word=="q" or word=="p")
                break;
        }
        if(word=="q")
            break;
        else if(word=="p"){
            cout<<endl;
            print();
            cout<<endl;
            word.clear();
            cout << "Enter new input:\n"<<endl;
            continue;
        }
        reverse(word.begin(), word.end());
        //put reverse input back into pointer
        for(int i=0; i<word.length();i++)
            input->putback(word[i]);
        //clears word for next input
        word.clear();
        //does calculations
        getToken();
        if (currentToken == END) break;
        if (currentToken == CONT) continue;
        //puts result into map table
        expr(false);
        }
        //prints result
        cout<< endl << endl<< "Total result:"<<endl;
        print();
        //deletes pointer
        if (input != &cin) delete input;
}
