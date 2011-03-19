---
title: Fundamentos
---

Fundamentos
===========

Esta p�gina resume os principais conceitos do Coral, provendo uma vis�o geral de sua arquitetura.


Linguagem de Especifica��o
--------------------------

A fim de permitir que diferentes linguagens de programa��o interoperem em um sistema de componentes, � necess�rio estabelecer um protocolo comum para a representa��o de dados e a invoca��o de opera��es.

Uma maneira de garantir isto � utilizando uma IDL (_Interface Definition Language_), e mapeamentos da IDL para as linguagens de programa��o desejadas. Com o aux�lio de um compilador IDL, � poss�vel gerar c�digo autom�tico que faz a convers�o entre o protocolo espec�fico de uma linguagem de programa��o e o protocolo comum.

O Coral define a linguagem _[Coral Specification Language (CSL)](csl.html)_, que se assemelha a uma IDL simples, mas tamb�m serve para especificar componentes e "classes nativas" (um conceito espec�fico do Coral). Como o Coral baseia-se no C++, que n�o prov� reflex�o em tempo de execu��o, a infra-estrutura da linguagem CSL tamb�m � usada para adicionar suporte � reflex�o de tipos.


Tipos
-----

No Coral, tipos como enums, structs, interfaces e componentes s�o definidos em arquivos CSL, seguindo o padr�o de nomenclatura `NomeDoTipo.csl`. Cada arquivo CSL cont�m a defini��o de **um �nico** tipo, cujo nome local deve ser igual ao nome do arquivo desconsiderando-se a extens�o `.csl`.

Os arquivos CSL podem ser organizados hierarquicamente, em diret�rios. Por exemplo:

	(raiz)
	  |
	  |- drink/
	  |    |- IDrink.csl        - Interface IDrink
	  |    |- IJuice.csl        - Interface IJuice, que herda de IDrink
	  |    |- ILiquor.csl       - Interface ILiquor, que herda de IDrink
	  |    +- liquor/
	  |         |- IVodka.csl   - Interface IVodka, que herda de ILiquor
	  |         +- IWhisky.csl  - Interface IWhisky, que herda de ILiquor
	  |
	  |- tasty/
	  |    |- Absolut.csl       - Componente Absolut, que implementa IVodka
	  |    +- Chivas.csl        - Componente Chivas, que implementa IWhisky
	  |
	  +- bargain/

Neste exemplo, a pasta `(raiz)` define um **reposit�rio de tipos**. As subpastas `drink`, `liquor`, `tasty` e `bargain` definem _namespaces_, e finalmente os arquivos `.csl` definem tipos (no exemplo, interfaces e componentes).

O nome completo de um tipo inclui os namespaces aos quais ele pertence, desde a raiz do reposit�rio, separados por ponto. Por exemplo, o nome completo da interface `IVodka` seria `drink.liquor.IVodka`, enquanto que o do componente `Absolut` seria `tasty.Absolut`.

A localiza��o de tipos � feita atrav�s da vari�vel `CORAL_PATH`, cujo comportamento � semelhante ao _classpath_ do Java. Se a vari�vel `CORAL_PATH` indicar mais de uma pasta raiz, o Coral far� as buscas por tipos em m�ltiplos reposit�rios &mdash; dando prefer�ncia ao reposit�rio que aparecer primeiro na lista, em caso de conflitos.


Introspec��o de Tipos
---------------------

O Coral oferece introspec��o completa para os tipos que comp�em o sistema de componentes &mdash; o que inclui todos os tipos declarados em arquivos `.csl` que estejam dentro de um reposit�rio de tipos.

A introspec��o de um tipo � feita em C++ com o aux�lio de um objeto do tipo `IType`. Todo tipo possui um `IType`, e todo `IType` possui identidade de refer�ncia. Ou seja, objetos do mesmo tipo compartilham a mesma inst�ncia de `IType`, e � poss�vel comparar se dois objetos s�o exatamente do mesmo tipo verificando se eles t�m o mesmo ponteiro para `IType`.

Al�m do `IType`, cada tipo do Coral � identificado por um _nome completo_ (vide a se��o anterior). Em C++, � poss�vel obter o nome de qualquer tipo conhecido, em tempo de compila��o, atrav�s do operador `co::nameOf`, como ilustrado abaixo:

<code lang="cpp">
#include <drink/liquor/IVodka.h>

using drink::liquor::IVodka;

std::cout << co::nameOf<IVodka>() << std::endl;
</code>

O exemplo acima imprimiria:

	drink.liquor.IVodka

Existem duas abordagens b�sicas para se obter o `IType` de um tipo. A primeira � baseada numa consulta pelo nome completo do tipo, e pode ser usada em qualquer situa��o. Por exemplo:

