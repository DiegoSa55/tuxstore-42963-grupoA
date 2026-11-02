/* =============================================================
   Curso      : Estructura de Datos (24UC00428)
   Evaluacion : C2 - Medicion de competencias - Semana 12
   Proyecto   : Tux Store v2 (kit base)
   Institucion: Universidad Continental
   Semestre   : 2026-20
   Compilador : Dev-C++ (MinGW / GCC, C++11 o superior)
   NRC        : 42963
   Grupo      : A
   Integrantes: 
   Gutierrez Palomino, Airton.
   Cabana Charrez, Fabricio Abelardo.
   Neira Huillca, Sebastian Carlos.
   Salazar Bravo, Diego Sebastian.
   ============================================================= */

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

const int DISPONIBLE = 0;
const int EN_FILA    = 1;
const int INSTALADA  = 2;
const string ARCHIVO = "tuxstore.txt";

struct NodoApp {
    string nombre;
    string version;
    int    mb;
    int    prioridad;
    int    estado;
    int    inicio, fin, espera;
    NodoApp *sig;
};

struct NodoFila {
    string nombre;
    int    llegada;
    int    prioridad;
    NodoFila *sig;
};

struct NodoBloque {
    string nombre;
    int    mb;
    NodoBloque *sig;
};

// ===== FICHA =====
const string SERVIDOR = "pinguino-de-prueba";
const int VELOCIDAD   = 4;
const int DISCO       = 150;
const int N_APPS      = 8;
string fNombre[N_APPS]  = {"discord", "htop", "openssh", "vlc", "cowsay", "godot", "neofetch", "obs"};
string fVersion[N_APPS] = {"0.0.71",  "3.3",  "9.6",     "3.0", "3.03",   "4.3",   "7.1",      "30.1"};
int    fMb[N_APPS]      = {95,        2,      8,         45,    1,        120,     1,          110};
int    fPrio[N_APPS]    = {3,         1,      1,         3,     3,        2,       3,          3};
// =================

string estadoTexto(int e) {
    if (e == DISPONIBLE) return "disponible";
    if (e == EN_FILA)    return "en fila";
    return "instalada";
}

NodoApp* crearApp(string nombre, string version, int mb, int prioridad) {
    NodoApp *n = new NodoApp;
    n->nombre = nombre; n->version = version; n->mb = mb; n->prioridad = prioridad;
    n->estado = DISPONIBLE;
    n->inicio = n->fin = n->espera = -1;
    n->sig = nullptr;
    return n;
}

void mostrarApp(NodoApp *a) {
    cout << "  " << left << setw(12) << a->nombre << setw(9) << a->version
         << right << setw(5) << a->mb << " MB   prioridad " << a->prioridad
         << "   " << estadoTexto(a->estado) << endl;
}

bool aEntero(string s, int &v) {
    if (s == "") return false;
    int i = 0, signo = 1; v = 0;
    if (s[0] == '-') { signo = -1; i = 1; if (s.size() == 1) return false; }
    for (; i < (int)s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
        v = v * 10 + (s[i] - '0');
    }
    v *= signo;
    return true;
}

NodoApp* buscarApp(NodoApp *catalogo, string nombre) {
    // TODO R1a
     NodoApp * actual = catalogo;

    while(actual != nullptr){
        if(actual -> nombre == nombre){
            return actual;
        }
        actual = actual->sig;
    }
    return nullptr;
}

bool agregarAlFinal(NodoApp *&catalogo, string nombre, string version, int mb, int prioridad) {
    // TODO R1b
    if (buscarApp(catalogo, nombre) != nullptr) {
        return false;
    }

    NodoApp *nuevo = crearApp(nombre, version, mb, prioridad);

    if (catalogo == nullptr) {
        catalogo = nuevo;
    }
    else {
        NodoApp *actual = catalogo;
         while (actual->sig != nullptr) {
            actual = actual->sig;
        }

        actual->sig = nuevo;
    }
    return true;
}

