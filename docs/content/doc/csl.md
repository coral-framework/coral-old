---
title: Coral Specification Language (CSL)
---

Coral Specification Language (CSL)
==================================

Visão Geral
-----------

A linguagem CSL (de _Coral Specification Language_) é uma forma de especificar interfaces e componentes no Coral. A linguagem permite a especificação de novos tipos, segundo o [sistema de tipos](types.html) do Coral, e ainda auxilia na implementação de componentes estáticos.

Os tipos especificados em CSL são mapeados para C++ segundo as regras de [mapeamento de CSL para C++](mapping.html), utilizando o [compilador do Coral](compiler.html).


Exemplos Básicos
----------------

A linguagem CSL permite seis tipos de especificações:

- **enum** - uma enumeração de identificadores constantes, com valores implícitos de 0 a (n-1).
- **exception** - uma exceção, identificada apenas por nome, que pode ser lançada por métodos.
- **struct** - estrutura contendo um conjunto de campos. Cada campo possui um tipo e um nome.
- **interface** - define a assinatura de um serviço de um componente. Uma interface contém um conjunto de métodos e atributos, cada qual com nomes distintos e que não causam conflitos. Opcionalmente, uma interface pode herdar de outras interfaces (permite-se herança múltipla).
- **native class** - permite que uma classe nativa do C++ seja "exportada" para o Coral, criando um novo tipo de valor opaco. Uma classe nativa contém um conjunto de métodos e atributos &mdash; assim como uma interface, porém as classes nativas não possuem herança. Para toda classe nativa, é obrigatório que se implemente um componente _wrapper_, responsável por fornecer reflexão para o tipo.
- **component** - uma lista de interfaces providas e/ou requeridas por uma certa implementação de componente. Este tipo de especificação geralmente é mantido como um arquivo privado, pois só é útil na implementação de um módulo e não precisa ser redistribuído.

