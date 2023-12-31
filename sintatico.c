#include <stdio.h>
#include <stdbool.h>
#include "lexico.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

#define STMT 3
#define STMTLIST1 4
#define EXPS 5
#define BLOCK 6
#define FORBLOCK 7

Token token;
char *code;
bool erro=false;

size_t firsts_size_STMT = 10;
int firsts_STMT[] = {260, 
		'(',
		302,
		320,
		309,
		316,
		301,
		307,
		308,
		311
};



void Stmtlist1();
void fim_do_codigo();
void Block();
void Exp();
void NameList();

bool esta_no_conjunto(char function_type, unsigned int n_terminal) {
	if (function_type == 'p') {
		switch(n_terminal) {
			case STMT:
				for (size_t i=0; i<firsts_size_STMT;i++) {
					if (firsts_STMT[i] == token.nome_atributo)
						return true;
				}
				return false;
		}
	} else if (function_type == 's') {
		return false;
	} else {
		printf("Cagou no pau na chamada da função 'esta_no_conjunto'\n");
		exit(1);
	}
	return false; // NOTE: Só coloquei para essa merda de editor parar de ficar enchendo o saco
}


void Erro(char function_type, unsigned int n_terminal) {
	if (function_type == 'p') { // 'p' para 'primeiro' == 'first'
		switch (n_terminal) {
			case STMTLIST1:
				
				 // Pseudo-codigo:
				 // Comparamos token com os "Firsts" de Stmtlist1
				 // Se não bater com nenhum, chamamos token de novo
				 // Se bater, caimos fora da função com o token na agulha
				 
				size_t firsts_size = 10;
				int firsts_stmtlist1[] = {260, 
						'(',
						302,
						320,
						309,
						316,
						301,
						307,
						308,
						311
						};
				for (size_t i=0; i<firsts_size; i++) {
					if (token.nome_atributo == firsts_stmtlist1[i]) {
						return;
					}
				}
				
				
				token = proximo_token();
				fim_do_codigo();
				Erro('p', n_terminal);
				break;
			case EXPS:
				size_t firsts_exps_size = 11;
				int firsts_exps[] = { 313,
						      '-',
						      260,
						      '(',
						      308,
						      '{',
						      312,
						      318,
						      306,
						      261,
						      270

				};
				for (size_t i=0;i<firsts_exps_size;i++) {
					if (token.nome_atributo == firsts_exps[i]){
						return;
					}
				}

				token = proximo_token();
				fim_do_codigo();
				Erro('p', n_terminal);
				break;
			case BLOCK:
				size_t firsts_block_size = 10;
				int firsts_block[] = {260, 
						'(',
						302,
						320,
						309,
						316,
						301,
						307,
						308,
						311
				};

				for (size_t i=0;i<firsts_block_size;i++) {
					if (token.nome_atributo == firsts_block[i]){
						return;
					}
				}

				token = proximo_token();
				fim_do_codigo();
				Erro('p', n_terminal);
				break;

			case FORBLOCK:
				if (token.nome_atributo == '=' || token.nome_atributo == ',') {
					return;
				}

				token = proximo_token();
				fim_do_codigo();
				Erro('p', n_terminal);
				break;

		}
	} else if (function_type == 's'){ // 's' para 'sequencia' == 'follow'
		switch(n_terminal) {
			case STMT:
				if (token.nome_atributo == ';') {
					return;
				}

				token = proximo_token();
				fim_do_codigo();
				Erro('s', n_terminal);
				break;
		}
	

	} else {
		printf("Você cagou no pau na chamada da funçao 'Erro'");
		exit(1);
	}
}

void fim_do_codigo() {
	if (token.nome_atributo == -1){ 
		printf( RED "Unexpected End of File\n" RESET);
		exit(1); 
	}
}

void Var() {
	// Var -> name | PrefixExp [ Exp ]

	if (token.nome_atributo == 260) {
		token = proximo_token();
		fim_do_codigo();
	} else if (token.nome_atributo == '(') { 
		token = proximo_token();
		fim_do_codigo();
	}
}

void BinOp() {
	// BinOp → or | and | < | > | <= | >= | ~= | == | … | + | - | * | / | ^
	
	// Isso ficou bem redundante eu sei, mas pelo bem de seguir a gramática, por enquanto irá ficar assim
	if (token.nome_atributo == 259 || token.nome_atributo == 300 || token.nome_atributo == 314 || token.nome_atributo == 268 || token.nome_atributo == '+' || token.nome_atributo == '-' || token.nome_atributo == '*' || token.nome_atributo == '/' || token.nome_atributo == '^') {
		token = proximo_token();
		fim_do_codigo();
		return;
	} // else é por que ocorreu algum erro e teremos que recuperar 
}

