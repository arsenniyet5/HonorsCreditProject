#!/usr/bin/env python3
"""
One-time dataset generator for the EditDistance and LongestCommonSubsequence
experiments.

Usage:
    python3 generate_datasets.py [output_dir] [--only FILE ...]
      output_dir  default: the folder this script is in
      --only      write only the named files, e.g. --only ed_shorter.csv lcs_shorter.csv
                  (the other files are left untouched)

Writes eight CSV files with columns: id, parameter_value, s1, s2, expected

  1. ed_length.csv        EditDistance, length 100..6400, alphabet A-E, similarity 50%
  2. ed_alphabet.csv      EditDistance, alphabet 2/4/8/16/26, length 1600, independent strings
  3. ed_similarity.csv    EditDistance, similarity 0..100%, length 1600, alphabet A-E
  4. lcs_length.csv       LCS, length 100..3200, alphabet A-E, similarity 50%
  5. lcs_alphabet.csv     LCS, alphabet 2/4/8/16/26, length 1600, independent strings
  6. lcs_similarity.csv   LCS, similarity 0..100%, length 1600, alphabet A-E
  7. ed_shorter.csv       EditDistance, shorter string 100..3200, longer string 3200,
                          alphabet A-E, independent strings
  8. lcs_shorter.csv      LCS, same design as 7 (for comparison)

Each dataset varies exactly one parameter; everything else is fixed.
5 string pairs per parameter value. For LCS only the length is stored.

Generation rules
  * Characters are drawn uniformly at random from the first k letters A..Z.
  * Similarity p: S2 has the same length as S1. Exactly round(p * n) positions
    (chosen uniformly at random) copy S1's character; every other position is
    a uniform random character *different* from S1's at that position, so the
    fraction of matching positions is exactly p.
  * "Independent random strings" (alphabet datasets): S1 and S2 are drawn
    fully independently (no similarity control).
  * Shorter-string datasets: the shorter string has length parameter_value,
    the longer one has length 3200; both are independent uniform strings over
    A-E. Rows alternate which side is short: odd id -> s1 is the shorter
    string, even id -> s2 is the shorter string.
  * A fixed seed per dataset makes the output reproducible. Adding datasets
    does not change existing ones (each file has its own seed).

Expected outputs are computed with independent reference implementations
(below), NOT with the project's C++ classes.

Sources for the reference implementations:
  * Edit Distance - GeeksforGeeks, "Edit Distance",
    https://www.geeksforgeeks.org/dsa/edit-distance-dp-5/
    (Space-optimized bottom-up DP, "Using Space Optimized DP")
  * Longest Common Subsequence - GeeksforGeeks, "Longest Common Subsequence (LCS)",
    https://www.geeksforgeeks.org/dsa/longest-common-subsequence-dp-4/
    (Space-optimized bottom-up DP, "Using Bottom-Up DP (Space-Optimization)")
"""

import argparse
import csv
import os
import random
import string
import sys
import time

PAIRS_PER_VALUE = 5
BASE_SEED = 20261119

LENGTHS_ED = [100, 200, 400, 800, 1600, 3200, 6400]
LENGTHS_LCS = [n for n in LENGTHS_ED if n <= 3200]
ALPHABET_SIZES = [2, 4, 8, 16, 26]
SIMILARITIES = [0, 25, 50, 75, 100]  # percent

FIXED_LENGTH = 1600
FIXED_ALPHABET = 5        # A, B, C, D, E
FIXED_SIMILARITY = 50     # percent (length datasets only)

SHORTER_LENGTHS = [100, 200, 400, 800, 1600, 3200]
LONGER_LENGTH = 3200


# ---------------------------------------------------------------------------
# Reference implementations (adapted from GeeksforGeeks, see module docstring)
# ---------------------------------------------------------------------------

