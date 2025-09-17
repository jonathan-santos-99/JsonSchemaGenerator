#!/usr/bin/env python3

import base64
import subprocess
import time
from os import listdir, path
from sys import argv, exit, stderr


def decode64(s: str) -> str:
    return base64.b64decode(bytes(s, "ascii")).decode("utf-8")

def run_test(program_path: str, test_path: str) -> tuple[int, str, str]:
    result = subprocess.run(
        [program_path, "--file", test_path],
        capture_output=True,
        text=True
    )

    return (result.returncode, result.stdout, result.stderr)


def snap(tests_folder: str, program: str) -> None:
    files = listdir(tests_folder)
    outs = {}
    for file in files:
        if not path.isdir(file) and not file.endswith(".snap"):
            test_path = path.join(tests_folder, file)
            outs[test_path] = (run_test(program, test_path))

    for test_path in outs:
        test_result = outs[test_path]
        snap_path = test_path + ".snap"
        try:
            with open(snap_path, "w+") as f:
                f.writelines([
                    str(test_result[0]),
                    "\n",
                    base64.b64encode(bytes(test_result[1], "utf-8")).decode("ascii"),
                    "\n",
                    base64.b64encode(bytes(test_result[2], "utf-8")).decode("ascii")
                ])
            print(f"SNAPED OK: {test_path} -> {snap_path}")
        except Exception as e:
            print(f"SNAPED FAILED: {test_path} {e}")

def run(tests_folder: str, program: str) -> None:
    files = listdir(tests_folder)
    passed = 0
    total = 0
    start = time.perf_counter()
    for file in files:
        if not path.isdir(file) and not file.endswith(".snap"):
            total += 1
            test_path = path.join(tests_folder, file)
            errcode, actual, err = run_test(program, test_path)
            try:
                with open(test_path + ".snap", "r") as f:
                    expected = f.readlines()
                    expected = list(map(lambda x: x.strip(), expected))
                    expected_code = int(expected[0])
                    expected_stdout = decode64(expected[1])
                    expected_stderr = ''
                    if len(expected) == 3:
                        expected_stderr = decode64(expected[2])

                    test_passed = (expected_code == errcode and
                                   expected_stdout == actual and
                                   expected_stderr == err)

                    if test_passed:
                        print(f"TEST PASSED -> {test_path}")
                        passed += 1
                    else:
                        print(f"TEST FAILED -> {test_path}")
                        print(f"    EXPECTED:")
                        print(f"        ERRCODE: {expected_code}")
                        print(f"        STDOUT : {expected_stdout}")
                        print(f"        STDERR : {expected_stderr}")
                        print(f"    ACTUAL:")
                        print(f"        ERRCODE: {errcode}")
                        print(f"        STDOUT : {actual}")
                        print(f"        STDERR : {err}")
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