void ExpBlock() { 
	// ExpBlock → BinOp Exp ExpBlock | ε
	

	if (token.nome_atributo == 259 || token.nome_atributo == 300 || token.nome_atributo == 314 || token.nome_atributo == 268 || token.nome_atributo == '+' || token.nome_atributo == '-' || token.nome_atributo == '*' || token.nome_atributo == '/' || token.nome_atributo == '^') {
		// Não consumimos o token da agulha pois estamos analisando os firsts
		BinOp();
	
		Exp();

		ExpBlock();
	} else {
		return;
	} 
}

void PeBlock() {
	// PeBlock → [ Exp ] PeBlock | ε
	if (token.nome_atributo == '[') {
		token = proximo_token();
		fim_do_codigo();

		Exp();
		if (token.nome_atributo == ']') {
			PeBlock();
		} // else aqui temos que colocar um erro
	} else {
		return;
	}
}

void PrefixExp() {
	// PrefixExp -> name PeBlock | ( Exp ) PeBlock
	
	bool gate = false;

	if (token.nome_atributo == 260) {
		token = proximo_token();
		fim_do_codigo();
		gate=true;
	} else if (token.nome_atributo == '(') {
		token = proximo_token();
		fim_do_codigo();
		Exp();
		if (token.nome_atributo == ')') {
			token = proximo_token();
			fim_do_codigo();
			gate=true;
		} // else é por que ocorreu algum erro e teremos que recuperar

	} // else é por que ocorreu algum erro e teremos que recuperar
	
	if (gate) {
		PeBlock();
	} 
}

void Field() {
	if (token.nome_atributo == '[') {
		// Field → [ Exp ] = Exp
		token = proximo_token();
		fim_do_codigo();

		Exp();

		if (token.nome_atributo == ']') {
			token = proximo_token();
			fim_do_codigo();

			if (token.nome_atributo == '=') {
				token = proximo_token();
				fim_do_codigo();

				Exp();
			} // else erro
		} // else erro

	} else if (token.nome_atributo == 260) {
		// Field → name = Exp
		token = proximo_token();
		fim_do_codigo();

		if (token.nome_atributo == '=') {
			token = proximo_token();
			fim_do_codigo();

			Exp();
		} // else erro
	}
}

void FieldRest() {
	// FieldRest → , Field FieldRest | ε
	if (token.nome_atributo == ',') {
		token = proximo_token();
		fim_do_codigo();

		Field();
		FieldRest();

	} else {
		return;
	}
}

void FieldList() {
	// FieldList → Field FieldRest | ε
	if (token.nome_atributo == '[' || token.nome_atributo == 260) {
		// Não consumimos o token da agulha por ser analise de first

		Field();
		FieldRest();

	} else {
		return;
	}

}

void Exp() {
	/*  Exatemante na ordem em que são analisados
	Exp → PrefixExp ExpBlock
	
	Exp → { FieldList } ExpBlock

	Exp → not ExpBlock
	Exp → - ExpBlock
	Exp → function ExpBlock
	Exp → nil ExpBlock
	Exp → true ExpBlock
	Exp → false ExpBlock
	Exp → number ExpBlock
	Exp → string ExpBlock  
	 */
	

	if (token.nome_atributo == 260 || token.nome_atributo == '(') {
		// Não consumimos o token da agulha pois estamos analisando o first
		PrefixExp();
		ExpBlock();

	} else if (token.nome_atributo == '{') {
		token = proximo_token();
		fim_do_codigo();

		FieldList();
	} else {
		size_t exps_possibilities_size = 8;
		int exps_possibilities[] = {313,
				'-',
				308,
				312,
				318,
				306,
				261,
				270
				};	
		for (size_t i=0; i<exps_possibilities_size; i++) {
			if (exps_possibilities[i] == token.nome_atributo){
				token = proximo_token();
				fim_do_codigo();
				ExpBlock();
				break;
			}
		}
	}


}

void ExpList() {
	// ExpList → , Exp ExpList | ε 
	if (token.nome_atributo == ',') {
		token = proximo_token();
		fim_do_codigo();

		Exp();
		ExpList();
	} else {
		return;
	}
}

void Exps() {
	// Exps -> Exp ExpList
	Exp();
	ExpList();
}

void VarList() {
	// VarList -> , Var VarList | vazio
	if (token.nome_atributo == ',') {
		token = proximo_token();
		fim_do_codigo();

		Var();
		VarList();
	} else {
		return;
	}
}

void Vars() {
	// Vars -> Var VarList
	Var();
	VarList();
}

