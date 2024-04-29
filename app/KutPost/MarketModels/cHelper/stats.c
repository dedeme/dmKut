// Copyright 30-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define mx_rows 24 // stat dates.
#define mx_cols 400 // simulationSteps x simulationSteps


int read_row (int **row, int *row_len, char *d, int id) {
  int *r = malloc(400 * sizeof(int));
  int ir = 0;

  char bf[100];
  int ibf = 0;
  for (;;) {
    char ch = d[id++];
    if ((ch >= '0' && ch <= '9') || (ibf == 0 && ch == '-')) {
      bf[ibf++] = ch;
    } else if (ch == ',' || ch == ']') {
      if (ibf == 0) {
        fprintf(stderr, "Value empty in '%s'\n", d);
        return -1;
      }

      bf[ibf] = 0;
      r[ir++] = atoi(bf);
      ibf = 0;

      if (ch == ']') {
        break;
      }
    } else {
      fprintf(stderr, "Unexpected character '%c' in '%s'\n", ch, d);
      return -1;
    }
  }

  *row = r;
  *row_len = ir;
  return id;
}

// Values < 0 returns -1 results.
int *calc (char *fn, int **d, int irow, int icol) {
  int *avgs = malloc(icol * sizeof(int));
  int iavgs = 0;
  for (int ic = 0; ic < icol; ++ic) {
    int sum = 0;
    for (int ir = 0; ir < irow; ++ir) {
      int v = d[ir][ic];
      sum = sum < 0 || v < 0 ? -1 : sum + v;
    }
    avgs[iavgs++] = sum < 0 ? -1 : sum/irow;
  };

  if (*fn == '0') return avgs;

  int *devs = malloc(icol * sizeof(int));
  int idevs = 0;
  for (int ic = 0; ic < icol; ++ic) {
    int sum = 0;
    for (int ir = 0; ir < irow; ++ir) {
      if (sum >= 0) {
        int v = d[ir][ic];
        int avg = avgs[ic];
        if (v >= 0 && avg >= 0) {
          sum += v > avg ? v - avg : avg - v;
        } else {
          sum = -1;
        }
      }
    }
    devs[idevs++] = sum;
  };

  if (*fn == '1') {
    for (int ic = 0; ic < icol; ++ic) {
      int avg = avgs[ic];
      int dev = devs[ic];
      avgs[ic] = (avg < 0 || dev < 0) ? -1 : avg - dev / irow;
    }
    free(devs);
    return avgs;
  }

  for (int ic = 0; ic < icol; ++ic) {
    int v = avgs[ic];
    int dev = devs[ic];
    devs[ic] = v < 0 || dev < 0
      ? -1
      : (int)((double)dev * 10000.0 / ((double)irow * (double)v))
    ;
  }
  free(avgs);
  return devs;
}

char *to_js (int *d, int len) {
  char *r = malloc(8000);
  char *pr = r;
  *pr++ = '[';
  for (int i = 0; i < len; ++i) {
    char n[20];
    int l = sprintf(n, "%d", d[i]);
    if (l == -1) return "Fail in itoa";
    memcpy(pr, n, l);
    pr = pr + l;
    *pr++ = ',';
  }
  *(pr - 1) = ']';
  *pr = 0;

  return r;
}

// MAIN --------------------------------

int main (int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr,
      "Use: stats function JSONvalues\n"
      "     Example: chelper 0 [[4,5],[2,7]]\n"
      "   - function must be:\n"
      "       0 -> average,\n"
      "       1 -> corrected average\n"
      "       2 -> deviation\n"
      "   - JSONvalues is a JSONized matrix of integers.\n"
    );
    return 0;
  }

  char *fn = argv[1];
  if (strcmp(fn, "0") && strcmp(fn, "1") && strcmp(fn, "2")) {
    fprintf(stderr,
      "Function must be:\n'0' (average),\n"
      "'1' (corrected average) or\n'2' (deviation)"
    );
    return 0;
  }

  int **rows = malloc(mx_rows * sizeof(int *));
  int irow = 0;
  int icol = 0;

  char *d = argv[2];
  int id = 0;
  if (d[id++] != '[') {
    fprintf(stderr, "Initial '[' is missing in %s\n", d);
    return 0;
  }

  for (;;) {
    char ch = d[id++];
    if (!ch) {
      fprintf(stderr, "Unexpected end of data in %s\n", d);
      return 0;
    }

    if (ch != '[') {
      fprintf(stderr, "Data '[' is missing in %s\n", d);
      return 0;
    }

    int *row = NULL;
    int row_len = 0;
    id = read_row(&row, &row_len, d, id); //
    if (id == -1) return 0;
    if (icol == 0) {
      icol = row_len;
    } else {
      if (row_len != icol) {
        fprintf(stderr, "There are rows with different length (%d and %d)\n",
          row_len, icol
        );
        return 0;
      }
    }
    rows[irow++] = row;

    ch = d[id++];
    if (ch != ',' && ch != ']') {
      fprintf(stderr, "Exptected ',' or ']'. Found %c in %s\n", ch, d);
      return 0;
    }

    if (ch == ']') break;
  }

  int *rs = calc(fn, rows, irow, icol);

  printf("%s", to_js(rs, icol));

  return 0;
}

