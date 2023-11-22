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
	fim_do_codigo(); // Acabei de me tocar que não preciso ficar chamando isso aqui o tempo todo, só depois de chamar proximo_token

	if (token.nome_atributo == 260) {
		token = proximo_token();
	} else if (token.nome_atributo == '(') { // TODO: Pensar se isso aqui está certo, to imaginando que quando chegamos aqui estamos vindo de 'Prefixexp'
		token = proximo_token();
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
		ExpBlock();
	}
}

void Exps() {
	fim_do_codigo();
	

	// Exps -> Exp (,Exp)*
	Exp();
	if (token.nome_atributo == ',') {
		Exp();
	}
	// Imagino que não precise chamar 'erro' aqui, mas não sei ainda


}

void Vars() {
	fim_do_codigo();
	

	// Vars -> Var (,Var)*
	Var();
	if (token.nome_atributo == ',') {
		Var();
	}
	// Imagino que não precise chamar 'erro' aqui, mas não sei ainda

}


void Stmt() {
	fim_do_codigo();	


	
	if (token.nome_atributo == 260 || token.nome_atributo == '(') { // 260(Id) e '(' são o conjunto First de 'Vars'
		// Stmt -> Vars = Exps
		Vars();
		if (token.nome_atributo == '=') {
			token = proximo_token();
			Exps();
		} /*else { // Para o futuro, quando iremos tratar os erros
			erro();
		} */

	}
}


void Block() {
	fim_do_codigo();	
	

        // Block -> (Stmt;)*
	Stmt();
	if (token.nome_atributo == ';'){
		token = proximo_token();
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
		Block();
	}
			
	free(code);
	return 0;
}
