"""Генерация A.txt и B.txt размера N×N."""
import sys
import os
import numpy as np

def save_matrix(path, M):
    N = M.shape[0]
    with open(path, "w") as f:
        f.write(f"{N}\n")
        np.savetxt(f, M, fmt="%.17g")

def main():
    N = int(sys.argv[1]) if len(sys.argv) > 1 else 200
    outdir = sys.argv[2] if len(sys.argv) > 2 else "data"
    os.makedirs(outdir, exist_ok=True)
    rng = np.random.default_rng(42)
    A = rng.random((N, N)) * 10.0
    B = rng.random((N, N)) * 10.0
    save_matrix(os.path.join(outdir, "A.txt"), A)
    save_matrix(os.path.join(outdir, "B.txt"), B)
    print(f"[gen] A.txt, B.txt  N={N}  dir={outdir}")

if __name__ == "__main__":
    main()