---
title: Fundamentos
---

Fundamentos
===========

Esta página resume os principais conceitos do Coral, provendo uma visão geral de sua arquitetura.


Linguagem de Especificação
--------------------------

A fim de permitir que diferentes linguagens de programação interoperem em um sistema de componentes, é necessário estabelecer um protocolo comum para a representação de dados e a invocação de operações.

Uma maneira de garantir isto é utilizando uma IDL (_Interface Definition Language_), e mapeamentos da IDL para as linguagens de programação desejadas. Com o auxílio de um compilador IDL, é possível gerar código automático que faz a conversão entre o protocolo específico de uma linguagem de programação e o protocolo comum.

O Coral define a linguagem _[Coral Specification Language (CSL)](csl.html)_, que se assemelha a uma IDL simples, mas também serve para especificar componentes e "classes nativas" (um conceito específico do Coral). Como o Coral baseia-se no C++, que não provê reflexão em tempo de execução, a infra-estrutura da linguagem CSL também é usada para adicionar suporte à reflexão de tipos.


Tipos
-----

No Coral, tipos como enums, structs, interfaces e componentes são definidos em arquivos CSL, seguindo o padrão de nomenclatura `NomeDoTipo.csl`. Cada arquivo CSL contém a definição de **um único** tipo, cujo nome local deve ser igual ao nome do arquivo desconsiderando-se a extensão `.csl`.

Os arquivos CSL podem ser organizados hierarquicamente, em diretórios. Por exemplo:

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

Neste exemplo, a pasta `(raiz)` define um **repositório de tipos**. As subpastas `drink`, `liquor`, `tasty` e `bargain` definem _namespaces_, e finalmente os arquivos `.csl` definem tipos (no exemplo, interfaces e componentes).

O nome completo de um tipo inclui os namespaces aos quais ele pertence, desde a raiz do repositório, separados por ponto. Por exemplo, o nome completo da interface `IVodka` seria `drink.liquor.IVodka`, enquanto que o do componente `Absolut` seria `tasty.Absolut`.

A localização de tipos é feita através da variável `CORAL_PATH`, cujo comportamento é semelhante ao _classpath_ do Java. Se a variável `CORAL_PATH` indicar mais de uma pasta raiz, o Coral fará as buscas por tipos em múltiplos repositórios &mdash; dando preferência ao repositório que aparecer primeiro na lista, em caso de conflitos.


Introspecção de Tipos
---------------------

O Coral oferece introspecção completa para os tipos que compõem o sistema de componentes &mdash; o que inclui todos os tipos declarados em arquivos `.csl` que estejam dentro de um repositório de tipos.

A introspecção de um tipo é feita em C++ com o auxílio de um objeto do tipo `IType`. Todo tipo possui um `IType`, e todo `IType` possui identidade de referência. Ou seja, objetos do mesmo tipo compartilham a mesma instância de `IType`, e é possível comparar se dois objetos são exatamente do mesmo tipo verificando se eles têm o mesmo ponteiro para `IType`.

Além do `IType`, cada tipo do Coral é identificado por um _nome completo_ (vide a seção anterior). Em C++, é possível obter o nome de qualquer tipo conhecido, em tempo de compilação, através do operador `co::nameOf`, como ilustrado abaixo:

<code lang="cpp">
#include <drink/liquor/IVodka.h>

using drink::liquor::IVodka;

std::cout << co::nameOf<IVodka>() << std::endl;
</code>

O exemplo acima imprimiria:

	drink.liquor.IVodka

Existem duas abordagens básicas para se obter o `IType` de um tipo. A primeira é baseada numa consulta pelo nome completo do tipo, e pode ser usada em qualquer situação. Por exemplo:

<code lang="cpp">
// IType of interface 'drink.IJuice'
co::IType* t = co::getType( "drink.IJuice" );
</code>

A segunda abordagem é mais simples, mas só é possível se o tipo for conhecido em tempo de compilação:

