#!/usr/bin/env python3
"""
Descriptive statistics for every numeric field of every dataset CSV.

Usage:
    python3 dataset_statistics.py [dataset_dir]      (default: this script's folder)

Numeric fields are detected automatically (every value parses as a number);
for these datasets they are: id, parameter_value, expected.
The string fields s1 and s2 are not numeric and are skipped.

Statistics: count, min, max, mean, median, standard deviation.
Standard deviation is the SAMPLE standard deviation (n - 1 denominator).

Output: printed table + <dataset_dir>/descriptive_statistics.csv
"""

import csv
import glob
import os
import statistics
import sys


def is_number(text: str) -> bool:
    try:
        float(text)
        return True
    except ValueError:
        return False


def describe(values: list[float]) -> dict:
    return {
        "count": len(values),
        "min": min(values),
        "max": max(values),
        "mean": statistics.mean(values),
        "median": statistics.median(values),
        "std": statistics.stdev(values) if len(values) > 1 else 0.0,
    }


def main() -> None:
    data_dir = (sys.argv[1] if len(sys.argv) > 1
                else os.path.dirname(os.path.abspath(__file__)))
    out_path = os.path.join(data_dir, "descriptive_statistics.csv")
    header = ["dataset", "field", "count", "min", "max", "mean", "median", "std"]
    rows = []

    for path in sorted(glob.glob(os.path.join(data_dir, "*.csv"))):
        if os.path.abspath(path) == os.path.abspath(out_path):
            continue
        with open(path, newline="") as f:
            records = list(csv.DictReader(f))
        if not records:
            continue
        for field in records[0].keys():
            column = [r[field] for r in records]
            if not all(is_number(v) for v in column):
                continue
            s = describe([float(v) for v in column])
            rows.append([os.path.basename(path), field, s["count"],
                         f"{s['min']:g}", f"{s['max']:g}",
                         f"{s['mean']:.2f}", f"{s['median']:g}",
                         f"{s['std']:.2f}"])

    with open(out_path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(header)
        writer.writerows(rows)

    widths = [max(len(str(x)) for x in col) for col in zip(header, *rows)]
    for line in [header] + rows:
        print("  ".join(str(x).rjust(w) for x, w in zip(line, widths)))
    print(f"\nSaved {out_path}")


if __name__ == "__main__":
    main()
