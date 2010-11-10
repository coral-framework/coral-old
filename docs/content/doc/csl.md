---
title: Coral Specification Language (CSL)
---

Coral Specification Language (CSL)
==================================

Vis�o Geral
-----------

A linguagem CSL (de _Coral Specification Language_) � uma forma de especificar interfaces e componentes no Coral. A linguagem permite a especifica��o de novos tipos, segundo o [sistema de tipos](types.html) do Coral, e ainda auxilia na implementa��o de componentes est�ticos.

Os tipos especificados em CSL s�o mapeados para C++ segundo as regras de [mapeamento de CSL para C++](mapping.html), utilizando o [compilador do Coral](compiler.html).


Exemplos B�sicos
----------------

A linguagem CSL permite seis tipos de especifica��es:

- **enum** - uma enumera��o de identificadores constantes, com valores impl�citos de 0 a (n-1).
- **exception** - uma exce��o, identificada apenas por nome, que pode ser lan�ada por m�todos.
- **struct** - estrutura contendo um conjunto de campos. Cada campo possui um tipo e um nome.
- **interface** - define a assinatura de um servi�o de um componente. Uma interface cont�m um conjunto de m�todos e atributos, cada qual com nomes distintos e que n�o causam conflitos. Opcionalmente, uma interface pode herdar de outras interfaces (permite-se heran�a m�ltipla).
- **native class** - permite que uma classe nativa do C++ seja "exportada" para o Coral, criando um novo tipo de valor opaco. Uma classe nativa cont�m um conjunto de m�todos e atributos &mdash; assim como uma interface, por�m as classes nativas n�o possuem heran�a. Para toda classe nativa, � obrigat�rio que se implemente um componente _wrapper_, respons�vel por fornecer reflex�o para o tipo.
- **component** - uma lista de interfaces providas e/ou requeridas por uma certa implementa��o de componente. Este tipo de especifica��o geralmente � mantido como um arquivo privado, pois s� � �til na implementa��o de um m�dulo e n�o precisa ser redistribu�do.