def edit_distance_ref(s1: str, s2: str) -> int:
    """Levenshtein distance (insert, delete, replace each cost 1).
    GfG space-optimized DP: keep only the previous and current rows."""
    m, n = len(s1), len(s2)
    prev = list(range(n + 1))          # dp[0][j] = j
    curr = [0] * (n + 1)
    for i in range(1, m + 1):
        curr[0] = i                    # dp[i][0] = i
        a = s1[i - 1]
        for j in range(1, n + 1):
            if a == s2[j - 1]:
                curr[j] = prev[j - 1]
            else:
                curr[j] = 1 + min(curr[j - 1],   # insert
                                  prev[j],       # delete
                                  prev[j - 1])   # replace
        prev, curr = curr, prev
    return prev[n]


def lcs_length_ref(s1: str, s2: str) -> int:
    """Length of the longest common subsequence.
    GfG space-optimized DP: keep only the previous and current rows."""
    m, n = len(s1), len(s2)
    prev = [0] * (n + 1)
    curr = [0] * (n + 1)
    for i in range(1, m + 1):
        a = s1[i - 1]
        for j in range(1, n + 1):
            if a == s2[j - 1]:
                curr[j] = 1 + prev[j - 1]
            else:
                curr[j] = max(prev[j], curr[j - 1])
        prev, curr = curr, prev
    return prev[n]


def self_test() -> None:
    """Sanity-check the reference implementations against the worked examples
    on the GfG pages and a naive recursive definition on tiny random inputs."""
    assert edit_distance_ref("geek", "gesek") == 1
    assert edit_distance_ref("gfg", "gfg") == 0
    assert edit_distance_ref("abcd", "bcfe") == 3
    assert edit_distance_ref("", "abc") == 3
    assert lcs_length_ref("ABC", "ACD") == 2
    assert lcs_length_ref("AGGTAB", "GXTXAYB") == 4
    assert lcs_length_ref("ABC", "CBA") == 1
    assert lcs_length_ref("", "ABC") == 0

    from functools import lru_cache

    def naive_ed(a, b):
        @lru_cache(maxsize=None)
        def f(i, j):
            if i == 0:
                return j
            if j == 0:
                return i
            if a[i - 1] == b[j - 1]:
                return f(i - 1, j - 1)
            return 1 + min(f(i, j - 1), f(i - 1, j), f(i - 1, j - 1))
        return f(len(a), len(b))

    def naive_lcs(a, b):
        @lru_cache(maxsize=None)
        def f(i, j):
            if i == 0 or j == 0:
                return 0
            if a[i - 1] == b[j - 1]:
                return 1 + f(i - 1, j - 1)
            return max(f(i - 1, j), f(i, j - 1))
        return f(len(a), len(b))

    rng = random.Random(0)
    for _ in range(300):
        a = "".join(rng.choice("ABC") for _ in range(rng.randint(0, 8)))
        b = "".join(rng.choice("ABC") for _ in range(rng.randint(0, 8)))
        assert edit_distance_ref(a, b) == naive_ed(a, b), (a, b)
        assert lcs_length_ref(a, b) == naive_lcs(a, b), (a, b)


# ---------------------------------------------------------------------------
# String generation
# ---------------------------------------------------------------------------

def make_pair(rng: random.Random, length: int, alphabet_size: int,
              similarity_pct: int | None) -> tuple[str, str]:
    """similarity_pct=None -> two fully independent random strings.
    Otherwise exactly round(p * n) positions match S1 and every other
    position is a random character different from S1's at that position."""
    alphabet = string.ascii_uppercase[:alphabet_size]
    s1 = [rng.choice(alphabet) for _ in range(length)]
    if similarity_pct is None:
        s2 = [rng.choice(alphabet) for _ in range(length)]
        return "".join(s1), "".join(s2)

    copies = round(similarity_pct / 100 * length)
    copied = set(rng.sample(range(length), copies))
    s2 = []
    for i in range(length):
        if i in copied:
            s2.append(s1[i])
        else:
            s2.append(rng.choice([c for c in alphabet if c != s1[i]]))
    return "".join(s1), "".join(s2)


