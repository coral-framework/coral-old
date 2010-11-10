---
title: Home
---

Notice
======

<span style="color:maroon">**The documentation will be rewritten in english soon.**</span>

This is a very outdated version of the documentation, originally written in portuguese.

Visão Geral
-----------

O **Coral** é um ambiente de desenvolvimento baseado em componentes construído sobre a linguagem de programação C++.
Ele compreende:

- Um modelo de componentes simples, com suporte a reflexão.
- Uma linguagem de especificação de componentes, interfaces e outros tipos.
- Um conjunto de ferramentas para a geração de código e criação de componentes.
- Um modelo de implantação e extensão baseado em módulos.

Além disso, o Coral inclui um conjunto abrangente de módulos padrões, visando principalmente o desenvolvimento de aplicações interativas. Os principais recursos providos pelos módulos padrões são:

- Serialização e recuperação de dados.
- _Bridge_ completo para a linguagem script Lua.
- Modelo de construção e extensão de GUIs baseado no Qt 4.5.

Se você procura uma introdução rápida ao Coral, leia o Tutorial de Início Rápido ou navegue na lista de Exemplos.

Modelo de Componentes
---------------------

Documentação do Modelo de Componentes:

1. Fundamentos
1. Tipos
1. Reflexão

Linguagem de Especificação
--------------------------

Documentação da _Coral Specification Language_.
Veja também: Mapeamento de CSL para C++.

Linguagens de Programação
-------------------------

É possível implementar componentes Coral em diferentes linguagens de programação:

- **C++** --- linguagem nativa do Coral, permite atingir o nível máximo de eficiência.
- **Lua** --- fornece maior agilidade na criação e composição de componentes.

Módulos Padrões
---------------

Módulos contendo extensões, serviços e componentes prontos para uso.

- TODO

Ferramentas
-----------

Ferramentas para auxiliar no processo de desenvolvimento de aplicações com o Coral.

- Compilador CSL &mdash; necessário para usar ou implementar componentes em C++.
- Pacote CMake &mdash; facilita a compilação de módulos e aplicações Coral utilizando o CMake.


Portabilidade
-------------

Por ser implementado em ISO C++, o Coral é portável para todas as principais plataformas.

Porém, visto que a implementação atual faz uso de recursos relativamente avançados do C++ (i.e. templates), pode ser obrigatório o uso de um compilador C++ recente, como o GCC 4.2 ou o Visual C++ 2005.

Além disso, por basear-se na ABI do C++ para implementar a interface binária dos componentes &mdash; e observado o fato que a ABI de um compilador C++ é geralmente incompatível com outros compiladores C++ &mdash; é provável que todos os módulos que compõem uma aplicação Coral precisem ser compilados com o mesmo compilador, sempre.
