#include <bits/stdc++.h>
using namespace std;

class Nodo{
	int index;
	bool es_final;
	vector< pair<Nodo*, char> > aristas;
	public:
		void cambiaEstadoFinal(bool cambia){ es_final = cambia; }

		void defineIndice(int i){ index = i; }

		pair<Nodo*, char> getArista(int i){ return aristas[i]; }

		int getSize(){ return aristas.size(); }

		Nodo* getHijo(int i){ return aristas[i].first; }

		int getEstadoFinal(){ return es_final; }

		char getTransicion(int i){ return aristas[i].second; }

		int getIndice(){ return index; }

		void agregaArista(Nodo* v, char transicion){
			aristas.push_back(make_pair(v, transicion));
		}

		Nodo(){
			index = -1;
			es_final = false;
		}

		Nodo(int indice, bool final){
			index = indice;
			es_final = final;
		}
};

class Automata{
	int numero_nodos;
	int estado_inicial;

	string abecedario;
	string tipo_automata;

	Nodo* nodo_inicial;

	Nodo* buscaNodo(int indice_u){
		stack< Nodo* > pilaRecursiva;
		pilaRecursiva.push(nodo_inicial);

		vector<bool> visitados(numero_nodos, false);
		visitados[nodo_inicial -> getIndice()] = true;
		while(!pilaRecursiva.empty()){
			Nodo* actual = pilaRecursiva.top();
			pilaRecursiva.pop();

			if(actual->getIndice() == indice_u) return actual;

			for(int i = 0; i < actual->getSize(); i++){
				if(!visitados[actual->getHijo(i)->getIndice()]){
					visitados[actual->getHijo(i)->getIndice()] = true;
					pilaRecursiva.push(actual->getHijo(i));
				}
			}
		} return NULL;
	}

	bool Verifica(Nodo* u, char transicion){
		for(int i = 0; i < u -> getSize(); i++){
			if(u -> getTransicion(i) == transicion){
				printf("Recuerda, no puede haber ambiguedad en un AFD\n");
				return false;
			}
		} return true;
	}

	void DefineNumeroEstados(int numero_estados){
		abecedario = "";
		numero_nodos = numero_estados;
	}

	void DefineAbecedario(string abecedario_recibido){
		abecedario += abecedario_recibido;
		if(tipo_automata == "AFN") abecedario += "?";
	}

	void DefineTransiciones(int transiciones){
		printf("Definir automatada...recuerda, el numero de nodos/estados, esta indexado desde 0\n");
		for(int i = 0; i < transiciones; i++){
			char transicion;
			int indice_u, indice_v;
			printf("%d ) Ingresa 2 enteros, u, v, para expresar que u esta conectado con v e ingresa un caracter para definir la transicion\n>", i);
			cin >> indice_u >> indice_v >> transicion;
			if(indice_u >= numero_nodos || indice_v >= numero_nodos){
                printf("Ese indice de nodo no existe en el automata, intentalo de nuevo\n");
				i--; continue;
			}

			Nodo* u = buscaNodo(indice_u);
			Nodo* v = buscaNodo(indice_v);
			if(u == NULL) u = new Nodo(indice_u, false);
			if(v == NULL) v = new Nodo(indice_v, false);

			bool existe = false;
			for(int j = 0; j < (int)abecedario.size(); j++){
				if(abecedario[j] == transicion) existe = true;
			} if(!existe) { printf("El caracter que ingresaste no existe en el abecedario\n"); i--; continue; }

			if(tipo_automata == "AFD"){
				if(!Verifica(u, transicion)){ i--; continue; }
			}

			u -> agregaArista(v, transicion);
		}
	}

	void DefineEstadosFinales(int estados_finales){
		for(int i = 0; i < estados_finales; i++){
			int indice_u;
			printf("%d) Ingresa un entero u, que definira que estado/nodo sera final\n>", i);
			cin >> indice_u;
			Nodo* u = buscaNodo(indice_u);
			u -> cambiaEstadoFinal(true);
		}
	}

	void DefineEstadoInicial(int estado_inicial_param){
		nodo_inicial = new Nodo(estado_inicial_param, false);
	}

	void DefineTipoAutomata(){
		string query;
		printf("El automata sera AFD o AFN?(AFD, AFN)\n>");
		cin >> tipo_automata;
	}

	public:
		Automata(int numero_estados, string abecedario_recibido, int transiciones, int numero_estados_finales, int est_inicial){
			DefineEstadoInicial(est_inicial);
			DefineNumeroEstados(numero_estados);
			DefineTipoAutomata();
			DefineAbecedario(abecedario_recibido);
			DefineTransiciones(transiciones);
			DefineEstadosFinales(numero_estados_finales);
		}

		Automata(Nodo* grafo, string abecedario_recibido, string tipo_AF){
			abecedario_recibido = abecedario;
			nodo_inicial = grafo;
			tipo_automata = tipo_AF;
		}

