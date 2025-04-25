# Vaccine-management-system
A first year Portuguese university project, for the course of introduction to algorithms and data structures utilising the C language

1. Introdução
Pretende-se a construção de um sistema de gestão de vacinas e suas inoculações. Para tal, o seu sistema deverá permitir a introdução de doses de vacinas e a gestão das suas aplicações.

A interacção com o programa deverá ocorrer através de um conjunto de linhas compostas por uma letra (comando) e um número de argumentos dependente do comando a executar. Pode assumir que todo o input fornecido respeitará os tipos indicados, por exemplo onde é esperado um valor inteiro decimal nunca será introduzida uma letra. Os possíveis comandos são listados na tabela seguinte e indicam as operações a executar.

Comando	Acção
q	termina o programa
c	introduz no sistema um novo lote de uma vacina
l	lista as vacinas disponíveis
a	aplica uma dose da vacina a um utente
r	retira a disponibilidade de uma vacina
d	apaga o registo de aplicações da vacina
u	lista as aplicações a um utente
t	avança o tempo simulado

2. Especificação do problema
O objectivo do projecto é ter um sistema de gestão de inoculações de vacinas a utentes. Para tal são introduzidos no sistema um conjunto de lotes de vacinas e respetivas doses. O sistema gere o registo da aplicação das doses das vacinas aos utentes.

Cada lote de vacina é caracterizado por um nome, um lote, uma data de validade e o número de doses disponíveis. O nome não pode conter carateres brancos (espaços, tabuladores ou newline), com um comprimento máximo de 50 bytes. Notar que um carater acentuado em utf-8 utiliza mais de um byte. Por exemplo tétano tem 6 letras mas ocupa 7 bytes (char em C). A designação do lote é constituída no máximo por 20 digitos hexadecimais, onde apenas são aceites dígitos decimais e letras maiúsculas de A a F. Um lote é único, mesmo para vacinas distintas. A data de validade, constituída por dia-mês-ano, inibe a aplicação de doses da vacina após o termo da validade. As data são sempre impressas com dois dígitos para o dia e mês mas a leitura é efetuada tanto com dois dígitos como com um só dígito. O número de doses que constituem o lote é um número inteiro positivo.

Uma inoculação é caracterizada por um nome do utente que a recebe, pelo lote da vacina de onde foi retirada a dose aplicada e pela data da aplicação. O nome que descreve o utente pode conter carateres brancos (espaços ou tabulador horizontal \t). Neste caso, o nome do utente é representado entre aspas. Caso não contenha carateres brancos, o nome do utente pode ser delimitado por aspas ou não. O nome do utente nunca contém o carater aspa na sua descrição. O nome do utente não tem comprimento máximo mas na maioria dos casos não excede 200 bytes.

Podem existir no máximo 1000 lotes de vacinas registados no sistema. Não existem limites para o número de inoculações nem no nome do utente, pelo que deve procurar utilizar a memória estritamente necessária. Não podem existir variáveis globais. Para facilitar a introdução dos dados, pode assumir que cada instrução não excede 65535 carateres. Se a memória se esgotar, o programa deve terminar de forma controlada, imprimindo a mensagem No memory. Antes de terminar, o programa deve libertar toda a memória reservada de forma dinâmica.

3. Dados de Entrada
Durante a execução do programa as instruções devem ser lidas do terminal (standard input) na forma de um conjunto de linhas iniciadas por um carácter, que se passa a designar por comando, seguido de um número de informações dependente do comando a executar; o comando e cada uma das informações são separados por pelo menos um carácter branco.

Os comandos disponíveis são descritos de seguida. Os caracteres < e > são utilizados apenas na descrição dos comandos para indicar os parâmetros. Os parâmetros opcionais estão indicados entre caracteres [ e ]. As repetições estão indicadas entre caracteres { e }. Cada comando tem sempre todos os parâmetros necessários à sua correta execução. Os caracteres ... indicam possíveis repetições de um parâmetro.

Cada comando indica uma determinada ação que se passa a caracterizar em termos de formato de entrada, formato de saída e erros a retornar.

Se o comando puder gerar mais de um erro, deverá ser indicado apenas o primeiro.

q - termina o programa:
Formato de entrada: q
Formato de saída: NADA
c - introduz no sistema um novo lote de uma vacina:
Formato de entrada: c <lote> <dia>-<mes>-<ano> <número-de-doses> <nome-da-vacina>
Formato de saída: <lote>. Insere as doses do lote da vacina indicada.
Erros:
too many vaccines no caso de exceder a capacidade de registo de vacinas do sistema.
duplicate batch number no caso de já existir um lote com a mesma designação.
invalid batch no caso de a designação do lote exceder o limite de digitos ou não ser constituídos por dígitos hexadecimais maiúsculos.
invalid name no caso de o nome exceder o limite de carateres ou conter o carateres inválidos.
invalid date no caso de a data não representar um dia válido ou este dia já ter sido ultrapassado pela data atual.
invalid quantity no caso de o número de doses não ser um número positivo.
l - lista os lotes das vacinas criadas:

Formato de entrada: l [<nome-da-vacina> { <nome-da-vacina> } ]
Formato de saída: Imprime informação de cada vacina com o formato <nome-da-vacina> <lote> <dia>-<mes>-<ano> <número-de-doses-disponíveis> <número-de-aplicações>. Imprime as vacinas por ordem cronológica da validade do lote e alfabética do lote para vacinas com a mesma validade. Imprime todas as vacinas caso seja invocado sem argumentos. Imprime as vacinas cujos nomes são indicados, separados por um espaço (), pela ordem que são indicados nos argumentos. Lotes apagados (ver adiante) não são listados.
Erros:
<nome-da-vacina>: no such vaccine no caso de não existir uma vacina com o nome indicado.
a - aplica uma dose da vacina a um utente:

