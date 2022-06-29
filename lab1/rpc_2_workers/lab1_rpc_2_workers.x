struct params {
  float * v;
  int n;
};

struct result {
  float min;
  float max;
};

program PROG {
  version VERSAO {
    result MIN_MAX(params) = 1;
  } = 100;
} = 44444444;