void encolarConPrioridad(NodoFila *&frente, NodoFila *&final, string nombre, int llegada, int prioridad) {
    // TODO R2a
    NodoFila *nuevo = new NodoFila;
    nuevo->nombre = nombre;
    nuevo->llegada = llegada;
    nuevo->prioridad = prioridad;
    nuevo->sig = nullptr;

    if (frente == nullptr) {
        frente = nuevo;
        final = nuevo;
    }
    else if (prioridad < frente->prioridad) {
        nuevo->sig = frente;
        frente = nuevo;
    }
    else {
        NodoFila *actual = frente;

        while (actual->sig != nullptr &&
                actual->sig->prioridad <= prioridad) {
            actual = actual->sig;
        }

        nuevo->sig = actual->sig;
        actual->sig = nuevo;

        if (nuevo->sig == nullptr) {
            final = nuevo;
        }
    }
}
void leerOrden(string linea, string &orden, string &a1, string &a2, string &a3, string &a4) {
    string t[8]; int n = 0;
    stringstream ss(linea);
    while (n < 8 && ss >> t[n]) n++;
    orden = a1 = a2 = a3 = a4 = "";
    if (n == 0) return;
    int k = 0;
    if (t[0] == "sudo") k = 1;
    string c = (k < n) ? t[k] : "";
    if (c == "apt" && k + 1 < n) {
        string s = t[k + 1];
        if (s == "list") orden = "catalogo";
        else if (s == "search") orden = "buscar";
        else if (s == "show") orden = "info";
        else if (s == "install") orden = "instalar";
        else if (s == "purge") orden = "retirar";
        else orden = "apt " + s;
        k += 2;
    } else if (c == "dnf" && k + 2 < n && t[k + 1] == "history" && t[k + 2] == "undo") {
        orden = "deshacer"; k = n;
    } else if (c == "dnf" && k + 1 < n && t[k + 1] == "history") {
        orden = "historial"; k = n;
    } else if (c == "dpkg" && k + 1 < n && t[k + 1] == "--get-selections") { orden = "guardar"; k = n; }
    else if (c == "dpkg" && k + 1 < n && t[k + 1] == "--set-selections") { orden = "cargar"; k = n; }
    else if (c == "df")   { orden = "espacio"; k = n; }
    else if (c == "man" || c == "help" || c == "--help") { orden = "ayuda"; k = n; }
    else if (c == "exit") { orden = "salir"; k = n; }
    else { orden = c; k++; }
    if (k < n) a1 = t[k];
    if (k + 1 < n) a2 = t[k + 1];
    if (k + 2 < n) a3 = t[k + 2];
    if (k + 3 < n) a4 = t[k + 3];
}

void ayuda() {
    cout << "  ORDEN (espanol)            EN LINUX                      QUE HACE" << endl;
    cout << "  catalogo                   apt list                      ver todas las apps" << endl;
    cout << "  buscar <texto>             apt search <texto>            buscar por nombre" << endl;
    cout << "  info <app>                 apt show <app>                ficha de una app" << endl;
    cout << "  agregar <app> <v> <MB> <p> -                             nueva app (p = 1, 2 o 3)" << endl;
    cout << "  prioridad <app> <p>        -                             cambiar la prioridad" << endl;
    cout << "  retirar <app>              sudo apt purge <app>          borrar del catalogo" << endl;
    cout << "  instalar <app> <seg>       sudo apt install <app> <seg>  pedir una app" << endl;
    cout << "  fila                       -                             ver la fila de descargas" << endl;
    cout << "  procesar                   -                             descargar la del frente" << endl;
    cout << "  historial                  dnf history                   bloques de disco (pila)" << endl;
    cout << "  deshacer                   dnf history undo last         liberar el ultimo bloque" << endl;
    cout << "  espacio                    df                            estado del disco" << endl;
    cout << "  guardar                    dpkg --get-selections         guardar en tuxstore.txt" << endl;
    cout << "  cargar                     dpkg --set-selections         cargar tuxstore.txt" << endl;
    cout << "  reporte                    -                             resumen del turno" << endl;
    cout << "  salir                      exit                          terminar" << endl;
}

