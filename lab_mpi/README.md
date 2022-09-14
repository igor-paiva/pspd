# Laboratório MPI

Alunos:

- Igor Batista Paiva - 18/0018728

## Sequencial

Para executar:

```
gcc sequencial.c -lm -o bin

./bin 100
```

## MPI um host

Será utilizado sempre a quantidade máxima de processos (comm size) - 1 como workers e dos processos será o gerente.

Para executar será necessário o mpicc:

```
mpicc mpi_um_host.c -lm -o bin

// desejar utilizar todas as threads disponíveis (não só cores)
// utilize a flag --use-hwthread-cpus

mpirun -n <NUMERO DE PROCESSOS> ./bin 100
```
