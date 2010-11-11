---
title: Mapeamento de CSL para C++
---

Mapeamento de CSL para C++
==========================

Tipos Básicos
-------------

Os tipos básicos de CSL são traduzidos para C++ seguindo a tabela abaixo:

<table cellspacing="0" style="width: 360px">
	<thead>
		<tr><th>CSL</th><th>C++</th></tr>
	</thead>
	<tbody>
		<tr><td>any</td><td>co::Any</td></tr>
		<tr><td>bool</td><td>bool</td></tr>
		<tr><td>int8</td><td>co::int8</td></tr>
		<tr><td>uint8</td><td>co::uint8</td></tr>
		<tr><td>int16</td><td>co::int16</td></tr>
		<tr><td>uint16</td><td>co::uint16</td></tr>
		<tr><td>int32</td><td>co::int32</td></tr>
		<tr><td>uint32</td><td>co::uint32</td></tr>
		<tr><td>int64</td><td>co::int64</td></tr>
		<tr><td>uint64</td><td>co::uint64</td></tr>
		<tr><td>float</td><td>float</td></tr>
		<tr><td>double</td><td>double</td></tr>
		<tr><td>string</td><td>std::string</td></tr>
	</tbody>
</table>


Arrays
------

Arrays, quando aparecem em um método ou no tipo de um atributo de uma interface, são traduzidos como um co::ArrayRange, uma espécie de **iterador** que abstrai a representação de array utilizada internamente pela implementação de um componente.

<table cellspacing="0" style="width: 360px">
	<thead>
		<tr><th>CSL</th><th>C++</th></tr>
	</thead>
	<tbody>
		<tr>
			<td><em>Type</em> <tt>[]</tt></td>
			<td><tt>co::ArrayRange&lt;</tt> <em>Type</em> <tt>&gt;</tt></td>
		</tr>
	</tbody>
</table>

Veja a documentação do _co::ArrayRange_ para saber como iterá-lo ou construí-lo um a partir de um std::vector ou qualquer seqüência de elementos em memória. Observe que um co::ArrayRange é apenas um iterador &mdash; não é possível adicionar ou remover elementos de um array através de um co::ArrayRange!


Namespaces
----------

O mapeamento de tipos para C++ respeita o local e o _namespace_ onde os tipos em CSL são definidos. Por exemplo:

<code lang="csl">
// CSL: foo/bar/EmptyStruct.csl
struct EmptyStruct
{
};
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// C++: foo/bar/EmptyStruct.h
namespace foo {
namespace bar {

struct EmptyStruct {};

} // namespace bar
} // namespace foo
</code>

**Nota:** o _namespace_ de um tipo em CSL é definido implicitamente pelo diretório onde o arquivo CSL se encontra (vide a página de [fundamentos](fundamentals.html)).


Enumerações (enum)
------------------

A tradução de um `enum` para C++ é feito trivialmente, mantendo a ordem dos elementos:

<code lang="csl">
// em CSL
enum Numbers
{
	One,
	Two,
	Three
};
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// em C++
enum Numbers
{
	One,
	Two,
	Three
};
</code>


Exceções (exception)
--------------------

Um `exception` com um determinado nome é mapeado para uma classe em C++, de mesmo nome, que herda de co::UserException.

<code lang="csl">
// especificação em CSL
exception MyException;
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// mapeamento para C++
class MyException : public co::UserException
{
public:
    MyException();
	MyException( const std::string& message );
};
</code>


Estruturas (struct)
-------------------

Um `struct` em CSL é mapeado para um simples `struct` em C++, mantendo a ordem dos campos. Por exemplo:

<code lang="csl">
// em CSL
struct Account
{
	uint32 id;
	string owner;
	double balance;
};
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// em C++
struct Account
{
	co::uint32 id;
	std::string owner;
	double balance;
};
</code>

O tipo dos campos são mapeados segundo as regras gerais, exceto quando o tipo é uma interface ou um array (veja abaixo).


### Campos do tipo Array

Todo campo do tipo array é mapeado como um `std::vector`, já que um `co::ArrayRange` não pode armazenar elementos. Exemplo:

<code lang="csl">
// em CSL
struct Foo
{
	int8[] intArrayField;
	string[] stringArrayField;
};
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// em C++
struct Foo
{
	std::vector<co::int8> intArrayField;
	std::vector<std::string> stringArrayField;
};
</code>


### Campos do tipo Interface

