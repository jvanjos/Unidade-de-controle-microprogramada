// Projeto Arquitetura.cpp : define o ponto de entrada para o aplicativo do console.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include<sstream>
#include<fstream>


std::ifstream codigo;
std::ifstream dados;
int ciclos;

/*Struct que pega cada comando, ou seja, cada linha do código Assembly*/
struct linhaComando {
	std::string comando;
	int recebe;
	std::string envia;
	int irPara;
};

/*Vetor que armazena todas as linhas de comando, ou seja, armazena todo código*/
std::vector<linhaComando> comandos;
/*Armazenas todos os valores dos 6 registradores */
int registradores[6];

/*Zera todos os registradores*/
void zerarRegistrador() {
	for (int i = 0; i < 6; i++) {
		registradores[i] = 0;
	}
}
/*Serve so pra ajudar a achar os erros, printando os valores dos registradores e dos comandos armazenados no vetor*/
void printar() {
	for (int i = 0; i < comandos.size(); i++)
		std::cout << comandos[i].comando << " " << comandos[i].recebe << " " << comandos[i].envia << " " << comandos[i].irPara << std::endl;
	for (int i = 0; i < 6; i++) {
		std::cout << i << " " << registradores[i] << std::endl;
	}
	std::cout << std::endl;
}


int main()
{
	std::string codigoNome;													// Nome do arquivo do código Assembly
	std::string dadosNome;													// Nome do arquivo de dados
	std::cout << "Digite o nome do codigo Assembly(ex. teste.txt): ";		
	std::cin >> codigoNome;													// Pega o nome do arquivo do código Assembly e coloca em codigoNome
	codigo.open(codigoNome.data());											//	Abre o arquivo
	if (!codigo.is_open()) {												// Se nao abrir, pausa e retorna -1
		system("pause");
		return -1;
	}
	std::string stringer;													// String para armazenar cada linha do código assembly
	std::string sRecebe;													// String que armazena o registrador que recebe a ação
	std::string sEnvia;														// String que armazena o modificador do registrador que recebe a ação
	std::string sIrPara;													// String que armazena o numero da linha de comando especfica que devemos ir na proxima execuçao do fetch

	while (!codigo.eof()) {													// constroi o vetor de processos baseado no arquivo recebido
		linhaComando aux;
		std::getline(codigo, stringer);										// pega a linha do arquivo
		std::stringstream a;												// String que pega uma linha do arquivo e separa ela pelos espaços em branco
		a << stringer;														// Colocando a linha na Stringstream a
		a >> aux.comando >> sRecebe >> aux.envia >> sIrPara;				// separa as strings separadas pelo espaço em branco. Ja coloca direto no envia da linha de comando pois eh uma string
		/*Se for JUMP com comparação ele so pega os 2 registradores de comparaçao e a linha que ele deve pular, mas como um dos registradores ja foi armazedo direto so pegamos os
		outros 2*/
		if (aux.comando == "JUMPME" or aux.comando == "JUMPMA" or aux.comando == "JUMPIG" or aux.comando == "JUMPMAIG" or aux.comando == "JUMPMEIG" or aux.comando == "JUMPDIF") {
			aux.recebe = sRecebe[1] - '0';
			aux.irPara = std::stoi(sIrPara);
		}
		/* Se for JUMP sem comparaçao so pegamos a linha que devemos ir*/
		else if (aux.comando == "JUMP") {
			aux.recebe = 0;
			aux.irPara = sRecebe[1] - '0';
		}
		/*Se for PRINT pegamos o regitsrador que devemos printar*/
		else if (aux.comando == "PRINT") {
			aux.recebe = sRecebe[1] - '0';
			aux.irPara = 0;
		}
		/*Os outros pegamos so o Registrador que recebe e o que modifica(envia)*/
		else {
			aux.recebe = sRecebe[1] - '0';			
			aux.irPara = 0;
		}
		comandos.push_back(aux);											// adiciona o comando ao vetor comandos
	}
	std::cout << "Digite o nome do arquivo de dados(ex. teste.txt): ";     
	std::cin >> dadosNome;													// Pega o nome do arquivo de dados e coloca em dadosNome
	dados.open(dadosNome.data());											// Abre o arquivo
	if (!dados.is_open()) {													// Se nao abrir, pausa e retorna -1
		system("pause");
		return -1;
	}
	while (!dados.eof()) {													//Enquanto nao acabar o arquivo de dados executa
		ciclos = 0;															// ciclo começa com 0
		zerarRegistrador();													// zera o registrador para ele nao ficar cheio entre uma dado e outro
		std::getline(dados, stringer);										// pega a linha do arquivo
		registradores[0] = std::stoi(stringer);
		for (int i = 1; i < comandos.size(); i++) {						//busca de instrução
			ciclos++;													// incrementa o numero de ciclos
			if (comandos[i].comando == "ADD") {						
				ciclos+=2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] + registradores[(comandos[i].envia[1] - '0') - 1];// execução
				else
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] + std::stoi(comandos[i].envia);// execução
			}
			else if (comandos[i].comando == "MOV") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[(comandos[i].envia[1] - '0') - 1];// execução
				else
					registradores[comandos[i].recebe - 1] = std::stoi(comandos[i].envia);// execução
			}
			else if (comandos[i].comando == "SUBB") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] - registradores[(comandos[i].envia[1] - '0') - 1];// execução
				else
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] - std::stoi(comandos[i].envia);// execução
			}
			else if (comandos[i].comando == "MUL") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] * registradores[(comandos[i].envia[1] - '0') - 1];// execução
				else
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] * std::stoi(comandos[i].envia);// execução
			}
			else if (comandos[i].comando == "DIV") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] / registradores[(comandos[i].envia[1] - '0') - 1];// execução
				else
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] / std::stoi(comandos[i].envia);// execução
			}
			else if (comandos[i].comando == "JUMPME") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] < registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;// execução
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] < std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;// execução
					}
				}
			}
			else if (comandos[i].comando == "JUMPMEIG") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] <= registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;// execução
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] <= std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;// execução
					}
				}
			}
			else if (comandos[i].comando == "JUMPMA") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] > registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;// execução
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] > std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;// execução
					}
				}
			}
			else if (comandos[i].comando == "JUMPMAIG") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] >= registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;// execução
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] >= std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;// execução
					}
				}
			}
			else if (comandos[i].comando == "JUMPDIF") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] != registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;// execução
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] != std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;// execução
					}
				}
			}
			else if (comandos[i].comando == "JUMPIG") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] == registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;// execução
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] == std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;// execução
					}
				}
			}
			else if (comandos[i].comando == "JUMP") {
				ciclos += 2;
				i = comandos[i].irPara - 2;// execução
			}
			else if (comandos[i].comando == "PRINT") {
				ciclos += 2;
				std::cout << registradores[comandos[i].recebe - 1] << " ";// execução e armazenamento na memória---- Printa o valor do registrador
			}			
		}
		std::cout <<"ciclos: "<< ciclos << std::endl;
	}
	system("pause");
	return 0;
}

