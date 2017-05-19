%{
#define IF 5
#define ID 12
#define INTEGER 13
#define REAL 14
%}
letter 		[A-Za-z]
digit 		[0-9]
id 			{letter}({letter}|{digit})*
integer  	{digit}+
%%
{if}		{return IF;}
{id}		{yyval=install_id();return ID;}
{integer}	{yyval=convert_int();return INTEGER;}
%%

install_id(){
	
}