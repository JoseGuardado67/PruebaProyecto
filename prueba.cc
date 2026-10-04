#include<iostream>
#include <string>

struct Nodo {
    int dato;
    Nodo* siguiente;
    Nodo* anterior;
};
//haciendo conexion de nodo
/*
nuevo->siguiente = cabeza;
nuevo->anterior = nullptr;

if (cabeza != nullptr)
    cabeza->anterior = nuevo;

cabeza = nuevo;*/
