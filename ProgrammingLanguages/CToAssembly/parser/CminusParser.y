/*******************************************************/
/*                     Cminus Parser                   */
/*                                                     */
/*******************************************************/

/*********************DEFINITIONS***********************/
%{
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <util/general.h>
#include <util/symtab.h>
#include <util/symtab_stack.h>
#include <util/dlink.h>
#include <util/string_utils.h>

#define SYMTABLE_SIZE 100
#define SYMTAB_VALUE_FIELD     "value"
#define SYMTAB_NAME_FIELD "name"

/*********************EXTERNAL DECLARATIONS***********************/

EXTERN(void,Cminus_error,(const char*));

EXTERN(int,Cminus_lex,(void));

EXTERN(void, freeIntegerRegister,(int reg));
EXTERN(int, getFreeIntegerRegisterIndex, (SymTable symtab));
EXTERN(void, initRegisters, ());

char *fileName;
char buf[100];

struct DLinkList_struct *instList;
struct DLinkList_struct *dataList;
SymTable symtab;
SymTable regSymtab;

int labelNum = 0;
int stringNum = 0;
int gOffset = 0;
int getValue(int index);
int setValue(int index, int value);
char* getName(int index);

extern int Cminus_lineno;

extern FILE *Cminus_in;
%}

%name-prefix="Cminus_"
%defines

%start Program

%token AND
%token ELSE
%token EXIT
%token FOR
%token IF
%token INTEGER
%token NOT
%token OR
%token READ
%token WHILE
%token WRITE
%token LBRACE
%token RBRACE
%token LE
%token LT
%token GE
%token GT
%token EQ
%token NE
%token ASSIGN
%token COMMA
%token SEMICOLON
%token LBRACKET
%token RBRACKET
%token LPAREN
%token RPAREN
%token PLUS
%token TIMES
%token IDENTIFIER
%token DIVIDE
%token RETURN
%token STRING
%token INTCON
%token MINUS

%left OR
%left AND
%left NOT
%left LT LE GT GE NE EQ
%left PLUS MINUS
%left TIMES DIVDE

/***********************PRODUCTIONS****************************/
%%
   Program		: Procedures
		{
			printf("\t.data\n");
			printf(".newline: .asciiz \"\\n\"\n");
			DNode current = dataList->head;
			while(current != NULL){
				printf("%s", current->atom);
				current = current->next;
			}
			printf("\t.text\n");
			printf("\t.globl main\n");
			printf("main:   nop\n");
			current = instList->head;
                        while(current != NULL){
                                printf("%s", current->atom);
                                current = current->next;
                        }
			//printf("<Program> -> <Procedures>\n");
		}
	  	| DeclList Procedures
		{
			printf("\t.data\n");
                        printf(".newline: .asciiz \"\\n\"\n");
                        DNode current = dataList->head;
                        while(current != NULL){
                                printf("%s", current->atom);
                                current = current->next;
                        }
                        printf("\t.text\n");
                        printf("\t.globl main\n");
                        printf("main:   nop\n");
                        current = instList->head;
                        while(current != NULL){
                                printf("%s", current->atom);
                                current = current->next;
                        }
			//printf("<Program> -> <DeclList> <Procedures>\n");
		}
          ;

Procedures 	: ProcedureDecl Procedures
		{
			//printf("<Procedures> -> <ProcedureDecl> <Procedures>\n");
		}
	   	|
		{
			//printf("<Procedures> -> epsilon\n");
		}
	   	;

ProcedureDecl : ProcedureHead ProcedureBody
		{
			//printf("<ProcedureDecl> -> <ProcedureHead> <ProcedureBody>\n");
		}
              ;

ProcedureHead : FunctionDecl DeclList
		{
			//printf("<ProcedureHead> -> <FunctionDecl> <DeclList>\n");
		}
	      | FunctionDecl
		{
			//printf("<ProcedureHead> -> <FunctionDecl>\n");
		}
              ;

FunctionDecl :  Type IDENTIFIER LPAREN RPAREN LBRACE
		{
			//printf("<FunctionDecl> ->  <Type> <IDENTIFIER> <LP> <RP> <LBR>\n"); 
		}
	      	;

