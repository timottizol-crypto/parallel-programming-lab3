set -e
OUT="${1:-results.csv}"
DATA="data"
SIZES=(200 400 800 1200 1600 2000)
PROCS=(1 2 4 8)

echo "N,P,time_sec,verified" > "$OUT"
mkdir -p "$DATA"

for N in "${SIZES[@]}"; do
    echo "─── Генерация N=$N ───"
    python3 generate_matrices.py "$N" "$DATA"

    for P in "${PROCS[@]}"; do
        if [ $((N % P)) -ne 0 ]; then
            echo "[skip] N=$N, P=$P (N % P != 0)"
            continue
        fi

        echo "[run] N=$N, P=$P"
        T=$(mpirun --oversubscribe -np "$P" ./main_mpi \
                "$DATA/A.txt" "$DATA/B.txt" 2>&1 \
                | tee /tmp/mpi_out.txt \
                | grep -oP 'time=\K[0-9.]+' || echo "")

        if [ -z "$T" ]; then
            echo "  ! не удалось извлечь time (вывод: /tmp/mpi_out.txt)"
            continue
        fi

        if python3 verify.py "$DATA/A.txt" "$DATA/B.txt" C_mpi.txt; then
            echo "$N,$P,$T,PASS" >> "$OUT"
        else
            echo "$N,$P,$T,FAIL" >> "$OUT"
        fi
    done
done

echo "════ Готово. Результаты: $OUT ════"