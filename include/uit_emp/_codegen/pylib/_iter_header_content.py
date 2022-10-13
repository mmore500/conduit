import subprocess
import typing

from ._iter_header_paths import iter_header_paths

def iter_header_content() -> typing.Iterator[str]:

    for header_path in iter_header_paths():
        echo = subprocess.Popen(
            (
                "printf",
                f'#include "../../third-party/Empirical/include/emp/{header_path}" \n int main(){{}}',
            ),
            stdout=subprocess.PIPE,
        )
        header_content = subprocess.check_output(
            [
                "g++",
                "-std=c++17",
                "-x",
                "c++",
                "-",
                "-H",
                "-o",
                "/dev/null",
            ],
            stdin=echo.stdout,
            stderr=subprocess.STDOUT,
        )
        for line in header_content.decode().splitlines():
            if "Empirical" in line:
                if ' ' in line:
                    line = line.split()[1]
                print(line)
                with open(line) as file:
                    yield file.read()
