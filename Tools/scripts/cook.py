
# cook.py
# Copies the project's Content directory next to the executable.
# Wrangler Content Cooker

from pathlib import Path
import shutil
import sys


def main():
    if len(sys.argv) != 4:
        print(
            "Usage: cook.py "
            "<source Content folder> "
            "<build directory> "
            "<executable directory>"
        )
        return 1

    source = Path(sys.argv[1]).resolve()
    output = Path(sys.argv[3]).resolve()

    if not source.is_dir():
        print(f"Content directory does not exist: {source}")
        return 1

    content_output = output / "Content"

    print("Wrangler Content Cooker")
    print("v1.0.0")
    print()

    print(f"Source: {source}")
    print(f"Output: {output}")
    print()

    print("Copying Content...")

    output.mkdir(
        parents=True,
        exist_ok=True,
    )

    shutil.copytree(
        source,
        content_output,
        dirs_exist_ok=True,
    )

    print()
    print("Content cooking complete.")
    print(f"Output: {content_output}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
