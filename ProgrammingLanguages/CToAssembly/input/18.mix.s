<Type> -> <INTEGER>
<VarDecl> -> <IDENTIFIER
<IdentifierList> -> <VarDecl>
<VarDecl> -> <IDENTIFIER
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<VarDecl> -> <IDENTIFIER
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<VarDecl> -> <IDENTIFIER
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<VarDecl> -> <IDENTIFIER
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<VarDecl> -> <IDENTIFIER
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<DeclList> -> <Type> <IdentifierList> <SC>
<Type> -> <INTEGER>
<FunctionDecl> ->  <Type> <IDENTIFIER> <LP> <RP> <LBR>
<ProcedureHead> -> <FunctionDecl>
<StringConstant> -> <STRING>
<IOStatement> -> <WRITE> <LP> <StringConstant> <RP> <SC>
"i = "
<Statement> -> <IOStatement>
<StatementList> -> <Statement>
<Variable> -> <IDENTIFIER>
<IOStatement> -> <READ> <LP> <Variable> <RP> <SC>
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<StringConstant> -> <STRING>
<IOStatement> -> <WRITE> <LP> <StringConstant> <RP> <SC>
"j = "
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<IOStatement> -> <READ> <LP> <Variable> <RP> <SC>
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<StringConstant> -> <STRING>
<IOStatement> -> <WRITE> <LP> <StringConstant> <RP> <SC>
"k = "
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<IOStatement> -> <READ> <LP> <Variable> <RP> <SC>
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<StringConstant> -> <STRING>
<IOStatement> -> <WRITE> <LP> <StringConstant> <RP> <SC>
"l = "
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<IOStatement> -> <READ> <LP> <Variable> <RP> <SC>
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<StringConstant> -> <STRING>
<IOStatement> -> <WRITE> <LP> <StringConstant> <RP> <SC>
"m = "
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<IOStatement> -> <READ> <LP> <Variable> <RP> <SC>
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <AddExpr> <PLUS> <MulExpr> 
<SimpleExpr> -> <SimpleExpr> <LT> <AddExpr> 
<Expr> -> <SimpleExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <SimpleExpr> <NE> <AddExpr> 
<Expr> -> <SimpleExpr>
<Factor> -> <LP> <Expr> <RP>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Expr> -> <NOT> <SimpleExpr> 
<Factor> -> <LP> <Expr> <RP>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Expr> -> <Expr> <AND> <SimpleExpr> 
<Assignment> -> <Variable> <ASSIGN> <Expr> <SC>
<Statement> -> <Assignment>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Expr> -> <SimpleExpr>
<IOStatement> -> <WRITE> <LP> <Expr> <RP> <SC>
0
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <MulExpr> <TIMES> <Factor> 
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <MulExpr> <TIMES> <Factor> 
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <SimpleExpr> <GT> <AddExpr> 
<Expr> -> <SimpleExpr>
<Constant> -> <INTCON>
<Factor> -> <Constant>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <SimpleExpr> <LE> <AddExpr> 
<Expr> -> <Expr> <OR> <SimpleExpr> 
<IOStatement> -> <WRITE> <LP> <Expr> <RP> <SC>
1
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <MulExpr> <TIMES> <Factor> 
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <AddExpr> <PLUS> <MulExpr> 
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <AddExpr> <PLUS> <MulExpr> 
<SimpleExpr> -> <SimpleExpr> <GT> <AddExpr> 
<Expr> -> <SimpleExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <SimpleExpr> <NE> <AddExpr> 
<Expr> -> <Expr> <AND> <SimpleExpr> 
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <SimpleExpr> <GT> <AddExpr> 
<Expr> -> <Expr> <OR> <SimpleExpr> 
<IOStatement> -> <WRITE> <LP> <Expr> <RP> <SC>
1
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <MulExpr> <DIVIDE> <Factor> 
<AddExpr> -> <MulExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <AddExpr> <MINUS> <MulExpr> 
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<SimpleExpr> -> <SimpleExpr> <GT> <AddExpr> 
<Expr> -> <SimpleExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <MulExpr> <TIMES> <Factor> 
<AddExpr> -> <AddExpr> <PLUS> <MulExpr> 
<SimpleExpr> -> <AddExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<AddExpr> -> <MulExpr>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <Factor>
<Variable> -> <IDENTIFIER>
<Factor> -> <Variable>
<MulExpr> -> <MulExpr> <TIMES> <Factor> 
<AddExpr> -> <AddExpr> <PLUS> <MulExpr> 
<SimpleExpr> -> <SimpleExpr> <NE> <AddExpr> 
<Expr> -> <Expr> <AND> <SimpleExpr> 
<IOStatement> -> <WRITE> <LP> <Expr> <RP> <SC>
0
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<ProcedureBody> -> <StatementList> <RBR>
<ProcedureDecl> -> <ProcedureHead> <ProcedureBody>
<Procedures> -> epsilon
<Procedures> -> <ProcedureDecl> <Procedures>
<Program> -> <DeclList> <Procedures>
