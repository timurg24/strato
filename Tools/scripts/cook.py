# Cook.py
# Takes project root `Content` folder, compiles and optimizes files,
# and places them next to your executable.

from pathlib import Path
import os
import shutil
import subprocess
import sys


def find_shaderc(build_dir: Path) -> Path:
    executable = "shaderc.exe" if os.name == "nt" else "shaderc"

    matches = list(build_dir.rglob(executable))

    if not matches:
        raise FileNotFoundError(
            f"Could not find {executable} in {build_dir}"
        )

    return matches[0]


def find_bgfx_include() -> Path:
    repo_root = Path(__file__).resolve().parents[2]

    bgfx_include = (
        repo_root
        / "thirdparty"
        / "bgfx.cmake"
        / "bgfx"
        / "src"
    )

    shader_file = bgfx_include / "bgfx_shader.sh"

    if not shader_file.exists():
        raise FileNotFoundError(
            f"Could not find bgfx_shader.sh at {shader_file}"
        )

    return bgfx_include


def compile_shader(
    shaderc: Path,
    bgfx_include: Path,
    source: Path,
    output: Path,
    varying: Path,
    shader_type: str,
):
    output.parent.mkdir(parents=True, exist_ok=True)

    command = [
        str(shaderc),

        "-f",
        str(source),

        "-o",
        str(output),

        "-i",
        str(bgfx_include),

        "--platform",
        "windows",

        "--type",
        shader_type,

        "-p",
        "s_5_0",

        "-O",
        "3",

        "--varyingdef",
        str(varying),
    ]

    print(f"Compiling: {source}")
    print(f"       -> {output}")

    subprocess.run(command, check=True)


def optimize(
    path: Path,
    source_root: Path,
    content_output: Path,
    shaderc: Path,
    bgfx_include: Path,
):
    ext = path.suffix.lower()

    if ext != ".sc":
        return

    # This is used by shaderc, but should not itself be compiled.
    if path.name == "varying.def.sc":
        return

    if path.name == "vert.sc":
        shader_type = "vertex"

    elif path.name == "frag.sc":
        shader_type = "fragment"

    else:
        print(f"Skipping unknown shader: {path}")
        return

    varying = path.parent / "varying.def.sc"

    if not varying.exists():
        raise FileNotFoundError(
            f"Missing varying.def.sc for shader: {path}"
        )

    # Get the shader's path relative to Content.
    relative = path.relative_to(source_root)

    # Example:
    #
    # Content/Shaders/pbr/vert.sc
    #
    # becomes:
    #
    # <exe>/Content/Shaders/pbr/vert.bin

    relative_output = relative.with_suffix(".bin")
    output = content_output / relative_output

    compile_shader(
        shaderc=shaderc,
        bgfx_include=bgfx_include,
        source=path,
        output=output,
        varying=varying,
        shader_type=shader_type,
    )


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
    build_dir = Path(sys.argv[2]).resolve()
    output = Path(sys.argv[3]).resolve()

    if not source.exists():
        print(f"Content directory does not exist: {source}")
        return 1

    shaderc = find_shaderc(build_dir)
    bgfx_include = find_bgfx_include()

    # Content will be placed next to the executable.
    content_output = output / "Content"

    print("Wrangler Content Cooker")
    print("v1.0.0")
    print()

    print(f"Source:       {source}")
    print(f"Build:        {build_dir}")
    print(f"Output:       {output}")
    print(f"Shaderc:      {shaderc}")
    print(f"BGFX include: {bgfx_include}")
    print()

    # ---------------------------------------------------------
    # Copy the entire Content directory.
    # ---------------------------------------------------------

    print("Copying Content...")

    output.mkdir(
        parents=True,
        exist_ok=True,
    )

    shutil.copytree(
        source,
        content_output,
        dirs_exist_ok=True,
        ignore=shutil.ignore_patterns("*.sc"),
    )

    # ---------------------------------------------------------
    # Optimize / compile assets.
    # ---------------------------------------------------------

    print()
    print("Cooking assets...")

    for path in source.rglob("*"):
        if not path.is_file():
            continue

        optimize(
            path=path,
            source_root=source,
            content_output=content_output,
            shaderc=shaderc,
            bgfx_include=bgfx_include,
        )

    print()
    print("Content cooking complete.")
    print(f"Output: {content_output}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())