int main() {
    NodoApp    *catalogo = nullptr;
    NodoFila   *frente = nullptr, *final = nullptr;
    NodoBloque *tope = nullptr;

    int reloj = 0, libre = DISCO, ultimaLlegada = 0;
    int instalaciones = 0, sumaEspera = 0, mbDescargados = 0;

    for (int i = 0; i < N_APPS; i++) agregarAlFinal(catalogo, fNombre[i], fVersion[i], fMb[i], fPrio[i]);

    cout << "Tux Store - servidor " << SERVIDOR << " (" << VELOCIDAD << " MB/s, "
         << DISCO << " MB). Escribe ayuda." << endl;

    string linea;
    while (true) {
        cout << "tux@" << SERVIDOR << ":~$ ";
        if (!getline(cin, linea)) break;
        string orden, a1, a2, a3, a4;
        leerOrden(linea, orden, a1, a2, a3, a4);
        if (orden == "") continue;
        cout << "> " << linea << endl;

        if (orden == "catalogo") {
    // TODO R1c
    NodoApp *actual = catalogo;

    while (actual != nullptr) {
        mostrarApp(actual);
        actual = actual->sig;
    }
}
        else if (orden == "buscar") {
            // TODO R1d

    if (a1 == "") {
        cout << "E: Ingresa un texto para buscar" << endl;
    } else {
        NodoApp *actual = catalogo;
        bool encontrado = false;

        while (actual != nullptr) {
            if (actual->nombre.find(a1) != string::npos) {
                mostrarApp(actual);
                encontrado = true;
            }
            actual = actual->sig;
        }

        if (!encontrado) {
            cout << "No se encontraron aplicaciones" << endl;
        }
    }
}

            else if (orden == "info") {
    // TODO R1e
    if (a1 == "") {
        cout << "Ingresa el nombre de una aplicacion" << endl;
    } else {
        NodoApp *app = buscarApp(catalogo, a1);

        if (app == nullptr) {
            cout << "E: Aplicacion no encontrada" << endl;
        } else {
            mostrarApp(app);
        }
    }
}
        else if (orden == "agregar") {
            // TODO R1f
            string nombre = a1;
            string version = a2;
            int mb = 0, prioridad = 0;

            bool mbValido = aEntero(a3, mb);
            bool prioridadValida = aEntero(a4, prioridad);

            if (nombre.empty() || version.empty() ||
                !mbValido || !prioridadValida ||
                mb <= 0 || prioridad < 1 || prioridad > 3) {
                cout << "Datos invalidos para agregar la aplicacion.\n";
            }
            else if (buscarApp(catalogo, nombre) != nullptr) {
                cout << "La aplicacion ya existe en el catalogo.\n";
            }
            else if (agregarAlFinal(catalogo, nombre, version, mb, prioridad)) {
                cout << "Aplicacion agregada: " << nombre << "\n";
            }
            else {
                cout << "No se pudo agregar la aplicacion.\n";
            }
        }
        
        else if (orden == "instalar") {
            // TODO R2b
            string nombre = a1;
            int llegada = 0;

            if (nombre.empty() || !aEntero(a2, llegada) || llegada < 0) {
                cout << "Datos invalidos para instalar.\n";
            }
            else {
                NodoApp *app = buscarApp(catalogo, nombre);

                if (app == nullptr) {
                    cout << "La aplicacion no existe en el catalogo.\n";
                }
                else if (app->estado != DISPONIBLE) {
                    cout << "La aplicacion ya esta en fila o instalada.\n";
                }
                else if (llegada < ultimaLlegada) {
                    cout << "La llegada no puede ser menor que la anterior.\n";
                }
                else {
                    encolarConPrioridad(frente, final, nombre,
                                        llegada, app->prioridad);

                    app->estado = EN_FILA;
                    ultimaLlegada = llegada;

                    cout << "Solicitud agregada a la fila: "
                         << nombre << "\n";
                }
            }
        }
        else if (orden == "fila") {
            // TODO R2c
            if (frente == nullptr) {
                cout << "La fila esta vacia.\n";
            }
            else {
                NodoFila *actual = frente;

                cout << "frente";

                while (actual != nullptr) {
                    cout << " -> " << actual->nombre
                         << "(" << actual->prioridad << ")";

                    actual = actual->sig;
                }

                cout << " <- final\n";
            }
        }
        else if (orden == "procesar") {
            // TODO R3
            if (frente == nullptr) {
                cout << "La fila esta vacia.\n";
            }
            else {
                //Obtenemos la primera solicitud
                NodoFila *atendido = frente;
                frente = frente->sig;

                if (frente == nullptr) {
                    final = nullptr;
                }

                //Buscamos la aplicacion en el catalogo
                NodoApp *app = buscarApp(catalogo, atendido->nombre);

                if (app == nullptr) {
                    cout << "Aplicacion no encontrada.\n";
                }
                else if (app->mb > libre) {
                    cout << "E: No hay espacio suficiente para "
                         << app->nombre << ": necesita "
                         << app->mb << " MB, libres "
                         << libre << " MB\n";

                    app->estado = DISPONIBLE;
                }
                else {
                    //Se calcula el tiempo de descarga
                    int descarga = (app->mb + VELOCIDAD - 1) / VELOCIDAD;

                    //Calculamos el inicio y fin
                    int inicio = reloj;

                    if (atendido->llegada > inicio) {
                        inicio = atendido->llegada;
                    }

                    int fin = inicio + descarga;
                    int espera = inicio - atendido->llegada;

                    //Se actualizan los datos
                    app->inicio = inicio;
                    app->fin = fin;
                    app->espera = espera;
                    app->estado = INSTALADA;

                    reloj = fin;
                    libre -= app->mb;

                    instalaciones++;
                    sumaEspera += espera;
                    mbDescargados += app->mb;

                    //Se registra la instalacion en la pila
                    NodoBloque *nuevo = new NodoBloque;
                    nuevo->nombre = app->nombre;
                    nuevo->mb = app->mb;
                    nuevo->sig = tope;
                    tope = nuevo;

                    cout << app->nombre
                         << ": descarga " << descarga << " s"
                         << " | inicio " << inicio
                         << " | fin " << fin
                         << " | espera " << espera
                         << " | libres " << libre << " MB\n";
                }

                //Se libera el nodo atendido de la cola
                delete atendido;
            }
        }
        else if (orden == "historial") {
            // TODO R4a
        }
        else if (orden == "deshacer") {
            // TODO R4b
        }
        else if (orden == "espacio") {
            // TODO R4c
        }
        else if (orden == "prioridad") {
            // TODO R5a
        }
        else if (orden == "retirar") {
            // TODO R5b
        }
        else if (orden == "guardar") {
            if (frente != nullptr) cout << "E: Procesen la fila antes de guardar" << endl;
            else {
                ofstream f(ARCHIVO);
                int n = 0, m = 0;
                // TODO R6a
                f.close();
                cout << "Guardado: " << n << " apps y " << m << " bloques en " << ARCHIVO << endl;
            }
        }
        else if (orden == "cargar") {
            ifstream f(ARCHIVO);
            if (!f) cout << "E: No existe " << ARCHIVO << endl;
            else if (frente != nullptr) cout << "E: Procesen la fila antes de cargar" << endl;
            else {
                int n = 0, m = 0;
                // TODO R6b
                cout << "Cargado: " << n << " apps y " << m << " bloques | libres " << libre
                     << " MB | turno nuevo" << endl;
            }
        }
        else if (orden == "reporte") {
            // TODO R7a
        }
        else if (orden == "ayuda") ayuda();
        else if (orden == "salir") break;
        else cout << orden << ": orden no encontrada (escribe ayuda)" << endl;
    }

    int liberados = 0;
    // TODO R7b
    cout << "Nodos liberados: " << liberados << ". Hasta pronto." << endl;
    return 0;
}
