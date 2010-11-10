---
title: Home
---

Notice
======

<span style="color:maroon">**The documentation will be rewritten in english soon.**</span>

This is a very outdated version of the documentation, originally written in portuguese.

Vis�o Geral
-----------

O **Coral** � um ambiente de desenvolvimento baseado em componentes constru�do sobre a linguagem de programa��o C++.
Ele compreende:

- Um modelo de componentes simples, com suporte a reflex�o.
- Uma linguagem de especifica��o de componentes, interfaces e outros tipos.
- Um conjunto de ferramentas para a gera��o de c�digo e cria��o de componentes.
- Um modelo de implanta��o e extens�o baseado em m�dulos.

Al�m disso, o Coral inclui um conjunto abrangente de m�dulos padr�es, visando principalmente o desenvolvimento de aplica��es interativas. Os principais recursos providos pelos m�dulos padr�es s�o:

- Serializa��o e recupera��o de dados.
- _Bridge_ completo para a linguagem script Lua.
- Modelo de constru��o e extens�o de GUIs baseado no Qt 4.5.

Se voc� procura uma introdu��o r�pida ao Coral, leia o Tutorial de In�cio R�pido ou navegue na lista de Exemplos.

Modelo de Componentes
---------------------

Documenta��o do Modelo de Componentes:

1. Fundamentos
1. Tipos
1. Reflex�o

Linguagem de Especifica��o
--------------------------

Documenta��o da _Coral Specification Language_.
Veja tamb�m: Mapeamento de CSL para C++.

Linguagens de Programa��o
-------------------------

� poss�vel implementar componentes Coral em diferentes linguagens de programa��o:

- **C++** --- linguagem nativa do Coral, permite atingir o n�vel m�ximo de efici�ncia.
- **Lua** --- fornece maior agilidade na cria��o e composi��o de componentes.

M�dulos Padr�es
---------------

M�dulos contendo extens�es, servi�os e componentes prontos para uso.

- TODO

Ferramentas
-----------

Ferramentas para auxiliar no processo de desenvolvimento de aplica��es com o Coral.

- Compilador CSL &mdash; necess�rio para usar ou implementar componentes em C++.
- Pacote CMake &mdash; facilita a compila��o de m�dulos e aplica��es Coral utilizando o CMake.


Portabilidade
-------------

Por ser implementado em ISO C++, o Coral � port�vel para todas as principais plataformas.

Por�m, visto que a implementa��o atual faz uso de recursos relativamente avan�ados do C++ (i.e. templates), pode ser obrigat�rio o uso de um compilador C++ recente, como o GCC 4.2 ou o Visual C++ 2005.

Al�m disso, por basear-se na ABI do C++ para implementar a interface bin�ria dos componentes &mdash; e observado o fato que a ABI de um compilador C++ � geralmente incompat�vel com outros compiladores C++ &mdash; � prov�vel que todos os m�dulos que comp�em uma aplica��o Coral precisem ser compilados com o mesmo compilador, sempre.