<code lang="cpp">
#include <drink/IJuice.h>

co::IInterfaceType* t = co::typeOf<drink::IJuice>();
</code>

Internamente o operador `co::typeOf<T>()` faz uma busca pelo nome do tipo:


Gerência de Tipos
-----------------

A classe `ITypeManager` é responsável por manter, em tempo de execução, um registro de todos os tipos conhecidos pela aplicação até o momento. Normalmente, um tipo é adicionado ao registro quando é usado pela primeira vez. Na prática, isto pode ocorrer em função dos seguintes eventos:

- **Quando um módulo é carregado**, todos os tipos utilizados pelo módulo, direta ou indiretamente, são verificados e registrados.
- **Quando o método `ITypeManager::get( fullName )` é chamado**, se o nome especificado for de um tipo desconhecido, o tipo será automaticamente carregado e registrado. O arquivo `.csl` do tipo deverá estar em algum repositório listado no `CORAL_PATH`; caso contrário, o método lançará uma exceção.
- **Quando um novo tipo é definido dinamicamente** através do método `co::INamespace::defineType()`.

Com o auxílio do `ITypeManager` e dos objetos `IType` é possível listar os métodos e atributos de qualquer tipo, navegar na hierarquia das interfaces conhecidas, e carregar todos os tipos presentes nos repositórios listados no `CORAL_PATH`.

Uma grande vantagem do modelo de repositório e registro de tipos do Coral é que a introspecção de tipos pode ser feita de forma muito eficiente, sem que nenhuma biblioteca dinâmica seja carregada. A biblioteca onde um tipo está implementado só precisa ser carregada caso o tipo seja instanciado, ou caso seja requerido o uso de reflexão para o tipo.


Reflexão
--------

O suporte a programação reflexiva no Coral é o alicerce para inúmeros serviços do sistema de componentes.
O sistema de reflexão, oferecido como uma extensão do sistema de tipos, permite a **manipulação** de tipos e instâncias.

Enquanto que a introspecção de tipos é feita através de objetos do tipo `IType`, os serviços de reflexão são oferecidos por objetos do tipo `IReflector`. Alguns exemplos de operações oferecidas pelos `IReflector`'s são:

- Instanciação de um tipo.
- Obtenção e atribuição de valores para atributos.
- Chamadas dinâmicas de métodos de interfaces.
- Criação de _proxies_ dinâmicos para interfaces.

Diferente do sistema de tipos, que precisa apenas de arquivos `.csl` para funcionar, o sistema de reflexão requer que as _bibliotecas dinâmicas_ onde os tipos foram implementados estejam carregadas.
Por razões de desempenho, o carregamento de bibliotecas no Coral é feito de forma tardia (_lazy_). O principal evento que pode causar o carregamento de bibliotecas é justamente a obtenção de um `IReflector` para um tipo.


Módulos
-------

Um módulo é um **pacote de extensão** que adiciona novos tipos, recursos e funcionalidades às aplicações. Os módulos são as unidades de empacotamento e distribuição do Coral. Fisicamente, eles são diretórios; e podem conter bibliotecas dinâmicas, scripts Lua, arquivos CSL, imagens, etc.

Como descrito na seção de Tipos, todo diretório acessível através do `CORAL_PATH` define um _namespace_. Por sua vez, todo namespace é _potencialmente_ um módulo. O que determina se um namespace é um módulo ou não é a existência de um `IModulePartLoader` (uma interface) capaz de "carregar" um `IModulePart` a partir do conteúdo do seu diretório.

Um mesmo módulo pode ser simultaneamente carregado por múltiplos `ModuleLoader`'s. Caso nenhum `ModuleLoader` seja capaz de carregar um módulo a partir de um certo diretório, o diretório é considerado apenas um namespace, e não um módulo. O Coral vem com um `ModuleLoader` padrão capaz de carregar módulos a partir de _biblioteca dinâmicas_. Neste caso, a biblioteca deve ter o mesmo nome do diretório do módulo.