<code lang="cpp">
// IType of interface 'drink.IJuice'
co::IType* t = co::getType( "drink.IJuice" );
</code>

A segunda abordagem � mais simples, mas s� � poss�vel se o tipo for conhecido em tempo de compila��o:

<code lang="cpp">
#include <drink/IJuice.h>

co::IInterfaceType* t = co::typeOf<drink::IJuice>();
</code>

Internamente o operador `co::typeOf<T>()` faz uma busca pelo nome do tipo:


Ger�ncia de Tipos
-----------------

A classe `ITypeManager` � respons�vel por manter, em tempo de execu��o, um registro de todos os tipos conhecidos pela aplica��o at� o momento. Normalmente, um tipo � adicionado ao registro quando � usado pela primeira vez. Na pr�tica, isto pode ocorrer em fun��o dos seguintes eventos:

- **Quando um m�dulo � carregado**, todos os tipos utilizados pelo m�dulo, direta ou indiretamente, s�o verificados e registrados.
- **Quando o m�todo `ITypeManager::get( fullName )` � chamado**, se o nome especificado for de um tipo desconhecido, o tipo ser� automaticamente carregado e registrado. O arquivo `.csl` do tipo dever� estar em algum reposit�rio listado no `CORAL_PATH`; caso contr�rio, o m�todo lan�ar� uma exce��o.
- **Quando um novo tipo � definido dinamicamente** atrav�s do m�todo `co::INamespace::defineType()`.

Com o aux�lio do `ITypeManager` e dos objetos `IType` � poss�vel listar os m�todos e atributos de qualquer tipo, navegar na hierarquia das interfaces conhecidas, e carregar todos os tipos presentes nos reposit�rios listados no `CORAL_PATH`.

Uma grande vantagem do modelo de reposit�rio e registro de tipos do Coral � que a introspec��o de tipos pode ser feita de forma muito eficiente, sem que nenhuma biblioteca din�mica seja carregada. A biblioteca onde um tipo est� implementado s� precisa ser carregada caso o tipo seja instanciado, ou caso seja requerido o uso de reflex�o para o tipo.


Reflex�o
--------

O suporte a programa��o reflexiva no Coral � o alicerce para in�meros servi�os do sistema de componentes.
O sistema de reflex�o, oferecido como uma extens�o do sistema de tipos, permite a **manipula��o** de tipos e inst�ncias.

Enquanto que a introspec��o de tipos � feita atrav�s de objetos do tipo `IType`, os servi�os de reflex�o s�o oferecidos por objetos do tipo `IReflector`. Alguns exemplos de opera��es oferecidas pelos `IReflector`'s s�o:

- Instancia��o de um tipo.
- Obten��o e atribui��o de valores para atributos.
- Chamadas din�micas de m�todos de interfaces.
- Cria��o de _proxies_ din�micos para interfaces.

Diferente do sistema de tipos, que precisa apenas de arquivos `.csl` para funcionar, o sistema de reflex�o requer que as _bibliotecas din�micas_ onde os tipos foram implementados estejam carregadas.
Por raz�es de desempenho, o carregamento de bibliotecas no Coral � feito de forma tardia (_lazy_). O principal evento que pode causar o carregamento de bibliotecas � justamente a obten��o de um `IReflector` para um tipo.


M�dulos
-------

Um m�dulo � um **pacote de extens�o** que adiciona novos tipos, recursos e funcionalidades �s aplica��es. Os m�dulos s�o as unidades de empacotamento e distribui��o do Coral. Fisicamente, eles s�o diret�rios; e podem conter bibliotecas din�micas, scripts Lua, arquivos CSL, imagens, etc.

Como descrito na se��o de Tipos, todo diret�rio acess�vel atrav�s do `CORAL_PATH` define um _namespace_. Por sua vez, todo namespace � _potencialmente_ um m�dulo. O que determina se um namespace � um m�dulo ou n�o � a exist�ncia de um `IModulePartLoader` (uma interface) capaz de "carregar" um `IModulePart` a partir do conte�do do seu diret�rio.

Um mesmo m�dulo pode ser simultaneamente carregado por m�ltiplos `ModuleLoader`'s. Caso nenhum `ModuleLoader` seja capaz de carregar um m�dulo a partir de um certo diret�rio, o diret�rio � considerado apenas um namespace, e n�o um m�dulo. O Coral vem com um `ModuleLoader` padr�o capaz de carregar m�dulos a partir de _biblioteca din�micas_. Neste caso, a biblioteca deve ter o mesmo nome do diret�rio do m�dulo.
