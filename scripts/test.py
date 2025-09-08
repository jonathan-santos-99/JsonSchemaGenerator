#!/usr/bin/env python3

from sys import argv, exit, stderr
from os import listdir, path
import subprocess
import time

def run_test(program_path: str, test_path: str) -> tuple[bool, str, str]:
    result = subprocess.run(
        [program_path, "--file", test_path],
        capture_output=True,
        text=True
    )

    return (result.returncode == 0, result.stdout, result.stderr)


def snap(tests_folder: str, program: str) -> None:
    files = listdir(tests_folder)
    outs = []
    for file in files:
        if not path.isdir(file) and not file.endswith(".snap"):
            test_path = path.join(tests_folder, file)
            ok, out, err = run_test(program, test_path)
            if not ok:
                print(
                    f"ERROR: test {test_path} failed: {err}. Aborting snaping.",
                    file=stderr
                )

            outs.append((test_path, out))

    for test_resul in outs:
        snap_path = test_resul[0] + ".snap"
        try:
            with open(snap_path, "w+") as f:
                f.write(test_resul[1])

            print(f"SNAPED OK: {test_resul[0]} -> {snap_path}")
        except Exception as e:
            print(f"SNAPED FAILED: {test_resul[0]} {e}")

def run(tests_folder: str, program: str) -> None:
    files = listdir(tests_folder)
    passed = 0
    total = 0
    start = time.perf_counter()
    for file in files:
        if not path.isdir(file) and not file.endswith(".snap"):
            total += 1
            test_path = path.join(tests_folder, file)
            ok, actual, err = run_test(program, test_path)
            if not ok:
                print(f"TEST FAILED -> {test_path}: {err}")
            else:
                try:
                    with open(test_path + ".snap", "r") as f:
                        expected = f.read()
                        if expected != actual:
                            print(f"TEST FAILED -> {test_path}")
                            print(f"    Expected: {expected}")
                            print(f"    Actual: {actual}")
                        else:
                            print(f"TEST PASSED -> {test_path}")
                            passed += 1
                except Exception as e:
                    print(f"TEST FAILED -> {test_path}: {e}")
    end = time.perf_counter()

    print(
f"""
TEST SUMMARY:
    TESTS PASSED: {passed}/{total}
    TOTAL EXECUTION TIME: {end - start:.6f} seconds
""")

def usage(program_name):
    print(f"usage: {program_name} [run | snap] [PROJECT_FOLDER] [EXE_NAME]")
    exit(1)

def main() -> None:
    if len(argv) < 3:
        usage(argv[0])

    mode, projet_folder, program = argv[1:4]
    tests_folder = path.join(projet_folder, "tests")
    program_path = path.join(projet_folder, program)

    if mode == "run":
        run(tests_folder, program_path)
    elif mode == "snap":
        snap(tests_folder, program_path)
    else:
        usage(argv[0])

if __name__ == "__main__":
    main()