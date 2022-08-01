# Contador de Amigos

Para executar esse programa da forma adequada é necessário ter todos os serviços necessários do Hadoop executando. A versão utilizada foi a 3.2.4.

## Gerador de arquivo de entrada

É necessário ter o Ruby e a [gem Faker](https://github.com/faker-ruby/faker) instalada.

```
ruby generate_file.rb <OUTPUT FILE NAME> <APPROX FILE SIZE IN MEGABYTES>
```

## Execução

É necessários substituir os caminhos para o caminho absoluto de cada arquivo na sua máquina, e também definir o arquivo de entrada e diretório de saída desejado. Para a execução é necessário o Hadoop Streaming, que geralmente é baixado junto com os demais arquivos de instalação.

Para executar:

```
hadoop jar <CAMINHO PARA O ARQUIVO JAR DO HADOOP STREAMING>/hadoop-streaming-3.2.4.jar \
  -input /<REMOTE PATH>/input.txt \
  -output /<REMOTE PATH>/output \
  -mapper '<CAMINHO BINÁRIO DO PYTHON> <CAMINHO PARA>/mapper.py' \
  -reducer '<CAMINHO BINÁRIO DO PYTHON> <CAMINHO PARA>/reducer.py'
```
