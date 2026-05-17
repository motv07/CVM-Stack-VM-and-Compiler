import os
import subprocess

EXECUTABLE = "./main.exe"

# Directories and how to treat them
# "valid"   -> must pass, output checked
# "invalid" -> must throw an error
TEST_DIRS = [
    ("tests/valid",       "valid"),
    ("tests/expressions", "valid"),
    ("tests/controlflow", "valid"),
    ("tests/invalid",     "invalid"),
    ("examples",          "valid"),
    ("scripts",           "mixed"),   # test1 is invalid, rest are valid
]

# input  : string fed to stdin (use "\n" to separate multiple inputs)
# expected: normalized expected output (no OUTPUT:/Enter value: prefixes)
TEST_CONFIG = {
    # ── tests/valid ──────────────────────────────────────────
    "test_var.cvm":          {"input": None,     "expected": "10"},
    "test_assign.cvm":       {"input": None,     "expected": "6"},
    "test_nested.cvm":       {"input": "7",      "expected": "10\n7"},

    # ── tests/expressions ────────────────────────────────────
    "grouping.cvm":          {"input": None,     "expected": "16"},
    "precedence.cvm":        {"input": None,     "expected": "11"},

    # ── tests/controlflow ────────────────────────────────────
    "if_test.cvm":           {"input": "8",      "expected": "8"},
    "while_tes.cvm":         {"input": "3",      "expected": "0"},
    "nested_if_while.cvm":   {"input": "3",      "expected": "3\n100\n1"},

    # ── examples ─────────────────────────────────────────────
    # arithmetic: a=10, b=2  →  10+2=12, 10-2=8, 10*2=20, 10/2=5
    "arthimetic.cvm":        {"input": "10\n2",  "expected": "12\n8\n20\n5"},
    # bool: x=5  →  x>0=1, x<0=0, x==0=0
    "bool.cvm":              {"input": "5",      "expected": "1\n0\n0"},
    # factorial: n=5  →  120
    "factorial.cvm":         {"input": "5",      "expected": "120"},
    # if_else: x=3 → 1  |  x=-3 → -1  |  x=0 → 0  (test with x=3)
    "if_else.cvm":           {"input": "3",      "expected": "1"},
    # int: x=42 → 42
    "int.cvm":               {"input": "42",     "expected": "42"},
    # logical: a=5, b=5 → equal → 1
    "logical.cvm":           {"input": "5\n5",   "expected": "1"},
    # while: x=3 → prints 3 2 1
    "while.cvm":             {"input": "3",      "expected": "3\n2\n1"},

    # ── scripts ──────────────────────────────────────────────
    # test1: "let x = 10 @"  → bad character, should error
    "test1.cvm":             {"input": None,     "expected": None,  "invalid": True},
    # test2: let x=10; print (x);  → 10
    "test2.cvm":             {"input": None,     "expected": "10"},
    # test3: x=3 countdown  → 3 2 1
    "test3.cvm":             {"input": None,     "expected": "3\n2\n1"},
    # test4: x=3 with if inside while  → 3 100 1
    "test4.cvm":             {"input": None,     "expected": "3\n100\n1"},
}

def normalize(output):
    """Strip VM prefixes like 'OUTPUT:' and 'Enter value:' for clean comparison."""
    lines = output.strip().splitlines()
    cleaned = []
    for line in lines:
        line = line.replace("Enter value:", "").replace("OUTPUT:", "").strip()
        if line:
            cleaned.append(line)
    return "\n".join(cleaned)

def run_test(filepath, filename, treat_as):
    config       = TEST_CONFIG.get(filename, {})
    stdin_data   = config.get("input")
    expected_out = config.get("expected")
    force_invalid = config.get("invalid", False)

    # Determine if this specific file should be treated as invalid
    is_invalid = (treat_as == "invalid") or force_invalid

    try:
        result = subprocess.run(
            [EXECUTABLE, filepath],
            input=stdin_data,
            capture_output=True,
            text=True,
            timeout=5
        )
    except subprocess.TimeoutExpired:
        return False, f"Timed out — check if input is missing in TEST_CONFIG"

    actual_raw  = result.stdout.strip()
    actual_norm = normalize(actual_raw)
    has_error   = result.returncode != 0 or "Error" in result.stdout or "Error" in result.stderr

    # ── Invalid test ──
    if is_invalid:
        if has_error:
            return True, "(Caught error as expected)"
        else:
            return False, f"Expected an error, but it passed! Output: {actual_norm}"

    # ── Valid test ──
    if has_error:
        err = (result.stdout + result.stderr).strip()
        return False, f"Error: {err}"

    if expected_out is not None and actual_norm != expected_out:
        return False, f"\n         Expected : {repr(expected_out)}\n         Got      : {repr(actual_norm)}"

    out_display = f" -> output: {actual_norm}" if actual_norm else ""
    return True, out_display

def run_all():
    if not os.path.exists(EXECUTABLE):
        print(f"Error: {EXECUTABLE} not found. Run 'mingw32-make' first.")
        return

    total, passed = 0, 0

    for test_dir, treat_as in TEST_DIRS:
        if not os.path.exists(test_dir):
            continue

        print(f"\n--- Running tests in {test_dir}/ ---")
        for filename in sorted(os.listdir(test_dir)):
            if not filename.endswith(".cvm"):
                continue

            total += 1
            filepath = os.path.join(test_dir, filename)
            ok, msg = run_test(filepath, filename, treat_as)

            if ok:
                passed += 1
                print(f"  [PASS] {filename}{msg}")
            else:
                print(f"  [FAIL] {filename}")
                print(f"         {msg}")

    print(f"\n=== Test Summary: {passed}/{total} Passed ===")

if __name__ == "__main__":
    run_all()