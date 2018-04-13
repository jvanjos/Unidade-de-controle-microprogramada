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

struct linhaComando {
	std::string comando;
	int recebe;
	std::string envia;
	int irPara;
};

std::vector<linhaComando> comandos;
int registradores[6];

void zerarRegistrador() {
	for (int i = 0; i < 6; i++) {
		registradores[i] = 0;
	}
}

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
	std::string codigoNome;
	std::string dadosNome;
	std::cout << "Digite o nome do codigo Assembly(ex. teste.txt): ";
	std::cin >> codigoNome;
	codigo.open(codigoNome.data());
	if (!codigo.is_open()) {
		system("pause");
		return -1;
	}
	std::string stringer;
	std::string sRecebe;
	std::string sEnvia;
	std::string sIrPara;

	while (!codigo.eof()) {													// constroi o vetor de processos baseado no arquivo recebido
		linhaComando aux;
		std::getline(codigo, stringer);										// pega a linha do arquivo
		std::stringstream a;
		a << stringer;
		a >> aux.comando >> sRecebe >> aux.envia >> sIrPara;						// separa as duas strings separadas pelo espaço em branco
		if (aux.comando == "JUMPME" or aux.comando == "JUMPMA" or aux.comando == "JUMPIG" or aux.comando == "JUMPMAIG" or aux.comando == "JUMPMEIG" or aux.comando == "JUMPDIF") {
			aux.recebe = sRecebe[1] - '0';
			aux.irPara = std::stoi(sIrPara);
		}
		else if (aux.comando == "JUMP") {
			aux.recebe = 0;
			aux.irPara = sRecebe[1] - '0';
		}
		else if (aux.comando == "PRINT") {
			aux.recebe = sRecebe[1] - '0';
			aux.irPara = 0;
		}
		else {
			aux.recebe = sRecebe[1] - '0';			
			aux.irPara = 0;
		}
		comandos.push_back(aux);											// adiciona o comando ao vetor comandos
	}
	std::cout << "Digite o nome do arquivo de dados(ex. teste.txt): ";
	std::cin >> dadosNome;
	dados.open(dadosNome.data());
	if (!dados.is_open()) {
		system("pause");
		return -1;
	}
	while (!dados.eof()) {
		ciclos = 0;
		zerarRegistrador();
		std::getline(dados, stringer);										// pega a linha do arquivo
		registradores[0] = std::stoi(stringer);
		for (int i = 1; i < comandos.size(); i++) {
			ciclos++;
			if (comandos[i].comando == "ADD") {
				ciclos+=2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] + registradores[(comandos[i].envia[1] - '0') - 1];
				else
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] + std::stoi(comandos[i].envia);
			}
			else if (comandos[i].comando == "MOV") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[(comandos[i].envia[1] - '0') - 1];
				else
					registradores[comandos[i].recebe - 1] = std::stoi(comandos[i].envia);
			}
			else if (comandos[i].comando == "SUBB") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] - registradores[(comandos[i].envia[1] - '0') - 1];
				else
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] - std::stoi(comandos[i].envia);
			}
			else if (comandos[i].comando == "MUL") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] * registradores[(comandos[i].envia[1] - '0') - 1];
				else
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] * std::stoi(comandos[i].envia);
			}
			else if (comandos[i].comando == "DIV") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R')
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] / registradores[(comandos[i].envia[1] - '0') - 1];
				else
					registradores[comandos[i].recebe - 1] = registradores[comandos[i].recebe - 1] / std::stoi(comandos[i].envia);
			}
			else if (comandos[i].comando == "JUMPME") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] < registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] < std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;
					}
				}
			}
			else if (comandos[i].comando == "JUMPMEIG") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] <= registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] <= std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;
					}
				}
			}
			else if (comandos[i].comando == "JUMPMA") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] > registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] > std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;
					}
				}
			}
			else if (comandos[i].comando == "JUMPMAIG") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] >= registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] >= std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;
					}
				}
			}
			else if (comandos[i].comando == "JUMPDIF") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] != registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] != std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;
					}
				}
			}
			else if (comandos[i].comando == "JUMPIG") {
				ciclos += 2;
				if (comandos[i].envia[0] == 'R') {
					if (registradores[comandos[i].recebe - 1] == registradores[(comandos[i].envia[1] - '0') - 1]) {
						i = comandos[i].irPara - 2;
					}
				}
				else {
					if (registradores[comandos[i].recebe - 1] == std::stoi(comandos[i].envia)) {
						i = comandos[i].irPara - 2;
					}
				}
			}
			else if (comandos[i].comando == "JUMP") {
				ciclos += 2;
				i = comandos[i].irPara - 1;
			}
			else if (comandos[i].comando == "PRINT") {
				ciclos += 2;
				std::cout << registradores[comandos[i].recebe - 1] << " ";
			}			
		}
		std::cout <<"ciclos: "<< ciclos << std::endl;
	}
	system("pause");
	return 0;
}