def make_shorter_pair(rng: random.Random, short_len: int, long_len: int,
                      alphabet_size: int, short_is_s1: bool) -> tuple[str, str]:
    """Two independent random strings of different lengths.
    short_is_s1 decides whether the shorter string is returned as s1 or s2."""
    alphabet = string.ascii_uppercase[:alphabet_size]
    short = "".join(rng.choice(alphabet) for _ in range(short_len))
    long = "".join(rng.choice(alphabet) for _ in range(long_len))
    return (short, long) if short_is_s1 else (long, short)


# ---------------------------------------------------------------------------
# Dataset definitions: (file name, solver, values, pair builder)
# The pair builder is called as build(rng, parameter_value, row_id) -> (s1, s2).
# ---------------------------------------------------------------------------

def fixed(length, alphabet_size, similarity):
    return lambda rng, v, row_id: make_pair(rng, length(v), alphabet_size(v),
                                            similarity(v))


def shorter(rng, v, row_id):
    return make_shorter_pair(rng, v, LONGER_LENGTH, FIXED_ALPHABET,
                             short_is_s1=(row_id % 2 == 1))


LENGTH = fixed(lambda v: v, lambda v: FIXED_ALPHABET, lambda v: FIXED_SIMILARITY)
ALPHABET = fixed(lambda v: FIXED_LENGTH, lambda v: v, lambda v: None)
SIMILARITY = fixed(lambda v: FIXED_LENGTH, lambda v: FIXED_ALPHABET, lambda v: v)

# Order matters: dataset k uses seed BASE_SEED + k. Append new datasets at the end.
DATASETS = [
    ("ed_length.csv", edit_distance_ref, LENGTHS_ED, LENGTH),
    ("ed_alphabet.csv", edit_distance_ref, ALPHABET_SIZES, ALPHABET),
    ("ed_similarity.csv", edit_distance_ref, SIMILARITIES, SIMILARITY),
    ("lcs_length.csv", lcs_length_ref, LENGTHS_LCS, LENGTH),
    ("lcs_alphabet.csv", lcs_length_ref, ALPHABET_SIZES, ALPHABET),
    ("lcs_similarity.csv", lcs_length_ref, SIMILARITIES, SIMILARITY),
    ("ed_shorter.csv", edit_distance_ref, SHORTER_LENGTHS, shorter),
    ("lcs_shorter.csv", lcs_length_ref, SHORTER_LENGTHS, shorter),
]


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("output_dir", nargs="?",
                        default=os.path.dirname(os.path.abspath(__file__)))
    parser.add_argument("--only", nargs="+", metavar="FILE")
    args = parser.parse_args()
    out_dir = args.output_dir
    os.makedirs(out_dir, exist_ok=True)

    known = {d[0] for d in DATASETS}
    unknown = set(args.only or []) - known
    if unknown:
        sys.exit(f"Unknown dataset(s): {', '.join(sorted(unknown))}")

    print("Self-testing reference implementations...", flush=True)
    self_test()

    for index, (filename, solver, values, build) in enumerate(DATASETS, 1):
        if args.only and filename not in args.only:
            continue
        rng = random.Random(BASE_SEED + index)
        path = os.path.join(out_dir, filename)
        start = time.time()
        with open(path, "w", newline="") as f:
            writer = csv.writer(f)
            writer.writerow(["id", "parameter_value", "s1", "s2", "expected"])
            row_id = 1
            for value in values:
                for _ in range(PAIRS_PER_VALUE):
                    s1, s2 = build(rng, value, row_id)
                    writer.writerow([row_id, value, s1, s2, solver(s1, s2)])
                    row_id += 1
                print(f"  {filename}: parameter_value={value} done", flush=True)
        print(f"Wrote {path} ({row_id - 1} rows, {time.time() - start:.1f}s)",
              flush=True)


if __name__ == "__main__":
    main()
