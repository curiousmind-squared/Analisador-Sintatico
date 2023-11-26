#include <stdio.h>
#include "lexico.h"


#define STMT 3

Token token;
char *code;


void fim_do_codigo();
void Block();

void Erro(char function_type, unsigned int n_terminal) {
	if (function_type == 'p') { // 'p' para 'primeiro' == 'first'
		switch (n_terminal) {
			case STMT:
				/*
				 Pseudo-codigo:
				 Comparamos token com os "Firsts" de STMS
				 Se não bater com nenhum, chamamos token de novo
				 Se bater, caimos fora da função com o token na agulha
				 */
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

void fim_do_codigo() {
	if (token.nome_atributo == -1){ 
		printf("End of File\n");
		exit(1); 
	}
}

void Var() {

	if (token.nome_atributo == 260) {
		token = proximo_token();
		fim_do_codigo();
	} else if (token.nome_atributo == '(') { // TODO: Pensar se isso aqui está certo, to imaginando que quando chegamos aqui estamos vindo de 'Prefixexp'
		token = proximo_token();
		fim_do_codigo();
	}
	/* Futura função para erro 
	else {
		erro();
	}
	*/
}

void ExpBlock() { // Por enquanto isso aqui não vai fazer nada
	return;
}

void Exp() {
	// Essa aqui é muito grande, vou fazer só uma agora para testar
	if (token.nome_atributo == 261) {
		token = proximo_token();
		fim_do_codigo();
		ExpBlock();
	}
}

void Exps() {
	

	// Exps -> Exp (,Exp)*
	Exp();
	if (token.nome_atributo == ',') {
		Exp();
	}
	// Imagino que não precise chamar 'erro' aqui, mas não sei ainda


}

void Vars() {
	

	// Vars -> Var (,Var)*
	Var();
	if (token.nome_atributo == ',') {
		Var();
	}
	// Imagino que não precise chamar 'erro' aqui, mas não sei ainda

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
		Block();
		if (token.nome_atributo == 305) {
			token = proximo_token();
			return;
		} 
	} else if (token.nome_atributo == 320) {// Keyword para 'while' na tabela de simbolos
		// Stmt -> while Exp do Block end
		Exp();
		if (token.nome_atributo == 302) {
			Block();
			if (token.nome_atributo == 305){
				token = proximo_token();
				return;
			}
		}
	} else if (token.nome_atributo == 309) { // Keyword para 'if' na tabela de simbolos
		// Stmt -> if Exp Then Block (elseif Exp then Block)* (else Block)^opt end
		printf("Keyword if\n");
		token = proximo_token();
		fim_do_codigo();
	} else if (token.nome_atributo == 316) { // Keyword para 'return' na tabela de simbolos
		// Stmt -> return (Exps)^opt
		printf("Keyword return\n");
		token = proximo_token();
		fim_do_codigo();
	} else if (token.nome_atributo == 301) { // Keyword para 'break' na tabela de simbolos
		// Stmt -> break
		printf("Keyword break\n");
		token = proximo_token();
		fim_do_codigo();
	} else if (token.nome_atributo == 307) { // Keyword para 'for' na tabela de simbolos
		// Stmt -> for ForBlock
		printf("Keyword para for\n");
		token = proximo_token();
		fim_do_codigo();
	} else if (token.nome_atributo == 308) { // Keyword para 'function'  na tabela de simbolos
		// Stmt -> functions Name FunctionBody
		printf("Keyword para function\n");
		token = proximo_token();
		fim_do_codigo();
	} else if (token.nome_atributo == 311) { // Keyword para 'local' na tabela de simbolos 
		// Stmt -> local LocalBlock
		printf("Keyword para local\n");
		token = proximo_token();
		fim_do_codigo();
	} /* else { // else para erro no futuro

	} */

}


void Block() {
	

        // Block -> StmtList
	// StmtList -> Stmt ; StmtList1
	// StmtList1 -> Stmt ; StmtList | vazio
	StmtList();
	if (token.nome_atributo == ';'){ 
		token = proximo_token();
		if (token.nome_atributo == -1) return;
		else Block();
	} else {
		printf("Erro, esperado ';'\n");
		Erro('p', STMT);  
		Block();
	}
	
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