ProcedureBody : StatementList RBRACE
		{
			//printf("<ProcedureBody> -> <StatementList> <RBR>\n");
		}
	      ;


DeclList 	: Type IdentifierList  SEMICOLON 
		{
			//printf("<DeclList> -> <Type> <IdentifierList> <SC>\n");
		}
	   	| DeclList Type IdentifierList SEMICOLON
	 	{
			//printf("<DeclList> -> <DeclList> <Type> <IdentifierList> <SC>\n");
	 	}
          	;


IdentifierList 	: VarDecl  
		{
			//printf("<IdentifierList> -> <VarDecl>\n");
		}
                | IdentifierList COMMA VarDecl
		{
			//printf("<IdentifierList> -> <IdentifierList> <CM> <VarDecl>\n");
		}
                ;

VarDecl 	: IDENTIFIER
		{ 
			setValue($1, gOffset);
			gOffset = gOffset + 4;
                        $$ = $1;
			//printf("<VarDecl> -> <IDENTIFIER\n");
		}
		| IDENTIFIER LBRACKET INTCON RBRACKET
                {
			int con = $3;
			setValue($1, gOffset);
                        gOffset = gOffset + (4 * con);
                        $$ = $1;
			//printf("<VarDecl> -> <IDENTIFIER> <LBK> <INTCON> <RBK>\n");
		}
		;

Type     	: INTEGER 
		{ 
			//printf("<Type> -> <INTEGER>\n");
		}
                ;

Statement 	: Assignment
		{ 
			//printf("<Statement> -> <Assignment>\n");
		}
                | IfStatement
		{ 
			//printf("<Statement> -> <IfStatement>\n");
		}
		| WhileStatement
		{ 
			//printf("<Statement> -> <WhileStatement>\n");
		}
                | IOStatement 
		{ 
			//printf("<Statement> -> <IOStatement>\n");
		}
		| ReturnStatement
		{ 
			//printf("<Statement> -> <ReturnStatement>\n");
		}
		| ExitStatement	
		{ 
			//printf("<Statement> -> <ExitStatement>\n");
		}
		| CompoundStatement
		{ 
			//printf("<Statement> -> <CompoundStatement>\n");
		}
                ;

