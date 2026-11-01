"""Сверка C_mpi.txt с A @ B через NumPy."""
import sys
import numpy as np

def load_matrix(path):
    with open(path, "r") as f:
        N = int(f.readline())
        data = np.loadtxt(f)
    if data.shape != (N, N):
        raise ValueError(f"{path}: shape {data.shape}, ожидалось ({N},{N})")
    return data

def main():
    if len(sys.argv) != 4:
        print("Usage: verify.py A.txt B.txt C_mpi.txt")
        sys.exit(2)

    A = load_matrix(sys.argv[1])
    B = load_matrix(sys.argv[2])
    C_mpi = load_matrix(sys.argv[3])

    C_ref = A @ B
    maxdiff = np.abs(C_mpi - C_ref).max()
    maxref = np.abs(C_ref).max()
    rel = maxdiff / maxref if maxref > 0 else maxdiff

    tol = 1e-9
    if rel < tol:
        print(f"PASS  N={A.shape[0]}  max_abs={maxdiff:.3e}  rel={rel:.3e}")
        sys.exit(0)
    else:
        print(f"FAIL  N={A.shape[0]}  max_abs={maxdiff:.3e}  rel={rel:.3e}")
        sys.exit(1)

if __name__ == "__main__":
    main()