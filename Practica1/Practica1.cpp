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
			for(int j = 0; j < abecedario.size(); j++){
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

				if(indice == cadena.size()){
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

//Este cuenta como el AFN
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
		for(int i = 0; i < posicion_ors.size(); i++){
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

set<Nodo*> estadoCerraduraEpsilon(set<Nodo*> conjunto){
	stack<Nodo*> pila;
	vector<bool> visitados(100000, false);

	set<Nodo*> cerraduraEpsilon;
	for(auto nodo : conjunto){
		visitados[nodo->getIndice()] = true;
		cerraduraEpsilon.insert(nodo);
		pila.push(nodo);
	}

	while(!pila.empty()){
		Nodo* actual = pila.top();
		pila.pop();

		cerraduraEpsilon.insert(actual);

		for(int i = 0; i < actual->getSize(); ++i){
			if(actual->getTransicion(i) == '?'){		
				if(visitados[actual->getHijo(i)->getIndice()]) continue;
				visitados[actual->getHijo(i)->getIndice()] = true;
				pila.push(actual->getHijo(i));
			}
		}
	} return cerraduraEpsilon;
}

//Este cuenta como el AFD
Nodo* TransformaAFN(Nodo* AFN, const string& abecedario, Nodo* nodo_final){
	int indice = 0;
	
	Nodo* AFD = new Nodo(indice++, false);

	//kernel, nodo
	map<set<Nodo*>, Nodo*> visitados;
	stack< pair<set<Nodo*>, Nodo*> > pila;

	set<Nodo*> conjuntoEAFN; conjuntoEAFN.insert(AFN);
	
	pila.push(make_pair(conjuntoEAFN, AFD));
	visitados[pila.top().first] = pila.top().second;
	
	while(!pila.empty()){
		set<Nodo*> kernelActual = pila.top().first;
		Nodo* nodo_actual = pila.top().second;
		pila.pop();

		set<Nodo*> conjuntoEpsilonActual = estadoCerraduraEpsilon(kernelActual);
		for(auto nodo : conjuntoEpsilonActual){
			if(nodo->getIndice() == nodo_final->getIndice()){
				nodo_actual->cambiaEstadoFinal(true); break;
			}
		}

		for(int k = 0; k < abecedario.size(); ++k){
			bool huboTransicion = false;
			set<Nodo*> nuevoConjunto;
			for(auto nodo: conjuntoEpsilonActual){
				for(int j = 0; j < nodo->getSize(); ++j){
					if(nodo->getTransicion(j) == abecedario[k]){
						nuevoConjunto.insert(nodo->getHijo(j));
						huboTransicion = true;
					}
				}
			}

			if(!huboTransicion) continue;

			Nodo* nodo_creado;
			if(visitados.find(nuevoConjunto) == visitados.end()){
				nodo_creado = new Nodo(indice++, false);
				visitados[nuevoConjunto] = nodo_creado;
				pila.push(make_pair(nuevoConjunto, nodo_creado));
			} else {
				nodo_creado = visitados[nuevoConjunto];
			} nodo_actual->agregaArista(nodo_creado, abecedario[k]);
		}	
	}
	return AFD;
}

int main (){
    return 0;
}
