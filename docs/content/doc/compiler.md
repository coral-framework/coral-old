---
title: Compiler
---

Compilador CSL
==============

Utilização
----------

O compilador tem um único argumento obrigatório, `moduleName`, que contém o nome do módulo Coral a ser compilado/implementado.

	Coral Compiler v0.5.0 (MacOSX x86_64 g++-4.2)
	Usage: coralc [options] [-g, --generate MODULE] [TYPE] ...
	Description:
	  Generates mappings for the list of types passed as command-line arguments.
	  If the -g option is specified (alongside a module name), the compiler will
	  generate code for a module. In this case, the passed list of types will be
	  considered extra module dependencies.
	Available options:
	  -p, --path EXTRA,DIRS  Add a list of repositories to the Coral path.
	  -o, --outdir DIR       Output dir for generated files (default: ./generated).
	  -m, --mappingsdir DIR  Separate output dir for mappings (when not specified,
							 mappings are generated in the 'outdir').
	  -v, --version          Show version information.

Opções
------

As principais flags aceitas pelo compilador estão descritas na tabela abaixo. É possível obter uma lista completa das flags aceitas passando-se a flag `--help` na linha de comando.

<table cellspacing="0">
	<thead>
		<tr><th>Flag</th><th>Descrição</th></tr>
	</thead>
	<tbody>
		<tr>
			<td>--path</td>
			<td>Path do Coral (sobrescreve a variável de ambiente `CORAL_PATH`).</td>
		</tr>
		<tr>
			<td>--impldir</td>
			<td>Diretório onde são gerados os arquivos `.cpp` que precisam ser compilados com o módulo.</td>
		</tr>
		<tr>
			<td>--mappingsdir</td>
			<td>Diretório onde são gerados os _mappings_ para os tipos utilizados pelo módulo.</td>
		</tr>
	</tbody>
</table>

Exemplos
--------

<div class="notice">Os exemplos precisam ser re-escritos.</div>

Exemplo Completo:
	coralc -c ~/home/projects/coral -o ./output -m ./mappings Coral

Exemplo Simplificado:
	coralc -c ~/home/projects/coral Coral
