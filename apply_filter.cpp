// apply_filter.cpp
// Serial image filter: usage:
//   ./apply_filter input_image.txt mask.txt output_image.txt
// mask format: 3 lines of 3 integers each. optional 4th line: divisor (int).
#include <bits/stdc++.h>
using namespace std;

static inline int clamp_int(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }

bool read_image(const string &fname, vector<vector<int>> &A, int &R, int &C) {
    ifstream f(fname);
    if (!f) return false;
    if (!(f >> R >> C)) return false;
    A.assign(R, vector<int>(C,0));
    for (int i=0;i<R;i++){
        for (int j=0;j<C;j++){
            int x;
            if (!(f >> x)) return false;
            A[i][j] = x;
        }
    }
    return true;
}

bool write_image(const string &fname, const vector<vector<int>> &A, int R, int C) {
    ofstream f(fname);
    if (!f) return false;
    f << R << "\t" << C << "\n";
    for (int i=0;i<R;i++){
        for (int j=0;j<C;j++){
            f << A[i][j];
            if (j+1 < C) f << "\t";
        }
        f << "\n";
    }
    return true;
}

bool read_mask(const string &fname, int M[3][3], int &divisor) {
    ifstream f(fname);
    if (!f) return false;
    int sum = 0;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            if (!(f >> M[i][j])) return false;
            sum += M[i][j];
        }
    }
    if (f >> divisor) {
        if (divisor == 0) divisor = 1;
    } else {
        divisor = sum;
        if (divisor == 0) divisor = 1;
    }
    return true;
}

int main(int argc, char** argv){
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " input_image.txt mask.txt output_image.txt\n";
        return 1;
    }
    string in = argv[1], maskf = argv[2], out = argv[3];
    vector<vector<int>> A;
    int R,C;
    if (!read_image(in, A, R, C)) { cerr << "Error reading image " << in << "\n"; return 1; }
    int M[3][3], divisor;
    if (!read_mask(maskf, M, divisor)) { cerr << "Error reading mask " << maskf << "\n"; return 1; }

    vector<vector<int>> B = A; // initialize output (keeps borders)
    auto t1 = chrono::high_resolution_clock::now();

    for (int i = 1; i < R-1; ++i) {
        for (int j = 1; j < C-1; ++j) {
            int s = 0;
            for (int mi = -1; mi <= 1; ++mi) {
                for (int mj = -1; mj <= 1; ++mj) {
                    s += A[i+mi][j+mj] * M[mi+1][mj+1];
                }
            }
            int v = s / divisor;
            B[i][j] = clamp_int(v);
        }
    }

    auto t2 = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(t2 - t1).count();
    cout << "Serial filter time (seconds): " << elapsed << "\n";

    if (!write_image(out, B, R, C)) { cerr << "Error writing output " << out << "\n"; return 1; }
    return 0;
}

