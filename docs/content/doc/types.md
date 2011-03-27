---
title: Sistema de Tipos
---

Sistema de Tipos
================

Introdu��o
----------

O Coral foi projetado para ser simples. Por esta raz�o, o seu sistema de tipos � _minimalista_ &mdash; ou seja, contempla apenas os elementos absolutamente necess�rios para um bom projeto de aplica��o.
As esp�cies de tipos oferecidas pelo Coral s�o:

- **Dados Primitivos** - booleano, inteiros, floats e string.
- **array** - vetor unidimensional de elementos.
- **enum** - enumera��o de identificadores constantes, com valores de 0 a (n-1).
- **exception** - exce��o, identificada apenas por nome, que pode ser lan�ada por m�todos.
- **struct** - conjunto de campos de tipos diversos, identificados por nomes.
- **native class** - importa uma classe nativa do C++ como uma esp�cie de "interface leve" no Coral.
- **interface** - conjunto de m�todos e atributos para acessar um servi�o de um componente.
- **any** - funciona como um `union` (ou _variant_), capaz de guardar qualquer tipo do Coral.

Categorias de Tipos
-------------------

### 1. Valores
Os valores s�o subdivididos em duas sub-categorias: valores b�sicos e valores complexos.

#### 1.1 Valores B�sicos
S�o os tipos mais simples, que requerem pouco ou nenhum suporte a reflex�o.

- **Dados Primitivos** - xxx.
- **array** - xxx.
- **enum** - xxx.

#### 1.2 Valores Complexos
S�o tipos compostos, mais complexos, com suporte completo a reflex�o.

- **struct** - xxx.
- **native class** - xxx.

### 2. Refer�ncias
- **interface** - xxx.

### 3. Especiais

- **exception** - S� pode ser usado para lan�ar exce��es; i.e. n�o pode ser o tipo de atributos e m�todos.
- **component** - xxx.

Arrays
------

- Vetor unidimensional de tamanho vari�vel.

Enumera��es (enum)
------------------

- Exce��o, identificada apenas por nome, que pode ser lan�ada por m�todos.

Exce��es (exception)
--------------------

- Somente exce��es podem ser lan�adas (raises) por m�todos.
- N�o podem ser passadas como par�metro e nem fazer parte de outros tipos.
- Toda exce��o herda implicitamente de co::Exception.

Estruturas (struct)
-------------------

- Podem conter campos de qualquer tipo, salvo exce��es.
- N�o podem ter construtor, destrutor, m�todos, atributos, e nem heran�a.
- Tipos de interface s�o traduzidos como smart-pointers em C++.

Classes Nativas (native class)
------------------------------

- Fornece interoperabilidade para classes implementadas fora do sistema de componentes, em C++.
- N�o tem heran�a. S� tem um construtor default.
- � considerado um tipo de dado (copiado por valor), e n�o uma refer�ncia (i.e. n�o � ref-counted).
- N�o gera classe de interface em C++, funciona apenas como um "proxy" (headerfile/typename) e associa operadores (e.g. `co::kindOf` e `co::typeOf`) ao tipo.
- Deve ser implementado atrav�s de um stub, estilo um componente.
- O sistema de componentes fornece um meta-objeto para reflex�o.

Interfaces (interface)
----------------------

<div class="notice">
TODO
</div>