		bool VerificaCadena(string cadena){
			bool resultado = false;

			stack< pair<Nodo*, int> > pila;
			pila.push( make_pair(nodo_inicial, 0) );

			while(!pila.empty()){
				Nodo* estado_actual = pila.top().first;
				int indice = pila.top().second;
				pila.pop();

				if(indice == (int)cadena.size()){
					if(estado_actual->getEstadoFinal()){
						resultado = true;
					} else {
						continue;
					}
				}

				for(int i = 0; i < estado_actual->getSize(); i++){
					if(estado_actual->getTransicion(i) == cadena[indice]){
						pila.push(make_pair(estado_actual->getHijo(i), indice + 1));
					}

					if(estado_actual->getTransicion(i) == '?'){
						pila.push(make_pair(estado_actual->getHijo(i), indice));
					}
				}
			} return resultado;
		}
};

typedef string ExpresionRegular;
typedef pair<Nodo*, Nodo*> AF;

int encuentraInicioParentesis(const ExpresionRegular& expresionRegular, int posicion_parentesis){
	int cerrados = 0; int resultado;
	for(int i = posicion_parentesis; i >= 0; i--){
		if(expresionRegular[i] == '\''){
			if(expresionRegular[i - 1] == ')') cerrados++;                  																		
			if(expresionRegular[i - 1] == '('){ 																									
				cerrados--; if(cerrados == 0) { resultado = i; break; }
			}
		}
	}
	
	return resultado;
}

AF ExpresionRegularAAFN(const ExpresionRegular& expresionRegular, int inicio, int final, int& numero_indice){
	vector<int> posicion_ors;
	for(int i = final; i >= inicio; i--){
		if(expresionRegular[i] == '\'' && expresionRegular[i - 1] == ')')                                                                 
			i = encuentraInicioParentesis(expresionRegular, i) - 1;

		if(expresionRegular[i] == '\'' && expresionRegular[i - 1] == '|'){ 
			posicion_ors.push_back(i - 1);             
		}      				  
	}

	Nodo* nodo_inicio = new Nodo(numero_indice++, false);
	Nodo* nodo_final = new Nodo(numero_indice++, false);

	if(posicion_ors.size() > 0){
		int actual_inicio = final;
		posicion_ors.push_back(inicio - 2);
		for(int i = 0; i < (int)posicion_ors.size(); i++){
			AF actual = ExpresionRegularAAFN(expresionRegular, posicion_ors[i] + 2, actual_inicio, numero_indice); 
			actual.second->cambiaEstadoFinal(false);

			nodo_inicio->agregaArista(actual.first, '?'); actual.second->agregaArista(nodo_final, '?');
			actual_inicio = posicion_ors[i] - 1;
		}
		nodo_final->cambiaEstadoFinal(true);
		return AF(nodo_inicio, nodo_final);
	}

	Nodo* actual = nodo_final; 
	nodo_final->cambiaEstadoFinal(true);
	for(int i = final; i >= inicio; i--){
		if((expresionRegular[i] >= '(' && expresionRegular[i] <= 'z') || expresionRegular[i] == ' ' || expresionRegular[i] == '%'){
			Nodo* nuevo_nodo = new Nodo(numero_indice++, false);
			nuevo_nodo->agregaArista(actual, expresionRegular[i]);
			actual = nuevo_nodo;
		}

		if(expresionRegular[i] == '\'' && (expresionRegular[i - 1] == '+' || expresionRegular[i - 1] == '*')){                                                          
			if(i - 2 < 0) continue;
			Nodo* inicio_cerradura = new Nodo(numero_indice++, false);
			if( (expresionRegular[i - 2] >= '(' && expresionRegular[i - 2] <= 'z') || expresionRegular[i - 2] == ' ' || expresionRegular[i - 2] == '%'){
				Nodo* temporal_inicio = new Nodo(numero_indice++, false);
				Nodo* temporal_final = new Nodo(numero_indice++, false);
 
				temporal_inicio->agregaArista(temporal_final, expresionRegular[i - 2]);
				temporal_final->agregaArista(actual, '?'); 
				
				inicio_cerradura->agregaArista(temporal_inicio,'?');

				temporal_final->agregaArista(temporal_inicio, '?');
			}

			bool esClinton = (expresionRegular[i - 1] == '*')? true : false;

			bool entroRecursion = false;
			if(expresionRegular[i - 2] == '\'' && expresionRegular[i - 3] == ')'){                                                                                                   
				entroRecursion = true;
				int posicion_parentesis_abierto = encuentraInicioParentesis(expresionRegular, i - 2);
				AF parentesis = ExpresionRegularAAFN(expresionRegular, posicion_parentesis_abierto + 1, i - 4, numero_indice);
				
				parentesis.second->agregaArista(actual, '?');
				parentesis.second->cambiaEstadoFinal(false);

				inicio_cerradura->agregaArista(parentesis.first, '?');
				parentesis.second->agregaArista(parentesis.first, '?');

				i = posicion_parentesis_abierto - 1;
			}

			if(expresionRegular[i - 1] == '*' || (entroRecursion && esClinton)){                                                                                   
				inicio_cerradura->agregaArista(actual, '?');
			}

			actual = inicio_cerradura;

			if(expresionRegular[i - 1] != '\'' && ((expresionRegular[i - 2] >= '(' && expresionRegular[i - 2] <= 'z') || expresionRegular[i - 2] == ' ' || expresionRegular[i - 2] == '%')) i = i - 2;
		}

		if(expresionRegular[i] == '\'' && expresionRegular[i - 1] == ')'){  																					
			int posicion_parentesis_abierto = encuentraInicioParentesis(expresionRegular, i);
			AF parentesis = ExpresionRegularAAFN(expresionRegular, posicion_parentesis_abierto + 1, i - 2, numero_indice);	
			parentesis.second->cambiaEstadoFinal(false);
			i = posicion_parentesis_abierto - 1;

			if(actual != nodo_final){
				for(int k = 0; k < actual->getSize(); k++){
					parentesis.second -> agregaArista(actual->getHijo(k), actual->getTransicion(k));
				}
			} else{
				nodo_final = parentesis.second;
			} delete actual; actual = parentesis.first;
		}
	}
	return AF(actual, nodo_final);
}

