---
title: Sistema de Tipos
---

Sistema de Tipos
================

Introdução
----------

O Coral foi projetado para ser simples. Por esta razão, o seu sistema de tipos é _minimalista_ &mdash; ou seja, contempla apenas os elementos absolutamente necessários para um bom projeto de aplicação.
As espécies de tipos oferecidas pelo Coral são:

- **Dados Primitivos** - booleano, inteiros, floats e string.
- **array** - vetor unidimensional de elementos.
- **enum** - enumeração de identificadores constantes, com valores de 0 a (n-1).
- **exception** - exceção, identificada apenas por nome, que pode ser lançada por métodos.
- **struct** - conjunto de campos de tipos diversos, identificados por nomes.
- **native class** - importa uma classe nativa do C++ como uma espécie de "interface leve" no Coral.
- **interface** - conjunto de métodos e atributos para acessar um serviço de um componente.
- **any** - funciona como um `union` (ou _variant_), capaz de guardar qualquer tipo do Coral.

Categorias de Tipos
-------------------

### 1. Valores
Os valores são subdivididos em duas sub-categorias: valores básicos e valores complexos.

#### 1.1 Valores Básicos
São os tipos mais simples, que requerem pouco ou nenhum suporte a reflexão.

- **Dados Primitivos** - xxx.
- **array** - xxx.
- **enum** - xxx.

#### 1.2 Valores Complexos
São tipos compostos, mais complexos, com suporte completo a reflexão.

- **struct** - xxx.
- **native class** - xxx.

### 2. Referências
- **interface** - xxx.

### 3. Especiais

- **exception** - Só pode ser usado para lançar exceções; i.e. não pode ser o tipo de atributos e métodos.
- **component** - xxx.

Arrays
------

- Vetor unidimensional de tamanho variável.

Enumerações (enum)
------------------

- Exceção, identificada apenas por nome, que pode ser lançada por métodos.

Exceções (exception)
--------------------

- Somente exceções podem ser lançadas (raises) por métodos.
- Não podem ser passadas como parâmetro e nem fazer parte de outros tipos.
- Toda exceção herda implicitamente de co::Exception.

Estruturas (struct)
-------------------

- Podem conter campos de qualquer tipo, salvo exceções.
- Não podem ter construtor, destrutor, métodos, atributos, e nem herança.
- Tipos de interface são traduzidos como smart-pointers em C++.

Classes Nativas (native class)
------------------------------

- Fornece interoperabilidade para classes implementadas fora do sistema de componentes, em C++.
- Não tem herança. Só tem um construtor default.
- É considerado um tipo de dado (copiado por valor), e não uma referência (i.e. não é ref-counted).
- Não gera classe de interface em C++, funciona apenas como um "proxy" (headerfile/typename) e associa operadores (e.g. `co::kindOf` e `co::typeOf`) ao tipo.
- Deve ser implementado através de um stub, estilo um componente.
- O sistema de componentes fornece um meta-objeto para reflexão.

Interfaces (interface)
----------------------

<div class="notice">
TODO
</div>
