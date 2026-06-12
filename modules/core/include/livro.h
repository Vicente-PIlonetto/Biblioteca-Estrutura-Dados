#ifndef LIVRO_H
#define LIVRO_H

enum LivroAnoComparacao{
  ANO_IGUAL,
  ANO_MENOR,
  ANO_MENOR_IGUAL,
  ANO_MAIOR,
  ANO_MAIOR_IGUAL
};

typedef struct {
  char *titulo;
  char *autor;
  unsigned int codigo;
  short anoPublicacao;
  short quantidadeDisponivel;
} Livro;

/*
Cria o Livro e retorna um ponteiro para memória alocada do livro
*/
Livro* criarLivro(char* titulo, char* autor, short anoPublicacao, short quantidadeDisponivel, unsigned int codigo);

Livro* copiaLivro(Livro* livro);

/*
Limpa memória alocada para o Livro, título e autor.
*/
void liberarLivro(Livro* Livro);
/*
Compara o código do livro com o código fornecido
*/
char compararCodigoLivro(Livro* livro, int codigo);
/*
Compara o ano de publicação de um livro com o parâmetro e comparação indicados
*/
char compararAnoPublicacaoLivro(Livro* livro, short anoPublicacao, enum LivroAnoComparacao comparacao);
/*
Compara se o livro está disponivel
*/
char compararDisponivelLivro(Livro* livro);
/*
Compara se o titulo é igual ao especificado
*/
char compararTituloIgualLivro(Livro* livro, const char* texto);
/*
Compara se o texto especificado está dentro do livro
*/
char compararTituloDentroLivro(Livro* livro, const char* texto);
/*
Compara se o autor é igual ao especificado
*/
char compararAutorIgualLivro(Livro* livro, const char* texto);
/*
Compara se o autor especificado está dentro do livro
*/
char compararAutorDentroLivro(Livro* livro, const char* texto);

#endif