#include <stdio.h>
#include "lexico.h"

Token token;
char *code;


void Stmt() {
	if (token.nome_atributo == -1){ // Será que terei que fazer isso sempre que chamar "proximo_token"?
		printf("End of File\n");
		exit(0); // Sinceramente não sei se isso está certo, kalai do futuro irá resolver
	}


	// Stmt -> Vars = Exps | 
}


void Block() {
	if (token.nome_atributo == -1){ // Será que terei que fazer isso sempre que chamar "proximo_token"?
		printf("End of File\n");
		exit(0); // Sinceramente não sei se isso está certo, kalai do futuro irá resolver
	}
	

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