Formato de entrada: a <nome-do-utente> <nome-da-vacina>
Formato de saída: Imprime a designação do lote aplicado ao utente, devendo ser escolhido o lote com pelo menos uma dose disponível com validade mais antiga mas ainda válida face à data atual.
Erros:
no stock no caso de não existir uma dose da vacina indicada válida e disponível.
already vaccinated no caso de o utente já ter sido vacinado nesse mesmo dia para a mesma vacina (<nome-da-vacina>).
r - retira a disponibilidade de uma vacina:

Formato de entrada: r <lote>
Formato de saída: <número-de-doses-já-aplicadas>. Apaga o lote da vacina indicado caso não tenham sido efetuadas inoculações do lote ou, caso tenha havido inoculações, o lote passa a indicar como sendo constituído pelo número de doses já inoculadas, não ficando doses disponíveis.
Erros:
<lote>: no such batch no caso de não existir um lote com a designação indicada.
d - apaga o registo de aplicações da vacina:

Formato de entrada: d <nome-do-utente> [ <data-de-aplicação> [ <lote> ]]
Formato de saída: <número-de-aplicações-apagadas>. Se o <lote> for omitido devem ser apagadas todas as aplicações de vacinas do utente para aquele dia. Se apenas for indicado o <nome-do-utente> devem ser apagadas todas as aplicações desse utente.
Erros:
<nome-do-utente>: no such user no caso de não existir nenhuma aplicação de vacina ao utente indicado.
invalid date no caso de a data não representar um dia válido ou este dia ser superior à data atual.
<lote>: no such batch no caso de não existir um lote com a designação indicada.
u - lista as todas as aplicações ou as aplicações de um utente:

Formato de entrada: u [<nome-do-utente>]
Formato de saída: <nome-do-utente> <lote> <dia>-<mes>-<ano> por cada inoculação, por ordem cronológica de aplicação.
Erros:
<nome-do-utente>: no such user no caso de não existir nenhuma aplicação de vacina ao utente indicado.
t - avança o tempo simulado:

Formato de entrada: t [<dia>-<mes>-<ano>]
Formato de saída: <dia>-<mes>-<ano> da nova data ou data data atual se for omitido o argumento. A data inicial do sistema é 01-01-2025.
Erros:
invalid date no caso de a data não representar um dia válido ou este dia ser anterior à data atual.
Deve ser possível invocar o programa com o argumento pt, ou seja ./proj pt, devendo as mensagens de erro ser expressas em português, nomeadamente: demasiadas vacinas, número de lote duplicado, lote inválido, nome inválido, data inválida, quantidade inválida, vacina inexistente, esgotado, já vacinado, lote inexistente, utente inexistente, sem memória.

Só pode usar as funções de biblioteca definidas em stdio.h, stdlib.h, ctype.h e string.h

Nota importante: não é permitida a utilização da instrução goto, da declaração extern, nem da função qsort nativa do C e nenhum destes nomes deve aparecer no vosso código.

Exemplos de utilização dos comandos
Comando c
O comando c permite criar o registo de um lote de um determinado tipo de vacina, com a validade indicada.

c A0C0 31-7-2025 210 malária
Comando l
O comando l sem argumentos permite listar todos os lotes de vacinas existentes no sistema.

l
O comando l com argumentos permite listar apenas os lotes das vacinas cujos nomes são indicados.

l tosse_convulsa
l tétano tosse_convulsa malária
Comando a
O comando a permite aplicar uma vacina a um utente.

a "João Miguel" tétano
Comando r
O comando r permite remover um determinado lote, não podendo haver mais aplicações desse lote.

r A0C0
Comando d
O comando d com um argumento permite apagar todos os registos de aplicações de vacina a um dado utente.

d xico
O comando d com dois argumentos permite apagar todos os registos de aplicações de vacina de um dado utente na data indicada.

d xico 1-1-2025
O comando d com três argumentos permite apagar todos os registos de aplicações de vacina a um dado utente na data indicada para o lote indicado.

d xico 1-1-2025 FA54
Comando u
O comando u sem argumentos permite listar todas as aplicações de vacinas existentes no sistema.

u
O comando u com um argumento permite listar todas as aplicações de vacinas existentes no sistema do utente indicado.

u xico
u "João Miguel"
Comando t
O comando t sem argumentos permite imprimir a data atual do sistema.

t
O comando t com um argumento permite alterar a data do sistema para uma data posterior.

t 2-2-2025
4. Compilação e teste
O compilador a utilizar é o gcc com as seguintes opções de compilação: -O3 -Wall -Wextra -Werror -Wno-unused-result. Para compilar o programa deve executar o seguinte comando:

  $ gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj *.c
O programa deverá escrever no standard output as respostas aos comandos apresentados no standard input. As respostas são igualmente linhas de texto formatadas conforme definido anteriormente neste enunciado. Tenha em atenção ao número de espaços entre elementos do seu output, assim como a ausência de espaços no final de cada linha. Procure respeitar escrupulosamente as indicações dadas.

Ver os exemplos de input e respectivos output na pasta public-tests/.

O programa deve ser executado da forma seguinte:

  $ ./proj < test.in > test.myout
Posteriormente poderá comparar o seu output (*.myout) com o output previsto (*.out) usando o comando diff,

  $ diff test.out test.myout
Para testar o seu programa poderá executar os passos indicados acima ou usar o comando make na pasta public-tests/.

Para utilizar o valgrind ou um debugger (gdb, ddd, ...) deverá substituir a opção de compilação -O3 por -g
