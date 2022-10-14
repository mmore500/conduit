import functools
import subprocess
import typing

from ._iter_header_paths import iter_header_paths

@functools.cache
def get_header_content() -> typing.List[str]:

    res = []
    for header_path in iter_header_paths():
        print(header_path)
        echo = subprocess.run(
            (
                "printf",
                f'#include "../../third-party/Empirical/include/emp/{header_path}" \n int main(){{}}',
            ),
            check=True,
            capture_output=True,
        )
        header_content = subprocess.run(
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
            input=echo.stdout,
            check=True,
            capture_output=True,
        ).stderr.decode()

        for line in header_content.splitlines():
            if "Empirical" in line:
                if ' ' in line:
                    line = line.split()[1]
                with open(line) as file:
                    res.append(file.read())
    return res