Cada especifica��o deve ser feita em um arquivo separado. A extens�o usada para estes arquivos � `.csl`. O nome do arquivo deve corresponder ao nome do tipo que � especificado no arquivo, enquanto que o _namespace_ do tipo � definido pela localiza��o do arquivo dentro do `CORAL_PATH` (vide a se��o "Tipos" da p�gina de [fundamentos](fundamentals.html).

Uma especifica��o em CSL pode fazer refer�ncia a um tipo especificado em outro arquivo CSL. Para este efeito, dois tipos de identificadores podem ser usados: o simples e o qualificado. Um identificador qualificado corresponde ao _nome completo_ do tipo (i.e. o caminho absoluto de _namespaces_ at� o tipo, separado por pontos). Um identificador simples corresponde apenas ao nome de um tipo, e pode ser usado para citar tipos no mesmo _namespace_. Outra possibilidade � que um identificador qualificado seja "importado", tornando-se um identificador simples no escopo de um arquivo CSL. Neste caso, os identificadores importados t�m preced�ncia sobre os tipos no mesmo _namespace_.

A especifica��o CSL abaixo define um `enum`, chamado `CoffeeSpecies`, que cont�m tr�s constantes com valores impl�citos de zero a dois.

<code lang="csl">
// file = coffee/beans/CoffeeSpecies.csl
enum CoffeeSpecies
{
	CoffeaArabica,
	CoffeaCanephora,
	CoffeaIberica
};
</code>

A especifica��o CSL abaixo define uma `struct`, chamada `RoastedCoffeeBean`, que cont�m dois campos. O primeiro campo, `species`, � do tipo definido no exemplo anterior. Como os dois tipos encontram-se no mesmo _namespace_, n�o � necess�rio usar um identificador qualificado.

<code lang="csl">
// file = coffee/beans/RoastedCoffeeBean.csl
struct RoastedCoffeeBean
{
	CoffeeSpecies species;
	int8 degreeOfRoast;
};
</code>

A especifica��o CSL abaixo define outra `struct`, `CoffeeBlend`, que cont�m tr�s campos. O segundo campo, `beans`, � um `array` do tipo definido no exemplo anterior. Como os tipos encontram-se em _namespaces_ diferentes, desta vez � necess�rio usar um identificador qualificado.

<code lang="csl">
// file = coffee/blends/CoffeeBlend.csl
struct CoffeeBlend
{
	string name;
	coffee.beans.RoastedCoffeeBean[] beans;
	double[] percentages;
};
</code>

O exemplo anterior pode ser re-escrito de forma a "importar" o tipo `RoastedCoffeeBean`, ao inv�s de citar o seu nome completo. Este recurso � especialmente �til quando um tipo � citado diversas vezes no mesmo arquivo.

<code lang="csl">
// file = coffee/blends/CoffeeBlend.csl
import coffee.beans.RoastedCoffeeBean;

struct CoffeeBlend
{
	string name;
	RoastedCoffeeBean[] beans;
	double[] percentages;
};
</code>

A especifica��o CSL abaixo define uma `interface`, chamada `ICoffeeMachine`, que cont�m dois atributos e dois m�todos. O primeiro atributo � _readonly_, e o segundo � _read/write_. Ambos os m�todos da interface declaram lan�ar exce��es. O �ltimo m�todo recebe um par�metro `inout`, o que indica que o par�metro � usado tanto para passar um argumento quanto para receber um valor de retorno.

<code lang="csl">
// file = coffee/ICoffeeMachine.csl
interface ICoffeeMachine
{
	readonly attribute bool turnedOn;
	attribute CoffeeBlend blend;

	void togglePower() raises PowerOutageException;

	void prepareCoffee( inout float liters )
		raises NotEnoughIngredientsException, PowerOutageException;
};
</code>


Descri��o da Gram�tica
----------------------

\subsection csl_grammar_identifiers Identificadores

O elemento mais b�sico da linguagem CSL s�o os identificadores. Como explicado acima, existem dois tipos de identificadores: o simples e o qualificado. Um identificador simples � uma _string_ ASCII, que come�a obrigatoriamente com uma letra, e pode conter somente letras, n�meros e o car�cter *underscore ('_')*. Um identificador qualificado � uma lista de identificadores simples, conectados por pontos.

<code lang="ebnf">
qualified-identifier = Identifier , { "." , Identifier } ;
</code>

### Arquivos CSL

Um arquivo CSL � composto por uma lista opcional de cl�usulas de importa��o de tipos, seguida por **uma �nica** especifica��o.

<code lang="ebnf">
csl           = { import-clause } , specification ;

import-clause = "import" , qualified-identifier , ";" ;

specification = enum-specification         |
				exception-specification    |
				struct-specification       |
				native-class-specification |
				interface-specification    |
				component-specification    ;
</code>

### Declara��o de Tipos

A declara��o de tipos � um elemento comum da linguagem CSL, que surge como parte da declara��o de atributos e m�todos. Um tipo pode ser declarado como sendo de um "tipo base" oferecido pelo Coral, ou ent�o de um tipo especificado em outro arquivo CSL. Em qualquer caso, o tipo � considerado um `array` caso receba um par de colchetes como sufixo.

<code lang="ebnf">
type-declaration = basic-type , [pair-of-braces]            |
				   qualified-identifier , [pair-of-braces]  ;

pair-of-braces   = "[" , "]" ;

basic-type       = "any"                 |
				   "bool"                |
				   "(u?)int(8|16|32|64)" |
				   "float"               |
				   "double"              |
				   "string"              ;
</code>

### Coment�rios e Documenta��o

Coment�rios estilo C/C++ podem aparecer imediatamente antes de uma especifica��o, declara��o de atributo, m�todo, etc. Os coment�rios _n�o_ s�o ignorados pelo compilador CSL, e _n�o_ podem aparecer em qualquer lugar. � proibido, por exemplo, incluir um coment�rio entre o in�cio da declara��o de um m�todo e o ";" que encerra a declara��o.

Sempre que poss�vel o compilador aproveitar� os coment�rios em CSL como documenta��o dos tipos. Em especial, o compilador tentar� transferir a documenta��o para os _mappings_ gerados em C++, na forma de coment�rios Doxygen.

A princ�pio, considera-se que todo coment�rio serve para documentar o elemento imediatamente a seguir � sua posi��o. Por�m, em alguns casos pode ser �til incluir o coment�rio logo ap�s o elemento a ser documentado. Nestes casos, deve-se usar o sinal "<" como primeiro car�ter do coment�rio, para indicar que ele se refere ao �ltimo elemento escrito, e n�o ao pr�ximo.

<code lang="csl">
/*
	This documents the enum 'CaseSensitivity'.
 */
enum CaseSensitivity
{
	CaseSensitive,		//< This documents the constant 'CaseSensitive'
	CaseInsensitive,	//< This documents the constant 'CaseInsensitive'
};
</code>

### Como especificar uma Enumera��o (enum)

A gram�tica para especificar um `enum` � fornecida abaixo. Um `enum` deve ter pelo menos um identificador. N�o � poss�vel atribuir valores aos identificadores &mdash; os valores s�o sempre de 0 a (n-1).

<code lang="ebnf">
enum-specification = "enum" , Identifier , "{" , identifier-list , "}" , ";" ;

identifier-list    = Identifier , { "," Identifier } ;
</code>

Exemplo de `enum`:

<code lang="csl">
enum EventType
{
	MouseEvent,
	KeyboardEvent
};
</code>


### Como especificar uma Exce��o (exception)

A gram�tica para especificar um `exception` � fornecida abaixo. Um `exception` � identificado apenas pelo nome, e na vers�o atual do Coral n�o tem atributos nem heran�a.

<code lang="ebnf">
exception-specification = "exception" , Identifier , ";" ;
</code>

Exemplo de `exception`:

<code lang="csl">
exception IllegalNameException;
</code>


### Como especificar uma Estrutura (struct)

A gram�tica para especificar uma `struct` � fornecida abaixo. Uma `struct` pode conter um ou mais atributos (campos), de qualquer tipo.

<code lang="ebnf">
struct-specification = "struct" , Identifier , "{" , { struct-member } , "}" , ";" ;

struct-member        = type-declaration , Identifier , ";" ;
</code>

Exemplo de `struct`:

<code lang="csl">
struct Sphere
{
	Vec3 center;
	double radius;
};
</code>

### Como especificar uma Classe Nativa (native class)

A gram�tica para especificar uma `native class` � fornecida abaixo. Al�m do nome do tipo no Coral, � necess�rio fornecer &mdash; entre par�nteses &mdash; o nome do tipo nativo em C++ (`CppType`) e o header onde ele est� definido (`CppHeader`). Aceita-se como `CppType` qualquer identificador C++ com separa��o de escopo (e.g. `nmspc::TypeName`), e como `CppHeader` qualquer _string_ entre os sinais de "<" e ">".

<code lang="ebnf">
native-class-specification = "native" , "class" , Identifier ,
                                 "(" , CppHeader , CppType , ")" ,
                                 "{" , { native-class-member } , "}" , ";" ;

native-class-member        = attribute-declaration | method-declaration ;
</code>

Um `native class` pode ter dois tipos de membros: atributos e m�todos. O nome de um atributo deve come�ar obrigatoriamente com uma letra min�scula. Note tamb�m que o Coral n�o suporta _overload_ de m�todos &mdash; o nome de um m�todo deve ser �nico, de modo que n�o cause conflitos com outros m�todos ou com os \e accessors gerados para os atributos (leia mais na p�gina de [mapeamento de CSL para C++](mapping.html)).

<code lang="ebnf">
attribute-declaration      = ["readonly"] , "attribute" , type-declaration , Identifier , ";" ;

method-declaration         = method-type-declaration , Identifier ,
                                 "(" , [parameter-declaration-list] , ")" , ")" , [exception-list] , ";" ;

method-type-declaration    = "void" | type-declaration ;

parameter-declaration-list = parameter-declaration , { "," , parameter-declaration } ;

parameter-declaration      = ( "in" | "out" | "inout" ) , type-declaration , Identifier ;

exception-list             = "raises" , qualified-identifier , { "," , qualified-identifier } ;
</code>

Exemplo de `native class`:

<code lang="csl">
native class Vec3 ( <vr/Vec3.h> vr::Vec3 )
{
	attribute double x;
	attribute double y;
	attribute double z;

	readonly attribute double length;

	double normalize();
	void add( in Vec3 v );
};
</code>


### Como especificar uma Interface (interface)

A gram�tica para especificar uma `interface` � fornecida abaixo. As declara��es de atributos e m�todos seguem a mesma gram�tica de uma `native class`. Por�m, diferente das classes nativas, as interfaces podem ter heran�a e um novo tipo de membro: blocos de c�digo C++, que s�o copiados _verbatim_ para os _mappings_ da interface (mais informa��es na p�gina de [mapeamento de CSL para C++](mapping.html)).

<code lang="ebnf">
interface-specification = "interface" , Identifier , [inheritance-declaration] ,
                              "{" , { interface-member } , "}" , ";" ;

inheritance-declaration = ":" , qualified-identifier , { "," , qualified-identifier } ;

interface-member        = cpp-block | attribute-declaration | method-declaration ;

cpp-block               = "<c++" , ? C++ code ? , "c++>"
</code>

Note que o uso de heran�a &mdash; e, especialmente, heran�a m�ltipla &mdash; est� sujeito a v�rios tipos de erros. O compilador verifica se h� heran�as c�clicas ou desnecessariamente redundantes. E ainda, se h� conflitos entre nomes de m�todos e atributos de interfaces herdadas. Para evitar conflitos, uma classe A s� pode herdar das classes B e C caso o conjunto dos nomes dos membros (de A, B e C) seja totalmente disjunto.

Exemplo de `interface`:

<code lang="csl">
interface ILoyaltyProgram
{
	attribute int32 totalPoints;
	readonly attribute Date expirationDate;

	void addPoints( in int32 points );
	void consumePoints( in int32 points )
		raises InsufficientPointsException;
};
</code>

### Como especificar um Componente (component)

Finalmente, a gram�tica para especificar um componente � fornecida abaixo.

<code lang="ebnf">
interface-specification = "component" , Identifier ,
                              "{" , { component-member } , "}" , ";" ;

component-member        = ( "provides" | "receives" ) , qualified-identifier , Identifier , ";" ;
</code>

Exemplo de `component`:

<code lang="csl">
component HotelReservation
{
	provides IHotelRes;
	receives ILoyaltyProgram;
	receives IBilling;
};
</code>
