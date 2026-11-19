#!/usr/bin/env python3
"""
One-time dataset generator for the EditDistance and LongestCommonSubsequence
experiments.

Usage:
    python3 generate_datasets.py [output_dir]      (default: ./datasets)

Writes six CSV files with columns: id, parameter_value, s1, s2, expected

  1. ed_length.csv        EditDistance, length 100..6400, alphabet A-E, similarity 50%
  2. ed_alphabet.csv      EditDistance, alphabet 2/4/8/16/26, length 1600, independent strings
  3. ed_similarity.csv    EditDistance, similarity 0..100%, length 1600, alphabet A-E
  4. lcs_length.csv       LCS, length 100..3200, alphabet A-E, similarity 50%
  5. lcs_alphabet.csv     LCS, alphabet 2/4/8/16/26, length 1600, independent strings
  6. lcs_similarity.csv   LCS, similarity 0..100%, length 1600, alphabet A-E

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
  * A fixed seed per dataset makes the output reproducible.

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


# ---------------------------------------------------------------------------
# Dataset definitions: (file name, solver, values, value -> (len, alpha, sim))
# ---------------------------------------------------------------------------

DATASETS = [
    ("ed_length.csv", edit_distance_ref, LENGTHS_ED,
     lambda v: (v, FIXED_ALPHABET, FIXED_SIMILARITY)),
    ("ed_alphabet.csv", edit_distance_ref, ALPHABET_SIZES,
     lambda v: (FIXED_LENGTH, v, None)),
    ("ed_similarity.csv", edit_distance_ref, SIMILARITIES,
     lambda v: (FIXED_LENGTH, FIXED_ALPHABET, v)),
    ("lcs_length.csv", lcs_length_ref, LENGTHS_LCS,
     lambda v: (v, FIXED_ALPHABET, FIXED_SIMILARITY)),
    ("lcs_alphabet.csv", lcs_length_ref, ALPHABET_SIZES,
     lambda v: (FIXED_LENGTH, v, None)),
    ("lcs_similarity.csv", lcs_length_ref, SIMILARITIES,
     lambda v: (FIXED_LENGTH, FIXED_ALPHABET, v)),
]


def main() -> None:
    out_dir = sys.argv[1] if len(sys.argv) > 1 else "datasets"
    os.makedirs(out_dir, exist_ok=True)

    print("Self-testing reference implementations...", flush=True)
    self_test()

    for index, (filename, solver, values, params) in enumerate(DATASETS, 1):
        rng = random.Random(BASE_SEED + index)
        path = os.path.join(out_dir, filename)
        start = time.time()
        with open(path, "w", newline="") as f:
            writer = csv.writer(f)
            writer.writerow(["id", "parameter_value", "s1", "s2", "expected"])
            row_id = 1
            for value in values:
                length, alphabet_size, similarity = params(value)
                for _ in range(PAIRS_PER_VALUE):
                    s1, s2 = make_pair(rng, length, alphabet_size, similarity)
                    writer.writerow([row_id, value, s1, s2, solver(s1, s2)])
                    row_id += 1
                print(f"  {filename}: parameter_value={value} done", flush=True)
        print(f"Wrote {path} ({row_id - 1} rows, {time.time() - start:.1f}s)",
              flush=True)


if __name__ == "__main__":
    main()