Todo campo do tipo interface é mapeado como um `co::RefPtr` (_smart pointer_) para que seja considerado na contagem de referências da interface. O mesmo se aplica caso o campo seja um array de interfaces &mdash; neste caso, o array é mapeado como um `co::RefVector`. Por exemplo:

<code lang="csl">
// em CSL
struct Foo
{
	co.Type aType;
	co.MemberInfo[] someMembers;
};
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// em C++
struct Foo
{
	co::RefPtr<co::Type> aType;
	co::RefVector<co::MemberInfo> someMembers;
};
</code>


Classes Nativas (native class)
------------------------------

No caminho inverso dos outros tipos, classes nativas são classes **originalmente implementadas em C++** que deseja-se &ldquo;importar&rdquo; para dentro do sistema de tipos do Coral. A idéia é que a programação seja feita usando diretamente a classe original em C++, o que torna a geração de mapeamentos desnecessária.

Embora o [compilador do Coral](compiler.html) gere um `.h` de mapeamento para as classes nativas, o _header_ gerado contém somente a declaração de operadores auxiliares como `co::kindOf`, `co::typeOf`, etc., além de um `#include` para o _header_ original do tipo.

Para informações sobre como implementar o adaptador de uma classe nativa, veja a página de classes nativas.


Interfaces (interface)
----------------------

Interfaces em CSL são mapeadas para classes puramente abstratas em C++. Todos os atributos e métodos da interface são mapeados para funções puramente virtuais. Por exemplo:

<code lang="csl">
// em CSL
interface Person
{
	attribute string name;

	void say( in string message );
};
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// em C++
class Person
{
public:
	virtual const std::string& getName() = 0;
	virtual void setName( const std::string& name ) = 0;

	virtual void say( const std::string& message ) = 0;
};
</code>

As regras para passagem e retorno de parâmetros nos métodos foram definidas de forma a priorizar simplicidade e eficiência. Em alguns casos, o programador acostumado com C++ pode achar que as modalidades de passagem de parâmetro disponíveis são poucas ou restritivas, mas as restrições existem para simplificar o sistema de tipos. Com algum cuidado, deve ser possível projetar qualquer API de forma eficiente, dentro destas regras.

<a name="in-parameters-table"></a>
**Regras Gerais para Passagem de Parâmetros e Retorno de Métodos:**

<table cellspacing="0">
	<thead>
		<tr><th>Categoria de Tipo</th><th>Regra de Passagem Aplicada</th></tr>
	</thead>
	<tbody>
		<tr>
			<td>Valores Primitivos (<tt>bool</tt>, <tt>[u]int{N}</tt>, <tt>float</tt>, <tt>double</tt> e <tt>enums</tt>)</td>
			<td>Passagem por valor (cópia)</td>
		</tr>
		<tr>
			<td>Valores Complexos (<tt>string</tt>, <tt>any</tt>, <tt>structs</tt> e <tt>native classes</tt>)</td>
			<td>Passagem por referência constante (<tt>const Type&</tt>)</td>
		</tr>
		<tr>
			<td>Referências (<tt>interfaces</tt>)</td>
			<td>Passagem por ponteiro (<tt>Type*</tt>)</td>
		</tr>
		<tr>
			<td>Arrays de Valores</td>
			<td>Passagem de um <tt>co::ArrayRange&lt;const Type&gt;</tt></td>
		</tr>
		<tr>
			<td>Arrays de Referências</td>
			<td>Passagem de um <tt>co::ArrayRange&lt;Type* const&gt;</tt></td>
		</tr>
	</tbody>
</table>

### Mapeamento de Atributos

Atributos são mapeados para métodos de acesso (_accessors_) utilizando convenções semelhantes às de JavaBeans. Um atributo normalmente é mapeado para dois métodos, um _getter_ e um _setter_ &mdash; a menos que seja `readonly`, quando é mapeado apenas para um _getter_.

Para um atributo `myAttrib` do tipo `T`, as convenções de nomenclatura aplicadas são:

- **Getter:**
		virtual T getMyAttrib() = 0;
- **Setter:**
		virtual void setMyAttrib( T myAttrib ) = 0;

Observe que o padrão no Coral é o nome dos atributos começar com letra minúscula e seguir o padrão _CamelCase_. Para formar o nome de um método, o nome do atributo é modificado para começar com maíscula e concatenado com um dos prefixos `get` ou `set`.