void ElseEndBlock() {
	// ElseEndBlock → else Block end | end
	if (token.nome_atributo == 303) {
		token = proximo_token();
		fim_do_codigo();

		Block();

		if (token.nome_atributo == 305) {
			token = proximo_token();
			//fim_do_codigo();
			return;
		}

	} else if (token.nome_atributo == 305) {
		token = proximo_token();
		fim_do_codigo();
		return;
	} // else teremos um erro
}

void ElseIfBlock() {
	// ElseIfBlock → elseif Exp then Block ElseIfBlock | ε
	if (token.nome_atributo == 304) {
		token = proximo_token();
		fim_do_codigo();
		
		Exp();

		if (token.nome_atributo == 317) {
			token = proximo_token();
			fim_do_codigo();
			
			Block();
			ElseIfBlock();

		} // else aqui teremos que ter um erro



	} else {
		return;
	}
}

void ElseBlock() {
	// ElseBlock → ElseIfBlock ElseEndBlock
	ElseIfBlock();

	ElseEndBlock();	
}

void ExpsOpt() {
	bool gate = false;
	size_t exps_possibilities_size = 11;
	int exps_possibilities[] = {260, '(', '{', 313, '-', 308, 312, 318, 306, 261, 270};

	for (size_t i=0;i<exps_possibilities_size;i++) {
		if (exps_possibilities[i] == token.nome_atributo) {
			gate=true;
		}
	}

	if (gate) {
		Exps();
	} else {
		return;
	}
} 

void ExpOpt1() {
	// ExpOpt1 → , Exp | ε
	if (token.nome_atributo == ',') {
		token = proximo_token();
		fim_do_codigo();

		Exp();
	} else {
		return;
	}
}

void ForBlock() {

	if (token.nome_atributo == '=') {
		// ForBlock → = Exp , Exp ExpOpt1 do Block end
		token = proximo_token();
		fim_do_codigo();

		Exp();

		if (token.nome_atributo == ',') {
			token = proximo_token();
			fim_do_codigo();

			Exp();
			ExpOpt1();

			if (token.nome_atributo == 302) {
				token = proximo_token();
				fim_do_codigo();

				Block();

				if (token.nome_atributo == 305) {
					token = proximo_token();
					fim_do_codigo();

					return;
				} // else temos erros para analisar(no off, tem erro PARA CARALHO para analisar, fudeufudeufudeufudeu)
			} // else temos erros para analisar


		} // else vamos ter que analisar o erro

	


	} else if (token.nome_atributo == ',') {
		// ForBlock → NameList in Exps do Block end
		// Não vamos consumir o token da agulha porque aqui é uma análise do first
		NameList();	
	} // else temos um erro para tratar(tem erro para tratar até o cú fazer bico)

	
}

void NameList() {
	if (token.nome_atributo == ',') {
		token = proximo_token();
		fim_do_codigo();

		if (token.nome_atributo == 260) {
			token = proximo_token();
			fim_do_codigo();

			NameList();
		} // else, erro

	} else {
		return;
	}
}

void Names() {
	if (token.nome_atributo == 260) {
		token = proximo_token();
		fim_do_codigo();
		NameList();
	}
}

void Params() {
	Names();
}

void ParamsList() {
	if (token.nome_atributo == 260) {
		// Não consumimos o token por ser uma análise de first
		Params();
	} else {
		return;
	}
}

void FunctionBody() {
	// FunctionBody → ( ParamsList ) block end 
	
	if (token.nome_atributo == '(') {
		token = proximo_token();
		fim_do_codigo();

		ParamsList();
		if (token.nome_atributo == ')') {
			token = proximo_token();
			fim_do_codigo();

			Block();
			if (token.nome_atributo == 305) {
				token = proximo_token();
				fim_do_codigo();
			} // else, msm história de smp

		} // else, erro
		
	} // else, já sabe né


}

void LocalBlock() {
	if (token.nome_atributo == 308) {
		// LocalBlock → function name FunctionBody
		token = proximo_token();
		fim_do_codigo();

		if (token.nome_atributo == 260) {
			token = proximo_token();
			fim_do_codigo();

			FunctionBody();
		} // else erro

	} else if (token.nome_atributo == 260) {
		// LocalBlock → Names = Exps
		// Não consumimos o token da agulha porque é uma analise de first
		Names();

		if (token.nome_atributo == '=') {
			token = proximo_token();
			fim_do_codigo();

			Exps();
		} // else erro
	} // else erro
}