map <Nodo *, bool> vis;
void bfs (Nodo * nodoIni){
	vis[nodoIni] = true;
	queue<Nodo *> colaBFS;

	colaBFS.push(nodoIni);

	while(!colaBFS.empty()){

		Nodo * actual = colaBFS.front();
		colaBFS.pop();

		for(int i = 0; i < actual->getSize(); i++){
			//Para la impresion en formato dot
			cout << "\t" << actual->getIndice() << " -> " << actual->getHijo(i)->getIndice() << " [label = \"" << actual->getTransicion(i) << "\"]\n"; 
			Nodo * u = actual->getArista(i).first; 
			if(!vis[u]){
				vis[u] = true;
				colaBFS.push(u);
			} 
		} 

	}
}

//Despues de hacer pruebas me di cuenta que propuse una sintaxis con comillas simples
//para que fuera mas sencilla la implementacion

const string p1 = "('a|'b)'";
const string p2 = "('a|'b)'|('a'b|'c)'";
const string prueba = "('a'+|' )''a'b('c'a|'c'+d)'";

const string origial = "(a+|E)ab(ca|c+d)";
const string formateada = "('a'+|' )''a'b('c'a|'c'+d)'";

//Para darle formato a nuestra expresion regular, la expresion regular tiene que ser valida
string formatearExpresionRegular(const string& expresionRegular){

	string expresionFormateada = "";
	for( int i = 0; i < (int)expresionRegular.size(); i++ ){

		if(i == 0){
			
			if(expresionRegular[i] >= 'a' && expresionRegular[i] <= 'z'){
				expresionFormateada = '\'' + expresionRegular[i];
			} else if (expresionRegular[i] == 'E') {
				expresionFormateada = '\'' + ' ';
			} else {
				expresionFormateada += expresionRegular[i];
			}

		} else {

			if(expresionRegular[i] >= 'a' && expresionRegular[i] <= 'z'){
			
				expresionFormateada.push_back('\'');
				expresionFormateada.push_back(expresionRegular[i]);
			
			} else if (expresionRegular[i] == 'E') {

				expresionFormateada.push_back('\'');
				expresionFormateada.push_back(' ');
			
			} else if (expresionRegular[i] == '+'){

				expresionFormateada.push_back('\'');
				expresionFormateada.push_back(expresionRegular[i]);
				expresionFormateada.push_back(expresionRegular[i+1]);
				i++;
				
			} else if (expresionRegular[i] == '*'){

				expresionFormateada.push_back('\'');
				expresionFormateada.push_back('*');

			} else if(expresionRegular[i] == ')'){

				expresionFormateada.push_back(')');
				expresionFormateada.push_back('\'');

			} else {
				expresionFormateada.push_back(expresionRegular[i]);
			}

		}

	}
	return expresionFormateada;
}

string origin = "(a+|E)ab(ca|c+d)";
string format = "('a'+|' )''a'b('c'a|'c'+d)'";

int main (){

	string expresionRegular;
	cin>>expresionRegular;

	// expresionRegular = origin;

	string expresionFormateada = formatearExpresionRegular(expresionRegular);

	cout << "# Expresion original: " << expresionRegular << endl;
	cout << "# Expresion formateada: " << expresionFormateada << endl;

	int idx = 0;
	auto afn = ExpresionRegularAAFN(expresionFormateada, 0, expresionFormateada.size() -1, idx);

    //Funcion de impresion mediante una bfs, se imprime en el formato que pide el .dot
	cout << "digraph AFN {\n";
	cout << "rankdir = LR\n"; // Grafo de izquierda a derecha
	bfs(afn.first);
    cout << "}";

    return 0;
}