Cada especificação deve ser feita em um arquivo separado. A extensão usada para estes arquivos é `.csl`. O nome do arquivo deve corresponder ao nome do tipo que é especificado no arquivo, enquanto que o _namespace_ do tipo é definido pela localização do arquivo dentro do `CORAL_PATH` (vide a seção "Tipos" da página de [fundamentos](fundamentals.html).

Uma especificação em CSL pode fazer referência a um tipo especificado em outro arquivo CSL. Para este efeito, dois tipos de identificadores podem ser usados: o simples e o qualificado. Um identificador qualificado corresponde ao _nome completo_ do tipo (i.e. o caminho absoluto de _namespaces_ até o tipo, separado por pontos). Um identificador simples corresponde apenas ao nome de um tipo, e pode ser usado para citar tipos no mesmo _namespace_. Outra possibilidade é que um identificador qualificado seja "importado", tornando-se um identificador simples no escopo de um arquivo CSL. Neste caso, os identificadores importados têm precedência sobre os tipos no mesmo _namespace_.

A especificação CSL abaixo define um `enum`, chamado `CoffeeSpecies`, que contém três constantes com valores implícitos de zero a dois.

<code lang="csl">
// file = coffee/beans/CoffeeSpecies.csl
enum CoffeeSpecies
{
	CoffeaArabica,
	CoffeaCanephora,
	CoffeaIberica
};
</code>

A especificação CSL abaixo define uma `struct`, chamada `RoastedCoffeeBean`, que contém dois campos. O primeiro campo, `species`, é do tipo definido no exemplo anterior. Como os dois tipos encontram-se no mesmo _namespace_, não é necessário usar um identificador qualificado.

<code lang="csl">
// file = coffee/beans/RoastedCoffeeBean.csl
struct RoastedCoffeeBean
{
	CoffeeSpecies species;
	int8 degreeOfRoast;
};
</code>

A especificação CSL abaixo define outra `struct`, `CoffeeBlend`, que contém três campos. O segundo campo, `beans`, é um `array` do tipo definido no exemplo anterior. Como os tipos encontram-se em _namespaces_ diferentes, desta vez é necessário usar um identificador qualificado.

<code lang="csl">
// file = coffee/blends/CoffeeBlend.csl
struct CoffeeBlend
{
	string name;
	coffee.beans.RoastedCoffeeBean[] beans;
	double[] percentages;
};
</code>

O exemplo anterior pode ser re-escrito de forma a "importar" o tipo `RoastedCoffeeBean`, ao invés de citar o seu nome completo. Este recurso é especialmente útil quando um tipo é citado diversas vezes no mesmo arquivo.

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

A especificação CSL abaixo define uma `interface`, chamada `ICoffeeMachine`, que contém dois atributos e dois métodos. O primeiro atributo é _readonly_, e o segundo é _read/write_. Ambos os métodos da interface declaram lançar exceções. O último método recebe um parâmetro `inout`, o que indica que o parâmetro é usado tanto para passar um argumento quanto para receber um valor de retorno.

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


Descrição da Gramática
----------------------

\subsection csl_grammar_identifiers Identificadores

O elemento mais básico da linguagem CSL são os identificadores. Como explicado acima, existem dois tipos de identificadores: o simples e o qualificado. Um identificador simples é uma _string_ ASCII, que começa obrigatoriamente com uma letra, e pode conter somente letras, números e o carácter *underscore ('_')*. Um identificador qualificado é uma lista de identificadores simples, conectados por pontos.

<code lang="ebnf">
qualified-identifier = Identifier , { "." , Identifier } ;
</code>

### Arquivos CSL

Um arquivo CSL é composto por uma lista opcional de cláusulas de importação de tipos, seguida por **uma única** especificação.

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

### Declaração de Tipos

A declaração de tipos é um elemento comum da linguagem CSL, que surge como parte da declaração de atributos e métodos. Um tipo pode ser declarado como sendo de um "tipo base" oferecido pelo Coral, ou então de um tipo especificado em outro arquivo CSL. Em qualquer caso, o tipo é considerado um `array` caso receba um par de colchetes como sufixo.

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

### Comentários e Documentação

Comentários estilo C/C++ podem aparecer imediatamente antes de uma especificação, declaração de atributo, método, etc. Os comentários _não_ são ignorados pelo compilador CSL, e _não_ podem aparecer em qualquer lugar. É proibido, por exemplo, incluir um comentário entre o início da declaração de um método e o ";" que encerra a declaração.

Sempre que possível o compilador aproveitará os comentários em CSL como documentação dos tipos. Em especial, o compilador tentará transferir a documentação para os _mappings_ gerados em C++, na forma de comentários Doxygen.

A princípio, considera-se que todo comentário serve para documentar o elemento imediatamente a seguir à sua posição. Porém, em alguns casos pode ser útil incluir o comentário logo após o elemento a ser documentado. Nestes casos, deve-se usar o sinal "<" como primeiro caráter do comentário, para indicar que ele se refere ao último elemento escrito, e não ao próximo.

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

### Como especificar uma Enumeração (enum)

A gramática para especificar um `enum` é fornecida abaixo. Um `enum` deve ter pelo menos um identificador. Não é possível atribuir valores aos identificadores &mdash; os valores são sempre de 0 a (n-1).

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


### Como especificar uma Exceção (exception)

A gramática para especificar um `exception` é fornecida abaixo. Um `exception` é identificado apenas pelo nome, e na versão atual do Coral não tem atributos nem herança.

<code lang="ebnf">
exception-specification = "exception" , Identifier , ";" ;
</code>

Exemplo de `exception`:

<code lang="csl">
exception IllegalNameException;
</code>


### Como especificar uma Estrutura (struct)

A gramática para especificar uma `struct` é fornecida abaixo. Uma `struct` pode conter um ou mais atributos (campos), de qualquer tipo.

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

A gramática para especificar uma `native class` é fornecida abaixo. Além do nome do tipo no Coral, é necessário fornecer &mdash; entre parênteses &mdash; o nome do tipo nativo em C++ (`CppType`) e o header onde ele está definido (`CppHeader`). Aceita-se como `CppType` qualquer identificador C++ com separação de escopo (e.g. `nmspc::TypeName`), e como `CppHeader` qualquer _string_ entre os sinais de "<" e ">".

<code lang="ebnf">
native-class-specification = "native" , "class" , Identifier ,
                                 "(" , CppHeader , CppType , ")" ,
                                 "{" , { native-class-member } , "}" , ";" ;

native-class-member        = attribute-declaration | method-declaration ;
</code>

Um `native class` pode ter dois tipos de membros: atributos e métodos. O nome de um atributo deve começar obrigatoriamente com uma letra minúscula. Note também que o Coral não suporta _overload_ de métodos &mdash; o nome de um método deve ser único, de modo que não cause conflitos com outros métodos ou com os \e accessors gerados para os atributos (leia mais na página de [mapeamento de CSL para C++](mapping.html)).

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

A gramática para especificar uma `interface` é fornecida abaixo. As declarações de atributos e métodos seguem a mesma gramática de uma `native class`. Porém, diferente das classes nativas, as interfaces podem ter herança e um novo tipo de membro: blocos de código C++, que são copiados _verbatim_ para os _mappings_ da interface (mais informações na página de [mapeamento de CSL para C++](mapping.html)).

<code lang="ebnf">
interface-specification = "interface" , Identifier , [inheritance-declaration] ,
                              "{" , { interface-member } , "}" , ";" ;

inheritance-declaration = ":" , qualified-identifier , { "," , qualified-identifier } ;

interface-member        = cpp-block | attribute-declaration | method-declaration ;

cpp-block               = "<c++" , ? C++ code ? , "c++>"
</code>

Note que o uso de herança &mdash; e, especialmente, herança múltipla &mdash; está sujeito a vários tipos de erros. O compilador verifica se há heranças cíclicas ou desnecessariamente redundantes. E ainda, se há conflitos entre nomes de métodos e atributos de interfaces herdadas. Para evitar conflitos, uma classe A só pode herdar das classes B e C caso o conjunto dos nomes dos membros (de A, B e C) seja totalmente disjunto.

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

Finalmente, a gramática para especificar um componente é fornecida abaixo.

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
