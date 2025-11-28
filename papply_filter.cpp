// papply_filter.cpp
// Parallel image filter using pthreads
// Usage: ./papply_filter input_image.txt mask.txt output_image.txt num_threads
#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

struct ThreadArg {
    int id;
    int start_row;
    int end_row; // exclusive
    int R,C;
    vector<vector<int>> *A;
    vector<vector<int>> *B;
    int M[3][3];
    int divisor;
};

static inline int clamp_int(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }

void* worker(void* argp) {
    ThreadArg *arg = (ThreadArg*)argp;
    auto &A = *arg->A;
    auto &B = *arg->B;
    int R = arg->R, C = arg->C;
    for (int i = max(1, arg->start_row); i < min(R-1, arg->end_row); ++i) {
        for (int j = 1; j < C-1; ++j) {
            int s = 0;
            for (int mi=-1; mi<=1; ++mi) for (int mj=-1; mj<=1; ++mj)
                s += A[i+mi][j+mj] * arg->M[mi+1][mj+1];
            int v = s / arg->divisor;
            B[i][j] = clamp_int(v);
        }
    }
    return nullptr;
}

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
    int sum=0;
    for (int i=0;i<3;i++) for (int j=0;j<3;j++) { if (!(f >> M[i][j])) return false; sum += M[i][j]; }
    if (f >> divisor) { if (divisor==0) divisor = 1; }
    else { divisor = sum; if (divisor==0) divisor = 1; }
    return true;
}

int main(int argc, char** argv){
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " input_image.txt mask.txt output_image.txt num_threads\n";
        return 1;
    }
    string in = argv[1], maskf = argv[2], out = argv[3];
    int num_threads = stoi(argv[4]);
    if (num_threads < 1) num_threads = 1;

    vector<vector<int>> A, B;
    int R,C;
    if (!read_image(in, A, R, C)) { cerr << "Error reading image\n"; return 1; }
    int M[3][3], divisor;
    if (!read_mask(maskf, M, divisor)) { cerr << "Error reading mask\n"; return 1; }

    B = A; // copy to preserve borders

    vector<pthread_t> threads(num_threads);
    vector<ThreadArg> args(num_threads);

    // divide rows among threads (exclude row 0 and R-1 if possible)
    int work_rows = R; // we'll let thread function clamp to [1,R-1)
    int base = R / num_threads;
    int rem = R % num_threads;
    int cur = 0;
    for (int t=0;t<num_threads;++t){
        int rows_for_t = base + (t < rem ? 1 : 0);
        int start = cur;
        int end = cur + rows_for_t;
        cur = end;

        args[t].id = t;
        args[t].start_row = start;
        args[t].end_row = end;
        args[t].R = R; args[t].C = C;
        args[t].A = &A; args[t].B = &B;
        args[t].divisor = divisor;
        for (int i=0;i<3;i++) for (int j=0;j<3;j++) args[t].M[i][j] = M[i][j];
    }

    auto t1 = chrono::high_resolution_clock::now();
    for (int t=0;t<num_threads;++t){
        pthread_create(&threads[t], nullptr, worker, &args[t]);
    }
    for (int t=0;t<num_threads;++t){
        pthread_join(threads[t], nullptr);
    }
    auto t2 = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(t2 - t1).count();
    cout << "Parallel filter time (seconds) with " << num_threads << " threads: " << elapsed << "\n";

    if (!write_image(out, B, R, C)) { cerr << "Error writing output\n"; return 1; }
    return 0;
}

