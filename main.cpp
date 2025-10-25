#include <iostream>
#include "segment_tree.h"


int main()
{
	segment_tree arbolin(3);
	arbolin.insert("Guayaba");
	arbolin.insert("Guayaba");
	arbolin.insert("Pepe");
	arbolin.insert("Miguelin");
	arbolin.insert("Mouse");
	arbolin.insert("C++");
	arbolin.insert("Algoritmos");
	arbolin.insert("Mafia 3 Definitive Edition");
	arbolin.insert("nose");
	arbolin.insert("test");
	arbolin.insert("IS2asdsad");
	arbolin.insert("Miguelin");
	arbolin.insert("Miguelin");
	arbolin.insert("Miguelin");
	arbolin.insert("Guayaba");
	arbolin.insert("Messi");
	


	arbolin.print();
}