Exemplo de mapeamento de atributos _read-only_ e _read/write_ (o padrão):

<code lang="csl">
// em CSL
interface Account
{
	readonly attribute int32 id;
	attribute string email;
	attribute bool enabled;
};
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// em C++
class Account
{
public:
	virtual co::int32 getId() = 0;

	virtual const std::string& getEmail() = 0;
	virtual void setEmail( const std::string& email ) = 0;

	virtual bool isEnabled() = 0;
	virtual void setEnabled( bool enabled ) = 0;
};
</code>

### Mapeamento de Métodos

Cada método de uma interface é mapeado para um método puramente virtual da classe em C++. O retorno dos métodos (caso não seja `void`) e os parâmetros `in` são mapeados segundo a [tabela anterior](#in-parameters-table) (_Regras Gerais para Passagem de Parâmetros e Retorno de Métodos_). Porém, caso o parâmetro seja `out` ou `inout`, as regras utilizadas são as da tabela abaixo.

**Regras para Passagem de Parâmetros de Saída:**

<table cellspacing="0">
	<thead>
		<tr><th>Categoria de Tipo</th><th>Regra de Passagem Aplicada</th></tr>
	</thead>
	<tbody>
		<tr>
			<td>Valores Primitivos (<tt>bool</tt>, <tt>[u]int{N}</tt>, <tt>float</tt>, <tt>double</tt> e <tt>enums</tt>)</td>
			<td>Passagem por referência (<tt>Type&</tt>)</td>
		</tr>
		<tr>
			<td>Valores Complexos (<tt>string</tt>, <tt>any</tt>, <tt>structs</tt> e <tt>native classes</tt>)</td>
			<td>Passagem por referência (<tt>Type&</tt>)</td>
		</tr>
		<tr>
			<td>Referências (<tt>interfaces<tt>)</td>
			<td>Passagem por referência para ponteiro (<tt>Type*&</tt>)</td>
		</tr>
		<tr>
			<td>Arrays de Valores</td>
			<td>Passagem de um <tt>std::vector&lt;Type&gt;&</tt></td>
		</tr>
		<tr>
			<td>Arrays de Referências</td>
			<td>Passagem de um <tt>co::RefVector&lt;Type&gt;&</tt></td>
		</tr>
	</tbody>
</table>

No exemplo de mapeamento a seguir, considere que `RequestType` é um `enum`, `Request` é um `struct`, `ServiceDelegate` é uma `interface` e `Uuid` é uma classe nativa.

<code lang="csl">
// em CSL
interface Service
{
	readonly attribute co.Uuid[] forbiddenClientIds;

	void registerDelegate( in RequestType t, in ServiceDelegate d )
		raises IllegalArgumentException;

	void getAllDelegates( out ServiceDelegate[] delegates );

	void forbidRequestsFrom( in co.Uuid clientId );
	void allowRequestsFrom( in co.Uuid clientId );

	int32 filterForbiddenClients( inout co.Uuid[] clients );

	bool getDelegateFor( in Request r, out ServiceDelegate d );

	void handleRequest( inout Request r ) raises ForbiddenClientException;
};
</code>

<div class="mapping-down-arrow"></div>

<code lang="cpp">
// em C++
class Service
{
public:
	virtual co::ArrayRange<const co::Uuid> getForbiddenClientIds() = 0;

	virtual void registerDelegate( RequestType t, ServiceDelegate* d ) = 0;

	virtual void getAllDelegates( co::RefVector<ServiceDelegate>* delegates ) = 0;

	virtual void forbidRequestsFrom( const co::Uuid& clientId ) = 0;
	virtual void allowRequestsFrom( const co::Uuid& clientId ) = 0;

	virtual co::int32 filterForbiddenClients( std::vector<co::Uuid>* clients ) = 0;

	virtual bool getDelegateFor( const Request& r, ServiceDelegate*& d ) = 0;

	virtual void handleRequest( Request& r ) = 0;
};
</code>

Observe que a declaração de exceções (_raises_) em CSL não são mapeadas diretamente para C++, uma vez que o suporte a este recurso pelos compiladores C++ é precário.


Componentes (component)
-----------------------

Componentes só podem ser manipulados através da API de reflexão ou de suas interfaces servidoras (e.g. `co::Component`). Por isso, não geram nenhum tipo de mapeamento para C++. Para informações sobre como implementar um componente, veja a página de componentes.
