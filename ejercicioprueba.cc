#include <iostream>
#include <string>

using namespace std;

// --- Estructura para los Mensajes (Lista Enlazada) ---
struct Mensaje {
    string remitente;
    string contenido;
    Mensaje* siguiente;

    Mensaje(string rem, string cont) : remitente(rem), contenido(cont), siguiente(nullptr) {}
};

class ListaMensajes {
private:
    Mensaje* cabeza;

public:
    ListaMensajes() : cabeza(nullptr) {}

    ~ListaMensajes() {
        while (cabeza != nullptr) {
            Mensaje* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void agregarMensaje(string rem, string cont) {
        Mensaje* nuevo = new Mensaje(rem, cont);
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Mensaje* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
    }

    void mostrarHistorial() {
        if (cabeza == nullptr) {
            cout << "  (No hay mensajes en este historial)\n";
            return;
        }
        Mensaje* actual = cabeza;
        while (actual != nullptr) {
            cout << "  [" << actual->remitente << "]: " << actual->contenido << "\n";
            actual = actual->siguiente;
        }
    }
};

// --- Estructura para los Usuarios (Tabla Hash) ---
struct Usuario {
    string idUsuario;
    string nombre;
    ListaMensajes historial;
    Usuario* siguiente; // Para manejar colisiones en la tabla hash

    Usuario(string id, string nom) : idUsuario(id), nombre(nom), siguiente(nullptr) {}
};

class TablaHashUsuarios {
private:
    static const int TAM_TABLA = 10;
    Usuario* tabla[TAM_TABLA];

    int funcionHash(string id) {
        int suma = 0;
        for (char c : id) {
            suma += c;
        }
        return suma % TAM_TABLA;
    }

public:
    TablaHashUsuarios() {
        for (int i = 0; i < TAM_TABLA; i++) {
            tabla[i] = nullptr;
        }
    }

    ~TablaHashUsuarios() {
        for (int i = 0; i < TAM_TABLA; i++) {
            Usuario* actual = tabla[i];
            while (actual != nullptr) {
                Usuario* temp = actual;
                actual = actual->siguiente;
                delete temp;
            }
        }
    }

    bool registrarUsuario(string id, string nombre) {
        int indice = funcionHash(id);
        Usuario* actual = tabla[indice];
        
        while (actual != nullptr) {
            if (actual->idUsuario == id) {
                return false; // Ya existe
            }
            actual = actual->siguiente;
        }

        Usuario* nuevo = new Usuario(id, nombre);
        nuevo->siguiente = tabla[indice];
        tabla[indice] = nuevo;
        return true;
    }

    Usuario* buscarUsuario(string id) {
        int indice = funcionHash(id);
        Usuario* actual = tabla[indice];
        while (actual != nullptr) {
            if (actual->idUsuario == id) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    bool eliminarUsuario(string id) {
        int indice = funcionHash(id);
        Usuario* actual = tabla[indice];
        Usuario* anterior = nullptr;

        while (actual != nullptr) {
            if (actual->idUsuario == id) {
                if (anterior == nullptr) {
                    tabla[indice] = actual->siguiente;
                } else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }
};

// --- Menú y Control Principal ---
void mostrarMenu() {
    cout << "\n=======================================\n";
    cout << "          SALA CHAT - UCA             \n";
    cout << "=======================================\n";
    cout << "1. Registrar / Validar Usuario\n";
    cout << "2. Buscar Usuario y Consultar Historial\n";
    cout << "3. Enviar Mensaje a Usuario\n";
    cout << "4. Eliminar Usuario\n";
    cout << "5. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    TablaHashUsuarios sistemaUsuarios;
    int opcion;

    do {
        mostrarMenu();
        if (!(cin >> opcion)) {
            cout << "Entrada invalida.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (opcion == 1) {
            string id, nombre;
            cout << "Ingrese ID unico de usuario: ";
            getline(cin, id);
            cout << "Ingrese nombre del usuario: ";
            getline(cin, nombre);

            if (sistemaUsuarios.registrarUsuario(id, nombre)) {
                cout << "Usuario registrado exitosamente.\n";
            } else {
                cout << "El usuario con ID '" << id << "' ya existe.\n";
            }
        } 
        else if (opcion == 2) {
            string id;
            cout << "Ingrese ID de usuario a buscar: ";
            getline(cin, id);

            Usuario* u = sistemaUsuarios.buscarUsuario(id);
            if (u != nullptr) {
                cout << "\n--- Informacion del Usuario ---\n";
                cout << "ID: " << u->idUsuario << "\nNombre: " << u->nombre << "\n";
                cout << "Historial de Conversacion:\n";
                u->historial.mostrarHistorial();
            } else {
                cout << "Usuario no encontrado.\n";
            }
        } 
        else if (opcion == 3) {
            string id, remitente, texto;
            cout << "Ingrese ID del usuario destinatario/propietario del chat: ";
            getline(cin, id);

            Usuario* u = sistemaUsuarios.buscarUsuario(id);
            if (u != nullptr) {
                cout << "Remitente del mensaje: ";
                getline(cin, remitente);
                cout << "Contenido del mensaje: ";
                getline(cin, texto);

                u->historial.agregarMensaje(remitente, texto);
                cout << "Mensaje guardado y asociado al usuario exitosamente.\n";
            } else {
                cout << "Usuario no encontrado. Registrelo primero.\n";
            }
        } 
        else if (opcion == 4) {
            string id;
            cout << "Ingrese ID del usuario a eliminar: ";
            getline(cin, id);

            if (sistemaUsuarios.eliminarUsuario(id)) {
                cout << "Usuario eliminado correctamente del sistema.\n";
            } else {
                cout << "No se encontro un usuario con ese ID.\n";
            }
        } 
        else if (opcion == 5) {
            cout << "Saliendo del sistema SalaChat... ¡Hasta luego!\n";
        } 
        else {
            cout << "Opcion no valida. Intente de nuevo.\n";
        }

    } while (opcion != 5);

    return 0;
}