Assignment      : Variable ASSIGN Expr SEMICOLON
		{
			//int reg_result = getFreeIntegerRegisterIndex(regSymtab);
			int reg1 = $1;
			int reg2 = $3;
			char *inst = nssave(5, "\tsw ", getName(reg2), ", 0(", getName(reg1), ")\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			freeIntegerRegister(reg1);
			freeIntegerRegister(reg2);
			//freeIntegerRegister(reg_result);
			//printf("<Assignment> -> <Variable> <ASSIGN> <Expr> <SC>\n");
		}
                ;

IfStatement	: IF TestAndThen ELSE CompoundStatement
		{
			sprintf(buf, "%d", (int) $2);
                        char *inst = nssave(3, ".L", buf, ":    nop\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			//printf("<IfStatement> -> <IF> <TestAndThen> <ELSE> <CompoundStatement>\n");
		}
		| IF TestAndThen
		{
			sprintf(buf, "%d", (int) $2);
                        char *inst = nssave(3, ".L", buf, ":    nop\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			//printf("<IfStatement> -> <IF> <TestAndThen>\n");
		}
		;

TestAndThen	: Test CompoundStatement
		{
			sprintf(buf, "%d", labelNum);
                        char *inst = nssave(3, "\tj .L", buf, "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			sprintf(buf, "%d", (int) $1);
                        inst = nssave(3, ".L", buf, ":    nop\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			$$ = labelNum;
			labelNum = labelNum + 1;
			//printf("<TestAndThen> -> <Test> <CompoundStatement>\n");
		}
		;
				
Test		: LPAREN Expr RPAREN
		{
			int reg1 = $2;
			sprintf(buf, "%d", labelNum);
			char *inst = nssave(5, "\tbeq ", getName(reg1), ", $zero, .L", buf, "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			freeIntegerRegister(reg1);
			$$ = labelNum;
			labelNum = labelNum + 1;
			//printf("<Test> -> <LP> <Expr> <RP>\n");
		}
		;
	

WhileStatement  : WhileToken WhileExpr Statement
		{
			sprintf(buf, "%d", (int) $1);
			char *inst = nssave(3, "\tj .L", buf, "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        sprintf(buf, "%d", (int) $2);
                        inst = nssave(3, ".L", buf, ":    nop\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			//printf("<WhileStatement> -> <WhileToken> <WhileExpr> <Statement>\n");
		}
                ;
                
WhileExpr	: LPAREN Expr RPAREN
		{
			int reg1 = $2;
			sprintf(buf, "%d", labelNum);
                        char *inst = nssave(5, "\tbeq ", getName(reg1), ", $zero, .L", buf,"\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			freeIntegerRegister(reg1);
			$$ = labelNum;
			labelNum = labelNum + 1;
			//printf("<WhileExpr> -> <LP> <Expr> <RP>\n");
		}
		;
				
WhileToken	: WHILE
		{
			sprintf(buf, "%d", labelNum);
                        char *inst = nssave(3, ".L", buf, ":    nop\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			$$ = labelNum;
			labelNum = labelNum + 1;
			//printf("<WhileToken> -> <WHILE>\n");
		}
		;


IOStatement     : READ LPAREN Variable RPAREN SEMICOLON
		{
		  	char *inst = nssave(4, "\t", "li $v0, 5\n", "\t", "syscall\n");
		  	dlinkAppend(instList, dlinkNodeAlloc(inst));

		  	int reg1 = $3;
                  	inst = nssave(5, "\tsw ", "$v0", ", 0(", getName(reg1), ")\n");
                  	dlinkAppend(instList, dlinkNodeAlloc(inst));
                  	freeIntegerRegister(reg1);
		     //printf("<IOStatement> -> <READ> <LP> <Variable> <RP> <SC>\n");
		}
                | WRITE LPAREN Expr RPAREN SEMICOLON
		{
			char *inst = nssave(3, "\tmove $a0, ", getName($3), "\n\tli $v0, 1\n\tsyscall\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			freeIntegerRegister($3);
			inst = nssave(1, "\tli $v0, 4\n\tla $a0, .newline\n\tsyscall\n");
			dlinkAppend(instList, dlinkNodeAlloc(inst));
			//printf("%d\n", $3);
			//printf("<IOStatement> -> <WRITE> <LP> <Expr> <RP> <SC>\n");
		}
                | WRITE LPAREN StringConstant RPAREN SEMICOLON         
		{
			sprintf(buf, "%d", $3);
			char *inst = nssave(8, "\t", "la $a0, .string", buf, "\n", "\t", "li $v0, 4\n", "\t", "syscall\n");
                  	dlinkAppend(instList, dlinkNodeAlloc(inst));
			inst = nssave(1, "\tli $v0, 4\n\tla $a0, .newline\n\tsyscall\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
		   	//printf("%s\n", (char *)SymGetFieldByIndex(symtab,$3, SYM_NAME_FIELD));
			//printf("<IOStatement> -> <WRITE> <LP> <StringConstant> <RP> <SC>\n");
		}
                ;

ReturnStatement : RETURN Expr SEMICOLON
		{
			//printf("<ReturnStatement> -> <RETURN> <Expr> <SC>\n");
		}
                ;

ExitStatement 	: EXIT SEMICOLON
		{
			sprintf(buf, "%d", 10);
                        char *inst = nssave(3, "\tli $v0, ", buf, "\n\tsyscall\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			//printf("<ExitStatement> -> <EXIT> <SC>\n");
		}
		;

CompoundStatement : LBRACE StatementList RBRACE
		{
			//printf("<CompoundStatement> -> <LBR> <StatementList> <RBR>\n");
		}
                ;

StatementList   : Statement
		{		
			//printf("<StatementList> -> <Statement>\n");
		}
                | StatementList Statement
		{		
			//printf("<StatementList> -> <StatementList> <Statement>\n");
		}
                ;

Expr            : SimpleExpr
		{
			$$ = $1;
			//printf("<Expr> -> <SimpleExpr>\n");
		}
                | Expr OR SimpleExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "or ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
		        //$$ = $1 | $3;
			//printf("<Expr> -> <Expr> <OR> <SimpleExpr> \n");
		}
                | Expr AND SimpleExpr 
		{
			//$$ = $1 & $3;
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
			int reg1 = $1;
			int reg2 = $3;
			char *inst = nssave(8, "\t", "and ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
			dlinkAppend(instList, dlinkNodeAlloc(inst));
			freeIntegerRegister(reg1);
			freeIntegerRegister(reg2);
			$$ = reg_result;
			//printf("<Expr> -> <Expr> <AND> <SimpleExpr> \n");
		}
                | NOT SimpleExpr 
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $2;
			char *inst = nssave(8, "\t", "nor ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg1), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			freeIntegerRegister(reg1);
			$$ = $2;
			//$$ = $2 ^ 1;
			//printf("<Expr> -> <NOT> <SimpleExpr> \n");
		}
                ;

SimpleExpr	: AddExpr
		{
			$$ = $1;
			//printf("<SimpleExpr> -> <AddExpr>\n");
		}
                | SimpleExpr EQ AddExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
			char *inst = nssave(8, "\t", "seq ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
		        //$$ = ($1 == $3);
			//printf("<SimpleExpr> -> <SimpleExpr> <EQ> <AddExpr> \n");
		}
                | SimpleExpr NE AddExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg_temp = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "sne ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
		        //$$ = ($1 != $3);
			//printf("<SimpleExpr> -> <SimpleExpr> <NE> <AddExpr> \n");
		}
                | SimpleExpr LE AddExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "sle ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
		        //$$ = ($1 <= $3);
			//printf("<SimpleExpr> -> <SimpleExpr> <LE> <AddExpr> \n");
		}
                | SimpleExpr LT AddExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "slt ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
		        //$$ = ($1 < $3);
			//printf("<SimpleExpr> -> <SimpleExpr> <LT> <AddExpr> \n");
		}
                | SimpleExpr GE AddExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "sge ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
		        //$$ = ($1 >= $3);
			//printf("<SimpleExpr> -> <SimpleExpr> <GE> <AddExpr> \n");
		}
                | SimpleExpr GT AddExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
			char *inst = nssave(8, "\t", "sgt ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
			//printf("<SimpleExpr> -> <SimpleExpr> <GT> <AddExpr> \n");
		        //$$ = ($1 > $3);
		}
                ;

AddExpr		:  MulExpr            
		{
			$$ = $1;
			//printf("<AddExpr> -> <MulExpr>\n");
		}
                |  AddExpr PLUS MulExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "add ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
			//$$ = $1 + $3;
			//printf("<AddExpr> -> <AddExpr> <PLUS> <MulExpr> \n");
		}
                |  AddExpr MINUS MulExpr
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "sub ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
			//$$ = $1 - $3;
			//printf("<AddExpr> -> <AddExpr> <MINUS> <MulExpr> \n");
		}
                ;

MulExpr		:  Factor
		{
			$$ = $1;
			//printf("<MulExpr> -> <Factor>\n");
		}
                |  MulExpr TIMES Factor
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "mul ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
			//$$ = $1 * $3;
			//printf("<MulExpr> -> <MulExpr> <TIMES> <Factor> \n");
		}
                |  MulExpr DIVIDE Factor
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        int reg1 = $1;
                        int reg2 = $3;
                        char *inst = nssave(8, "\t", "div ", getName(reg_result), ", ", getName(reg1), ", ", getName(reg2), "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        freeIntegerRegister(reg1);
                        freeIntegerRegister(reg2);
                        $$ = reg_result;
			//$$ = $1 / $3;
			//printf("<MulExpr> -> <MulExpr> <DIVIDE> <Factor> \n");
		}
                ;

Factor          : Variable
		{
			int reg1 = $1;
			char *inst = nssave(5, "\tlw ", getName(reg1), ", 0(", getName(reg1), ")\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			$$ = reg1;
			//printf("<Factor> -> <Variable>\n");
		}
                | Constant
		{ 
			sprintf(buf, "%d", $1);
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        char *inst = nssave(5, "\tli ", getName(reg_result), ", ", buf, "\n");
			dlinkAppend(instList, dlinkNodeAlloc(inst));
			$$ = reg_result;
			//printf("<Factor> -> <Constant>\n");
		}
                | IDENTIFIER LPAREN RPAREN
       		{	
			//printf("<Factor> -> <IDENTIFIER> <LP> <RP>\n");
		}
         	| LPAREN Expr RPAREN
		{
			$$ = $2;
			//printf("<Factor> -> <LP> <Expr> <RP>\n");
		}
                ;  

Variable        : IDENTIFIER
		{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
                        sprintf(buf, "%d", (int) getValue($1));
                        char *inst = nssave(5, "\tadd ", getName(reg_result), ", $gp, ", buf, "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        $$ = reg_result;
			//printf("<Variable> -> <IDENTIFIER>\n");
		}
                | IDENTIFIER LBRACKET Expr RBRACKET    
               	{
			int reg_result = getFreeIntegerRegisterIndex(regSymtab);
			int reg1 = $3;
                        char *inst;
                        sprintf(buf, "%d", (int) getValue($1));
			inst = nssave(6, "\tmul ", getName(reg_result), ", ", getName(reg1), ", 4", "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
                        inst = nssave(6, "\tadd ", getName(reg_result), ", ", getName(reg_result), ", $gp", "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			inst = nssave(7, "\tadd ", getName(reg_result), ", ", getName(reg_result), ", ", buf, "\n");
                        dlinkAppend(instList, dlinkNodeAlloc(inst));
			freeIntegerRegister(reg1);
                        $$ = reg_result;
			//printf("<Variable> -> <IDENTIFIER> <LBK> <Expr> <RBK>\n");
               	}
                ;			       

StringConstant 	: STRING
		{
			sprintf(buf, "%d", stringNum);
			char *data = nssave(5, ".string", buf, ": .asciiz \"", SymGetFieldByIndex(symtab, $1, SYMTAB_NAME_FIELD), "\"\n");
			dlinkAppend(dataList, dlinkNodeAlloc(data));
		        $$ = stringNum;
			stringNum++;
			//printf("<StringConstant> -> <STRING>\n");
		}
                ;

Constant        : INTCON
		{
			$$ = $1;
			//printf("<Constant> -> <INTCON>\n");
		}
                ;

%%


/********************C ROUTINES *********************************/

void Cminus_error(const char *s)
{
  fprintf(stderr,"%s: line %d: %s\n",fileName,Cminus_lineno,s);
}

int Cminus_wrap() {
	return 1;
}

static void initialize(char* inputFileName) {

	Cminus_in = fopen(inputFileName,"r");
        if (Cminus_in == NULL) {
          fprintf(stderr,"Error: Could not open file %s\n",inputFileName);
          exit(-1);
        }

	char* dotChar = rindex(inputFileName,'.');
	int endIndex = strlen(inputFileName) - strlen(dotChar);
	char *outputFileName = nssave(2,substr(inputFileName,0,endIndex),".s");
	stdout = freopen(outputFileName,"w",stdout);
        if (stdout == NULL) {
          fprintf(stderr,"Error: Could not open file %s\n",outputFileName);
          exit(-1);
        }

	 symtab = SymInit(SYMTABLE_SIZE);
	 SymInitField(symtab,SYMTAB_VALUE_FIELD,(Generic)-1,NULL);

	 regSymtab = SymInit(SYMTABLE_SIZE);
         SymInitField(regSymtab,SYMTAB_VALUE_FIELD,(Generic)-1,NULL);

	 instList = dlinkListAlloc(NULL);
	 dataList = dlinkListAlloc(NULL);
	 initRegisters();
}

static void finalize() {

    SymKillField(symtab,SYMTAB_VALUE_FIELD);
    SymKill(symtab);
    SymKillField(regSymtab,SYMTAB_VALUE_FIELD);
    SymKill(regSymtab);
    fclose(Cminus_in);
    fclose(stdout);

}

int main(int argc, char** argv)

{	
	fileName = argv[1];
	initialize(fileName);
	
        Cminus_parse();
  
  	finalize();
  
  	return 0;
}

int getValue(int index)
{
  return (int)SymGetFieldByIndex(symtab, index, SYMTAB_VALUE_FIELD); 
}

int setValue(int index, int value)
{
  SymPutFieldByIndex(symtab, index, SYMTAB_VALUE_FIELD, (Generic)value); 
}

char* getName(int index)
{
  return (char *)SymGetFieldByIndex(regSymtab,index, SYM_NAME_FIELD);
}

/******************END OF C ROUTINES**********************/
