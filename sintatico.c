#include <stdio.h>
#include "lexico.h"

Token token;
char *code;

void Teste4() {
	printf("Nome do atributo: %d\nAtributo: %d\n",token.nome_atributo, token.atributo);
}


void Teste3() {
	printf("Nome do atributo: %d\nAtributo: %d\n",token.nome_atributo, token.atributo);
	
	token = proximo_token();
	Teste4();
}

void Teste2() {
	printf("Nome do atributo: %d\nAtributo: %d\n",token.nome_atributo, token.atributo);

	token = proximo_token();
	Teste3();
}
void Teste() {
	
	printf("Nome do atributo: %d\nAtributo: %d\n",token.nome_atributo, token.atributo);
	
	token = proximo_token();
	Teste2();
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
		Teste();
	}
			
	free(code);
	return 0;
}
