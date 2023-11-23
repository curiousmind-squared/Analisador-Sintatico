#include <stdio.h>
#include "lexico.h"

Token token;
char *code;

void fim_do_codigo() {
	if (token.nome_atributo == -1){ // Será que terei que fazer isso sempre que chamar "proximo_token"?
		printf("End of File\n");
		exit(0); // Sinceramente não sei se isso está certo, kalai do futuro irá resolver
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
		printf("Keyword Do\n");
	} else if (token.nome_atributo == 320) {// Keyword para 'while' na tabela de simbolos
		// Stmt -> while Exp do Block end
		printf("Keyword while\n");
	} else if (token.nome_atributo == 309) { // Keyword para 'if' na tabela de simbolos
		// Stmt -> if Exp Then Block (elseif Exp then Block)* (else Block)^opt end
		printf("Keyword if\n");
	} else if (token.nome_atributo == 326) { // Keyword para 'return' na tabela de simbolos
		// Stmt -> return (Exps)^opt
		printf("Keyword return\n");
	} else if (token.nome_atributo == 301) { // Keyword para 'break' na tabela de simbolos
		// Stmt -> break
		printf("Keyword break\n");
	} else if (token.nome_atributo == 307) { // Keyword para 'for' na tabela de simbolos
		// Stmt -> for ForBlock
		printf("Keyword para for\n");
	} else if (token.nome_atributo == 308) { // Keyword para 'function'  na tabela de simbolos
		// Stmt -> functions Name FunctionBody
		printf("Keyword para function\n");
	} else if (token.nome_atributo == 311) { // Keyword para 'local' na tabela de simbolos 
		// Stmt -> local LocalBlock
		printf("Keyword para local\n");
	} /* else { // else para erro no futuro

	} */

}


void Block() {
	

        // Block -> (Stmt;)*
	Stmt();
	if (token.nome_atributo == ';'){ // Caso seja ';', terminou corretamente, mas como fazer (Stmt;)*?
		token = proximo_token();
		fim_do_codigo();	
		Stmt();
	}

	/* TODO: No futuro teremos que adicionar esse reconhecimento para erro
	else
		erro();
	*/
	
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
			
	free(code);
	return 0;
}
