#include <stdio.h>
#include <stdbool.h>
#include "lexico.h"


#define STMT 3

Token token;
char *code;

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



void fim_do_codigo();
void Block();
void Exp();


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

/* // Isso aqui ainda não está pronto
void Erro(char function_type, unsigned int n_terminal) {
	if (function_type == 'p') { // 'p' para 'primeiro' == 'first'
		switch (n_terminal) {
			case STMT:
				
				 // Pseudo-codigo:
				 // Comparamos token com os "Firsts" de STMS
				 // Se não bater com nenhum, chamamos token de novo
				 // Se bater, caimos fora da função com o token na agulha
				 
				size_t firsts_size = 10;
				int firsts[] = {260, 
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
					if (token.nome_atributo == firsts[i]) {
						return;
					}
				}
				
				
				token = proximo_token();
				fim_do_codigo();
				Erro('p', n_terminal);
				break;
				

		}
	} else if (function_type == 's'){ // 's' para 'sequencia' == 'follow'
	

	} else {
		printf("Você cagou no pau na chamada da funçao 'Erro'");
		exit(1);
	}
}
*/
void fim_do_codigo() {
	if (token.nome_atributo == -1){ 
		printf("End of File\n");
		exit(1); 
	}
}

void Var() {
	// Var -> name | Prefixexp [ Exp ]

	if (token.nome_atributo == 260) {
		token = proximo_token();
		fim_do_codigo();
	} else if (token.nome_atributo == '(') { // TODO: Pensar se isso aqui está certo, to imaginando que quando chegamos aqui estamos vindo de 'PrefixExp'
		token = proximo_token();
		fim_do_codigo();
	}
}

void ExpBlock() { // Por enquanto isso aqui não vai fazer nada
	return;
}

void PeBlock() {

	
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

	}
	
	if (gate) {
		PeBlock();
	} // else é por que deu algum erro e n faço ideia do que fzr por enquanto kkkkkkkkkkkkk
}

void FieldList() {

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
	
	bool gate = false;

	if (token.nome_atributo == 260 || token.nome_atributo == '(') {
		PrefixExp();
		gate = true;
	} else if (token.nome_atributo == '{') {
		FieldList();
		gate = true;
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
				gate = true;
			}
		}
	}
	if (gate) {
		ExpBlock();	
	} // else é por que deu algum erro e teremos que recuperar

}

void ExpList() {
	if (token.nome_atributo == ',') {
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
	if (token.nome_atributo == ',') {
		Exp();
	}

}

void VarList() {
	// VarList -> , Var VarList | vazio
	if (token.nome_atributo == ',') {
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


void Stmt() {

	
	if (token.nome_atributo == 260 || token.nome_atributo == '(') { // 260(Id) e '(' são o conjunto First de 'Vars'
		// Stmt -> Vars = Exps
		Vars();
		if (token.nome_atributo == '=') {
			token = proximo_token();
			fim_do_codigo();	
			Exps();
		} /*else { // Para o futuro, quando iremos tratar os erros
			erro();
		} */

	} else if (token.nome_atributo == 302) { // Keyword para 'do' na tabela de simbolos
		// Stmt -> do Block end

		token = proximo_token();
		fim_do_codigo();
		Block();

		if (token.nome_atributo == 305) {
			token = proximo_token();
			fim_do_codigo();
			return;
		} 
	} else if (token.nome_atributo == 320) {// Keyword para 'while' na tabela de simbolos
		// Stmt -> while Exp do Block end

		token = proximo_token();
		fim_do_codigo();
		Exp();

		if (token.nome_atributo == 302) {
			Block();
			if (token.nome_atributo == 305){
				token = proximo_token();
				fim_do_codigo();
				return;
			}
		}
	} else if (token.nome_atributo == 309) { // Keyword para 'if' na tabela de simbolos
		// Stmt -> if Exp Then Block ElseBlock

		token = proximo_token();
		fim_do_codigo();
		Exp();

		if (token.nome_atributo == 317) {
			Block();
			//ElseBlock(); // TODO: Implementar ElseBlock
		}

	} else if (token.nome_atributo == 316) { // Keyword para 'return' na tabela de simbolos
		// Stmt -> return ExpsOpt
		token = proximo_token();
		fim_do_codigo();
		//ExpsOpt(); // TODO: Implementar ExpsOpt
		
	} else if (token.nome_atributo == 301) { // Keyword para 'break' na tabela de simbolos
		// Stmt -> break
		token = proximo_token();
		fim_do_codigo();
		return;

	} else if (token.nome_atributo == 307) { // Keyword para 'for' na tabela de simbolos
		// Stmt -> for ForBlock

		token = proximo_token();
		fim_do_codigo();
		//ForBlock(); // TODO: Implementar ForBlock
	} else if (token.nome_atributo == 308) { // Keyword para 'function'  na tabela de simbolos
		// Stmt -> LocalOpt function name FunctionBody
		
		// TODO: Acho que isso aqui vai ser um problema, pois teremos que alterar o first de Stmt, qual o first de LocalOpt?

	} else if (token.nome_atributo == 311) { // Keyword para 'local' na tabela de simbolos 
		// Stmt -> local Names = Exps

		token = proximo_token();
		fim_do_codigo();
		//Names(); // TODO: Implementar Names

	} /* else { // else para erro no futuro

	} */

}


void StmtList1() {
	// StmtList1 -> Stmt ; StmtList | vazio
	
	if (esta_no_conjunto('p', STMT)){
		Stmt();
	} else {
	   	return;
	}
}

void StmtList() {
	// StmtList -> Stmt ; StmtList1
	Stmt();
	if (token.nome_atributo == ';'){ 
		token = proximo_token();
		if (token.nome_atributo == -1) return;
		else StmtList1();
	} /*else { // TODO: Se der erro, vamos procurar o first de 'StmtList1' 
		printf("Erro, esperado ';'\n");
		Erro('p', STMT);  
		Block();
	}*/
}

void Block() {
        // Block -> StmtList
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
	} else {
		token = proximo_token();
		fim_do_codigo();	
		Block();
	}
	printf("Análise sintática realizada com sucesso\n");
			
	free(code);
	return 0;
}