void Stmt() {

	
	if (token.nome_atributo == 260 || token.nome_atributo == '(') { // 260(Id) e '(' são o conjunto First de 'Vars'
		// Stmt -> Vars = Exps
		Vars();
		if (token.nome_atributo == '=') {
			token = proximo_token();
			fim_do_codigo();	
			Exps();
		} else {
			erro=true;
			printf( RED "Erro, esperado '='\n" RESET );
			Erro('p', EXPS);
			Exps();

		} 

	} else if (token.nome_atributo == 302) { // Keyword para 'do' na tabela de simbolos
		// Stmt -> do Block end

		token = proximo_token();
		fim_do_codigo();
		Block();

		if (token.nome_atributo == 305) {
			token = proximo_token();
			fim_do_codigo();
			return;
		} else {
			erro=true;
			printf( RED "Erro, esperado 'end'\n" RESET );
			Erro('s', STMT);
			return;
		} 
	} else if (token.nome_atributo == 320) {// Keyword para 'while' na tabela de simbolos
		// Stmt -> while Exp do Block end

		token = proximo_token();
		fim_do_codigo();
		Exp();

		if (token.nome_atributo == 302) {
			token = proximo_token();
			fim_do_codigo();

	
		} else {
			erro=true;
			printf( RED "Erro, esperado 'do'\n" RESET );
			Erro('p', BLOCK);
		}

		Block();

		if (token.nome_atributo == 305){
			token = proximo_token();
			//fim_do_codigo();
			return;
		} else {
			erro=true;
			printf( RED "Erro, esperado 'end'\n" RESET );
			Erro('s', STMT);
			return;
		}
	} else if (token.nome_atributo == 309) { // Keyword para 'if' na tabela de simbolos
		// Stmt -> if Exp Then Block ElseBlock

		token = proximo_token();
		fim_do_codigo();
		
		Exp();

		if (token.nome_atributo == 317) {
			token = proximo_token();
			fim_do_codigo();

		} else {
			erro=true;
			printf( RED "Erro, esperado 'then'\n" RESET );
			Erro('p', BLOCK);
		}
		Block();
		ElseBlock(); 

	} else if (token.nome_atributo == 316) { // Keyword para 'return' na tabela de simbolos
		// Stmt -> return ExpsOpt
		token = proximo_token();
		fim_do_codigo();
		ExpsOpt(); 
		
	} else if (token.nome_atributo == 301) { // Keyword para 'break' na tabela de simbolos
		// Stmt -> break
		token = proximo_token();
		fim_do_codigo();
		return;

	} else if (token.nome_atributo == 307) { // Keyword para 'for' na tabela de simbolos
		// for name ForBlock
		
		token = proximo_token();
		fim_do_codigo();

		if (token.nome_atributo == 260) {
			token = proximo_token();
			fim_do_codigo();

		} else {
			erro=true;
			printf( RED "Erro, esperado identificador\n" RESET );
			Erro('p', FORBLOCK);
		} 
		
		ForBlock(); 

	} else if (token.nome_atributo == 308) { // Keyword para 'function'  na tabela de simbolos
		// Stmt → function name FunctionBody
		
		token = proximo_token();
		fim_do_codigo();

		if (token.nome_atributo == 260) {
			token = proximo_token();
			fim_do_codigo();

			FunctionBody();
		}

	} else if (token.nome_atributo == 311) { // Keyword para 'local' na tabela de simbolos 
		// Stmt → local LocalBlock

		token = proximo_token();
		fim_do_codigo();
		LocalBlock();

	}  else {
		erro = true;
		printf( RED "Erro, era esperado um dos símbolos first de Statement\n" RESET);
		Erro('s', STMT);
		StmtList1();
	} 

}


void StmtList1() {
	// StmtList1 → Stmt ; StmtList1 | ε
	
	if (esta_no_conjunto('p', STMT)){
		Stmt();
		if (token.nome_atributo == ';') {
			token = proximo_token();
			//fim_do_codigo();

			StmtList1();
		} else {
			erro = true;
			printf( RED "Erro, esperado ';'\n" RESET);
			Erro('p', STMTLIST1);
			StmtList1();
		}
	} else {
	   	return;
	}
}

void StmtList() {
	// StmtList → Stmt ; StmtList1 
	Stmt();
	if (token.nome_atributo == ';'){ 
		token = proximo_token();
		if (token.nome_atributo == -1) return;
		else StmtList1();
	} else { 
		erro = true;
		printf( RED "Erro, esperado ';'\n" RESET);
		Erro('p', STMTLIST1);  
		Block();
	}
}

void Block() {
        // Block → StmtList 
	StmtList();
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Como chamar o compilador: %s <nome do arquivo>\n", argv[0]);
		return 1;
	}

	char *filename = argv[1];
	code = readFile(filename);
	
	
	if (code == NULL) {
		printf("Arquivo não encontrado\n");
		exit(0);
	} else {
		token = proximo_token();
		fim_do_codigo();	
		Block();
	}

	if (erro) {
		printf( RED "Análise sintática realizada com erros encontrados no código\n" RESET);
	} else {
		printf( GREEN "Análise sintática realizada com sucesso\n" RESET);
	}
			
	free(code);
	return 0;
}
